#include "./include/video.h"
#include "./x86/include/Machine.h"


// Variable to hold where should we start rendering
// this avoids us to render the entire screen every time
// something is printed
Video::Video() {
	// default write mode is arabic
	writeMode = Arabic;
	_x = 0;
    _y = 0;
    _attribute = 0x0f;
    mem = (BYTE*)(0xb8000);
    width = 80; 
    height = 25;
    reset = false;
    
    map = new FontMap();
	map->loadDefaultMap();
	clear();
}



  
void Video::vprintf(const char *format, va_list ap)  
{	// reinit yfrom
	yfrom = _y;
	
	char c;
	int num;
	
	//buf will contain the converted number
	char buf[20];
	
	while((c = *format++) != 0)
	{
		if (c == '%') {
			const char *p;
			c = *format++;
			if(!c)
				break;
			switch (c) {
				case '%':
					putAt('%');
					break;
				case 'd':
				case 'u':
				case 'x':
					num = va_arg(ap,unsigned int);
					intToString (buf, c, num);
					for (p = buf; *p != 0; p++) 
						putAt(*p);
					break;
				case 'c':
					num = va_arg(ap,unsigned int);
					putAt((char) num);
					break;
				case 's': {
					char *string = va_arg(ap,char *);
					if(!string)
						string = "(null)";
					for (; *string != 0; string++) 
						putAt(*string);
					break;
				}
				default:
					num = va_arg(ap,unsigned int);
					putAt(num);
					break;
			}
			
		} else {
			putAt(c);
		}
	}
	for (int i = yfrom; i <= _y; ++i) {
		map->process(screen+(i*WIDTH), visual+(i*WIDTH), writeMode);
	}
	updateScreen();
}

void Video::printf(const char *format, ...){
	va_list ap;
 
	va_start(ap, format);
  	vprintf(format, ap);
  	va_end(ap);	
  	updateCursor();
}

void Video::printfAt(int x, int y, const char *format, ...){
	if(x<0 || x>=width || y<0 || y>=height)
		return;
	int ox = _x;
	int oy = _y;
	moveTo(x,y);
	va_list ap;
 
	va_start(ap, format);
  	vprintf(format, ap);
  	va_end(ap);	
  	moveTo(ox, oy);
}


void Video::moveTo(int x, int y)
{
  _x = x;
  _y = y;
}

void Video::updateCursor()
{
  
  // calculate the linear location, translate x if necessary
  WORD loc = (_y * width) + translate(_x);
  
  // first we write the low byte of the new cursor's position
  Machine::outb(0x3D4, 0x0F);
  Machine::outb(0x3D5,  loc & 0xFF);
  // the we write the high byte of the new cursor's position
  Machine::outb(0x3D4, 0x0E);
  Machine::outb(0x3D5,   (loc>>8) & 0xFF);
}

// scroll the logical screen
void Video::scroll(int dy)
{ int bottom = height-dy;
  // scroll up
  memcpy(screen, screen + (dy * width), (height-dy) * width);
  memcpy(visual, visual + (dy * width), (height-dy) * width);
  // blank the bottom lines of the screen
  memset(screen+(bottom*width), 0, width*dy);
  memset(visual+(bottom*width), 0, width*dy);
  reset = true;
}
 
// clear both logical and visual screen
void Video::clear()
{
	memset(screen, 0, width*height);
	memset(visual, 0, width*height);
	for (int i = 0; i < WIDTH*HEIGHT*2; i+=2) {
		mem[i] = ' ';
		mem[i+1] = _attribute;
	}
	_x = 0;
	_y = 0;
	yfrom = 0;
	yto = 0;
	updateCursor();
}

void Video::attribute(BYTE attribute) {
	_attribute = attribute;
}

// return the current attribute
BYTE Video::attribute() const {
	return _attribute;
}

// return the cursor's current horizontal position
int Video::x() const { return _x; }

// the cursor's current vertical position
int Video::y() const { return _y; }
