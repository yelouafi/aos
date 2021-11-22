#ifndef VIDEO_H_
#define VIDEO_H_

#include"system.h"

class Video
{
private:
  // cursor's position
  int _x, _y;

  // current attribute
  BYTE _attribute;

  // pointer to video memory
  BYTE *mem;

  // screen width and height
  const int width, height;

public:
  Video();

  // clears the screen
  void clear();

  //puts a character at the current location
  // c The character's ASCII code
  void put(unsigned char c);

  // A simple printf function
  void printf (const char *format, ...);

  // moves the cursor to the given position
  // x The cursor's new x coordinate, -1 for no change
  // y The cursor's new y coordinate, -1 for no change
  void moveTo(int x, int y);
  
  // update the cursor's position in screen to
  // reflect the actual (x,y) position
  void updateCursor();

	
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

