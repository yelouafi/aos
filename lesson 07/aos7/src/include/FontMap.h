#ifndef FONTMAP_H_
#define FONTMAP_H_

#include"system.h"

// screen height & width
#define WIDTH	80
#define HEIGHT	25

// Character type: before breaker, joiner...
#define AFT_BREAK 	1
#define BEF_BREAK 	2
#define JOIN 		3
#define NUMBER	 	5
#define OTHER	 	255		// other than arab latters


enum WriteMode { Arabic, Latin };

// Code page to translate from char codes
// to glyphs in the font table
struct CodePage {
	DWORD code;
	CHAR glyph;
};

// gives differents glyphs for an arab letters
// according to its position
struct GlyphMap {
	CHAR chr, alone, first, middle, last;	
};


// Braces correspondance to inverts the direction
// in arab mode ex ')' -> '('
struct Braces {
	CHAR in, out;	
};


// transform pairs to 'one' glyph
// ex. lamalef
struct Pairs {
	CHAR old1, old2, new1, new2;
};

class FontMap
{
public:
	FontMap();

	void loadDefaultMap();	
	CHAR* process(CHAR *buf, CHAR *visual, WriteMode mode);
};

#endif /*FONTMAP_H_*/
