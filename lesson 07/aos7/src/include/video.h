#ifndef VIDEO_H_
#define VIDEO_H_

#include"system.h"
#include"FontMap.h"


class Video
{
private:
	int yfrom, yto;
	bool reset;
	// this buffer holds the logical screen
	// it will contain the chars printed in the console
	// but without any processing, once a line int 
	// this buffer is processed by FontMap::process
	// it will be rendered to the visual screen
	
	CHAR screen[WIDTH*HEIGHT];
	CHAR visual[WIDTH*HEIGHT];
	
	WriteMode writeMode;
	
	// cursor's position
	int _x, _y;
	// current attribute
	BYTE _attribute;
	// pointer to video memory
	 BYTE *mem;
	// screen width and height
	int width, height;
	  
	// put a character but dont update the hardware cursor
	// used by printf to update the hardware cursor
	// only once the entire string is printed
	inline void putAt(unsigned char c)
	{	if(c == 0x08)
	    {
	        if(_x != 0) _x--;
	    }
	
	    else if(c == '\r')
	    {
	        _x= 0;
	    }
	
	    else if(c == '\n')
	    {   _x= 0;
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
	
		if(_y >= height) {
			int dy = _y-height+1;
			// scroll also works only on logical screen
			scroll(dy);
			_y -= dy;
			yfrom -= dy;
		}
	};
	
	// translate the x position according to the
	// current write mode
	inline int translate(int x) {
		return (writeMode != Arabic)?x:width-x-1;	
	};
	
	// render a processed line in the visual screen
	inline void renderLine(int y, CHAR *line) {
		for (int j = 0; j < width; ++j) {
			int loc2 = ((y*width)+translate(j))*2;
			mem[loc2] = (line[j]!=0)?line[j]:' '; // if no char print ' '
			mem[loc2+1] = _attribute;
			}
	};
	
	// update the screen from the line 'yfrom' at 
	// the current line '_y'
	inline void updateScreen() {
		int from = (!reset)?yfrom:0;
		int to = (!reset)?_y:height-1;
		for (int i = from; i <= to; ++i) {
			renderLine(i, visual+(i*WIDTH));
		}
		reset = false;
	};
	
	// const is private
	Video();
	
	friend class Kernel;
public:
  
  // clears the screen
  void clear();
  
  // The class char processor
	FontMap *map;


	// c is the char code expressed in the 
	// current code page 
	inline void put(unsigned char c) {
		yfrom = _y;
		putAt(c);
		for (int i = yfrom; i <= _y; ++i) {
			map->process(screen+(i*WIDTH), visual+(i*WIDTH), writeMode);
		}
		updateScreen();
		updateCursor();
	}

  // A simple printf function
  void printf (const char *format, ...);
  void printfAt (int x, int y, const char *format, ...);
  void vprintf (const char *format, va_list ap);

  // moves the cursor to the given position
  // x The cursor's new x coordinate, -1 for no change
  // y The cursor's new y coordinate, -1 for no change
  void moveTo(int x, int y);
  
  // update the cursor's position in screen to
  // reflect the actual (x,y) position
  void updateCursor();


  // set the write mode arabic, english
  inline void setWriteMode(WriteMode newMode) { 
  	writeMode = newMode; 
  }
  
  inline WriteMode getWriteMode() {
  	return writeMode;
  }
  
	
  // scrolls the screen by the given vertical amount
  // y The numbers of lines to scroll
  void scroll(int dy);
	
  // sets the attribute for the next writes
  // att The character's attribute
  void attribute(BYTE attribute);
	
  // return the current attribute
  BYTE attribute() const;

  // return the cursor's current horizontal position
  int x() const;

  // the cursor's current vertical position
  int y() const;
}
;

#endif /*VIDEO_H_*/

