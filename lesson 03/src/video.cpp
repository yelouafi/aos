#include"video.h"

Video::Video()
  : _x(0),
    _y(0),
    _attribute(0x0f),
    mem(reinterpret_cast<BYTE*>(0xb8000)),
    width(80),
    height(25)
{
}


void Video::put(char c)
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
        int index = (_y*width + _x) * 2;
  
        // puts the character
        mem[index] = c;
        mem[index+1] = _attribute;

        _x++;
    }

    if(_x >= width)
    {
        _x= 0;
        _y++;
    }

	if(_y >= height)
		scroll(_y-height+1);

    updateCursor();
}
  
  
void Video::printf (const char *format, ...)  
{	
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
			switch (c) {
				case '%':
					put('%');
					break;
				case 'd':
				case 'u':
				case 'x':
					num = *(reinterpret_cast<int *> (arg));
					arg++;
					intToString (buf, c, num);
					for (p = buf; *p != 0; p++) 
						put(*p);
					break;
	
				case 's':
					for (p = *arg; *p != 0; p++) 
						put(*p);
					arg++;
					break;
				default:
					num = *(reinterpret_cast<int *> (arg));
					arg++;
					put (num);
					break;
			}
			
		} else {
			put(c);	
		}
	}
}

void Video::moveTo(int x, int y)
{
  _x = x;
  _y = y;

  // update the VGA cursor
	updateCursor();
}

void Video::updateCursor()
{
  
  // calculate the linear location
  WORD loc = (_y * width) + _x;
  
  // first we write the low byte of the new cursor's position
  outb(0x3D4, 0x0F);
  outb(0x3D5,  loc & 0xFF);
  // the we write the high byte of the new cursor's position
  outb(0x3D4, 0x0E);
  outb(0x3D5,   (loc>>8) & 0xFF);
}


void Video::scroll(int y)
{
  // scroll up
  memcpy(mem, mem + (y * width * 2), (height - y) * width * 2);
  
  // blank the bottom lines of the screen
  for (int x=0; x< (width*2); x+=2)
  {
    for (int y=(height - y)*2; y<(height*2); y+=2)
    {
      int loc = x + (y * width);
      mem[loc] = ' ';
      mem[loc+1 ] = _attribute;
    }
  }
}


void Video::clear()
{
  for (int x=0; x<width; ++x)
  {
    for (int y=0; y<height; ++y)
    {
      put(' ');
    }
  }

  _x = 0;
  _y = 0;
}

void Video::attribute(BYTE attribute) {
	_attribute = attribute;
}


BYTE Video::attribute() const {
	return _attribute;
}

  // return the cursor's current horizontal position
int Video::x() const { return _x; }

  // the cursor's current vertical position
int Video::y() const { return _y; }
