#include"video.h"
#include "Machine.h"


// this buffer holds the logical screen
// it will contain the chars printed in the console
// but without any processing, once a line int 
// this buffer is processed by FontMap::process
// it will be rendered to the visual screen

static CHAR screen[WIDTH*HEIGHT];

// Variable to hold where should we start rendering
// this avoids us to render the entire screen every time
// something is printed
static int yfrom; 

Video::Video() {
	// default write mode is arabic
	writeMode = Arabic;
	_x = 0;
    _y = 0;
    _attribute = 0x0f;
    mem = (BYTE*)(0xb8000);
    width = 80;
    height = 25;
}

void Video::putAt(unsigned char c)
{	
    if(c == 0x08)
    {
        if(_x != 0) _x--;
    }

    else if(c == '\r')
    {
        _x= 0;
    }

    else if(c == '\n')
    {
        _x= 0;
        _y++;
    }
 
    else if(c >= ' ')
    {
       // actually, we output chars in the logical screen first
        screen[_y*width + _x] = c;
        _x++;
    } else
    	screen[_y*width + _x] = '.';

    if(_x >= width)
    {
        _x= 0;
        _y++; 
    }

	if(_y >= height)
	// scroll also works only on logical screen
		scroll(_y-height+1);
}



// update the screen from the line 'yfrom' at 
// the current line '_y'
void Video::updateScreen() {
	
	for (int i = yfrom; i <= _y; ++i) {
		renderLine(i, 
			// map->process process the line at 'i'
			// and returns the result
       		map->process(screen+(i*WIDTH),  writeMode)
       	);
	}
	//reinit yfrom
	yfrom = _y;
}
  
void Video::printf(const char *format, ...)  
{	// reinit yfrom
	yfrom = _y;
	/* arg points to te first arg ie format: 
		consider args for kprintf as an array of pointers
		args = { format, arg1, arg2 ...}
		*/
	const char **arg = reinterpret_cast<const char **> (&format);
	char c;
	int num;
	
	//buf will contain the converted number
	char buf[20];
	
	// increments to points to the second argument
	arg++;
	
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
					num = *(reinterpret_cast<int *> (arg));
					arg++;
					intToString (buf, c, num);
					for (p = buf; *p != 0; p++) 
						putAt(*p);
					break;
				case 'c':
					num = *(reinterpret_cast<int *> (arg));
					arg++;
					putAt((char) num);
					break;
				case 's':
					for (p = *arg; *p != 0; p++) 
						putAt(*p);
					arg++;
					break;
				default:
					num = *(reinterpret_cast<int *> (arg));
					arg++;
					putAt(num);
					break;
			}
			
		} else {
			putAt(c);	
		}
	}
	updateScreen();
	updateCursor();
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
{
  // scroll up
  memcpy(screen, screen + (dy * width), (height-dy) * width);
  _y -= dy;
  
  // blank the bottom lines of the screen
  memset(screen+(_y*width), ' ', width*height);
}

// clear both logical and visual screen
void Video::clear()
{
	memset(screen, 0, width*height);
	for (int i = 0; i < WIDTH*HEIGHT*2; i+=2) {
		mem[i] = ' ';
		mem[i+1] = _attribute;
	}
	_x = 0;
	_y = 0;
	updateScreen();
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
