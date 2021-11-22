#include "./include/FontMap.h"
#include "./x86/include/Machine.h"


Braces braces[] = {
	{'{','}'} , {'}','{'} , {'[',']'} , {']','['} , {'(',')'} , {')','('}
	, {'<','>'} , {'>','<'} , 
	{0,0}
};

Pairs pairs[] = {
	{0xfa,0x96,0xfa,0xa2},
	{0xfa,0x9d,0xfa,0xa3},
	{0xfa,0x9e,0xfa,0xa4},
	{0xfa,0x9f,0xfa,0xa1},
	{0xd3,0x20,0x91,0x95},
	{0xd4,0x20,0x92,0x95},
	{0xd5,0x20,0x93,0x95},
	{0xd6,0x20,0x94,0x95},
	{0xd3,0xa0,0x91,0x95},
	{0xd4,0xa0,0x92,0x95},
	{0xd5,0xa0,0x93,0x95},
	{0xd6,0xa0,0x94,0x95},
	{0,0,0,0}	
};

GlyphMap glyphMap[] = {
	{0xa8,0xa8,0xa8,0xbc,0xa8},
	{0xa9,0xa9,0xa9,0xbd,0xa9},
	{0xaa,0xaa,0xaa,0xbe,0xaa},
	{0xab,0xab,0xab,0xdb,0xab},
	{0xac,0xac,0xac,0xdc,0xac},
	{0xad,0xad,0xad,0xdd,0xad},
	{0xae,0xae,0xae,0xde,0xae},
	{0xaf,0xaf,0xaf,0xdf,0xaf},
	{0xc2,0xc2,0xc2,0x96,0x96},
	{0xc3,0xc3,0xc3,0x9d,0x9d},
	{0xc5,0xc5,0xc5,0x9e,0x9e},
	{0xc6,0xc6,0xc0,0xc0,0xc6},
	{0xc7,0xc7,0xc7,0x9f,0x9f},
	{0xc8,0xc8,0xeb,0xeb,0xc8},
	{0xca,0xca,0xec,0xec,0xca},
	{0xcb,0xcb,0xed,0xed,0xcb},
	{0xcc,0xcc,0xee,0xee,0xcc},
	{0xcd,0xcd,0xef,0xef,0xcd},
	{0xce,0xce,0xf0,0xf0,0xce},
	{0xd3,0xd3,0xf1,0xf1,0xd3},
	{0xd4,0xd4,0xf2,0xf2,0xd4},
	{0xd5,0xd5,0xf3,0xf3,0xd5},
	{0xd6,0xd6,0xf4,0xf4,0xd6},
	{0xd9,0xd9,0xf5,0x97,0x99},
	{0xda,0xda,0xf6,0x98,0x9a},
	{0xe1,0xe1,0xf7,0xf7,0xe1},
	{0xe2,0xe2,0xf8,0xf8,0xe2},
	{0xe3,0xe3,0xf9,0xf9,0xe3},
	{0xe4,0xe4,0xfa,0xfa,0xe4},
	{0xe5,0xe5,0xfb,0xfb,0xe5},
	{0xe6,0xe6,0xfc,0xfc,0xe6},
	{0xe7,0xe7,0xfd,0x9b,0xe7},
	{0xe9,0xe9,0xfe,0xfe,0x8f},
	{0xea,0xea,0xfe,0xfe,0x90},
	{0,0,0,0,0}
};



// code pages: chars ares ordered in classe
// after breakers, before (and after) breakers,
// joiners, numbers and others
// each class is marked by a preceding dummy
// struct of the form {255, numclass}
//******************** Code page iso-8859-6 ****************/

CodePage iso_8859_6[] = {
    
// After brekers
	{255,AFT_BREAK},
	{0xc2,0xc2},
	{0xc3,0xc3},
	{0xc4,0xc4},
	{0xc5,0xc5},
	{0xc7,0xc7},
	{0xc9,0xc9},
	{0xcf,0xcf},
	{0xd0,0xd0},
	{0xd1,0xd1},
	{0xd2,0xd2},
	{0xe8,0xe8},

// before and after breakers
	{255,BEF_BREAK},
	{0xc1,0xc1},

// joiners
	{255,JOIN},
	{0xc6,0xc6},
	{0xc8,0xc8},
	{0xca,0xca},
	{0xcb,0xcb},
	{0xcc,0xcc},
	{0xcd,0xcd},
	{0xce,0xce},
	{0xd3,0xd3},
	{0xd4,0xd4},
	{0xd5,0xd5},
	{0xd6,0xd6},
	{0xd7,0xd7},
	{0xd8,0xd8},
	{0xd9,0xd9},
	{0xda,0xda},
	{0xe0,0xe0},
	{0xe1,0xe1},
	{0xe2,0xe2},
	{0xe3,0xe3},
	{0xe4,0xe4},
	{0xe5,0xe5},
	{0xe6,0xe6},
	{0xe7,0xe7},
	{0xe9,0xe9},
	{0xea,0xea},

	
// Numbers
	{255,NUMBER},
	{48,48},
	{49,49},
	{50,50},
	{51,51},
	{52,52},
	{53,53},
	{54,54},
	{55,55},
	{56,56},
	{57,57},
	{58,58},

/*end 255*/
    {255,OTHER}
};




/******************** Code page cp1256 ****************/

CodePage cp1256[] = {
    
	{255,AFT_BREAK},
	{0xc2,0xc2},
	{0xc3,0xc3},
	{0xc4,0xc4},
	{0xc5,0xc5},
	{0xc7,0xc7},
	{0xc9,0xc9},
	{0xcf,0xcf},
	{0xd0,0xd0},
	{0xd1,0xd1},
	{0xd2,0xd2},
	{0xe6,0xe8},

	{255,BEF_BREAK},
	{0xc1,0xc1},

	{255,JOIN},
	{0xc6,0xc6},
	{0xc8,0xc8},
	{0xca,0xca},
	{0xcb,0xcb},
	{0xcc,0xcc},
	{0xcd,0xcd},
	{0xce,0xce},
	{0xd3,0xd3},
	{0xd4,0xd4},
	{0xd5,0xd5},
	{0xd6,0xd6},
	{0xd8,0xd7},
	{0xd9,0xd8},
	{0xda,0xd9},
	{0xdb,0xda},
	{0xdc,0xe0},
	{0xdd,0xe1},
	{0xde,0xe2},
	{0xdf,0xe3},
	{0xe1,0xe4},
	{0xe3,0xe5},
	{0xe4,0xe6},
	{0xe5,0xe7},
	{0xec,0xe9},
	{0xed,0xea},

	
	{255,NUMBER},
	{48,48},
	{49,49},
	{50,50},
	{51,51},
	{52,52},
	{53,53},
	{54,54},
	{55,55},
	{56,56},
	{57,57},
	{58,58},

    {255,OTHER}
};




static CodePage *defcp;		// holds the used code page


// Some inline functions to read and write
// VGA ports
// port: class registers (Graphics, sequencer)
// index : register index
// value: value to store
inline void outVGA(WORD port, BYTE index, BYTE value) {
	Machine::outb(port, index);
	Machine::outb(port+1, value);	
}

inline BYTE inVGA(WORD port, BYTE index) {
	Machine::outb(port, index);
	return Machine::inb(port+1);	
}


#define	VGA_SEQ	0x3C4	// Sequencer port adress
#define	VGA_GR	0x3CE	// Graphics port address


// used to temporarly save registers
static BYTE seq2, seq4, gr0, gr[9];


void saveVGARegs() {
		// save sequencer registers
		seq2 = inVGA(VGA_SEQ, 2);
		seq4 = inVGA(VGA_SEQ, 4);
		
		//save Graphics register
		for (int i = 0; i < 9; ++i) {
			gr[i] = inVGA(VGA_GR, i);
		}
}

void restoreVGARegs() {
	// restore sequencer registers
	outVGA(VGA_SEQ, 0, 1);	// synchrnous reset
	outVGA(VGA_SEQ, 2, seq2);
	outVGA(VGA_SEQ, 4, seq4);
	outVGA(VGA_SEQ, 1, 1);	// selects 8 dots per char
	outVGA(VGA_SEQ, 0, 3);	// clear synchrnous reset
	
	
	// restore Graphics registers
	for (int i = 0; i < 9; ++i) {
			outVGA(VGA_GR, i, gr[i]);
		}
}

// this variable points to the begining
// of the font table, see multiboot.s
extern char arGlyphs;


// load font table in VGA memory
void FontMap::loadDefaultMap() {
	char *mem = (char *) 0xA0000;  // VGA adress
	char *gl = &arGlyphs;   // adress of the font table
	
	saveVGARegs();
	
	// start inititialization
	outVGA(VGA_SEQ, 0, 1);	// synchrnous reset
	outVGA(VGA_SEQ, 2, 4);	// only write to map 2
	outVGA(VGA_SEQ, 4, 7);	// Disable O/E, enable Extended memory	
	outVGA(VGA_SEQ, 0, 3);	// clear synchrnous reset
	
	outVGA(VGA_GR, 1, 0);	// disable set/reset
	outVGA(VGA_GR, 3, 0);	// logical operation=replace, rotate count=0
	outVGA(VGA_GR, 4, 2);	// selects map 2 for cpu reads
	outVGA(VGA_GR, 5, 0);	// write mode=0, read mode=0
	outVGA(VGA_GR, 6, 0);
	
	// load the font map into the first 
	// Character map at 0xa0000
	for (int i = 0; i < 256; ++i)  {
		for (int j = 0; j < 16; ++j) {
			mem[i*32+j] = gl[i*16+j];		
		}	
	}
		
	restoreVGARegs();
}

// actually i'm workin under Windows & eclipse, and i set
// cp1256 in order to write arabic letters directly in the
// source code, if you change thi define to iso-8859-6
// in ordre to use an editor supporting this charset
// dont forget to change char strings inside source code
FontMap::FontMap()
{
	#ifdef ISO
		defcp = iso_8859_6
	#else
		defcp = cp1256;
	#endif
}

// arabic letters in iso table
inline bool isArabic(CHAR c) {
	return ( (c >= 0xc1 && c <= 0xda) ||
				(c>=0xe1 && c<=0xea) || c == 0xa0 /* arab space*/);
}

inline bool isLatin(CHAR c) {
	return ( (c >= 'A' && c <= 'Z') ||
				(c>='a' && c<='z'));
}

inline bool isNumber(CHAR c) {
	return (c >= '0' && c <= '9');
}

// reverse the order of 'len' chars
inline void reverse(CHAR *buf, int len) {
	CHAR *head, *tail;
	
	head = buf;
	tail = buf+len;
	while (head < tail) {
		char tmp = *head;
		*head = *tail;
		*tail = tmp;
		head++;
		tail--;
	}
}

BYTE *mem = (BYTE *)(0xB8000+80*10);
// reverse the order of arab letters and other symbols
// numbers remain unchanged
inline void reverseAr(CHAR *buf, int len1) {
	// first reverse all chars
	int endNumber = 0; // hold the last number
	reverse(buf, len1);
	// restore numbers to their correct position
	for (int i = 0; i < len1; ++i) {
		if(isNumber(buf[i])) {
			int x = i;
			for (; x < len1 && (isNumber(buf[x]) ||
				buf[x] == ' ' || buf[x] == '.' || buf[x] == ',')
				; ++x) {
					if(isNumber(buf[x])) 
						endNumber = x;	
				}
			// endNumber points to the last number
			// x-1 points to the first non number letter
			reverse(&buf[i], endNumber-i);
			i = x-1;
		}
	}
}


static CHAR types[WIDTH];   // to hold char type: AFT_BREAK ...
static CHAR *newLine;
static CHAR natif[WIDTH];	// char codes before shaping
static CHAR *line;			// line to process

// translate char from the actual code page to
// font table number
inline void findCharAndType(CHAR c, int i) {
	for (int j = 0; j < 255; ++j) {
		CodePage *page = defcp+j;
		if(page->code == 255) {
			// retreive the curent type
			types[i] = page->glyph;
		}
		else if(page->code == c) {
			natif[i] = newLine[i] = page->glyph;
			return;
		}
	}	
}

// return the corresponding glyphMap entry
inline int getGlyphMap(CHAR ch) {
	for (int i = 0; glyphMap[i].chr!=0; ++i) {
		if(glyphMap[i].chr == ch)
			return i;
	}	
	return -1;
}


// transform shape at position 'i'
inline void reshapeCharAt(int i) {
	// retreive types for previous and next char
	// force OTHER if no previous or next
	CHAR xt = (i!=0)?types[i-1]:OTHER;
	CHAR zt = (i!=WIDTH-1)?types[i+1]:OTHER;
	
	// find the corresponding entry in the glyph map
	int gm = getGlyphMap(natif[i]);
	// not found: nothing to do
	if(gm < 0)
		return;
	
	
	// can we connect with previous ?
	if( xt!=AFT_BREAK && xt != OTHER ) {
	// We can connect: either middle or last glyph
		// Can we connect with next ?
		if(zt!=BEF_BREAK && zt != OTHER)
			// We can connect: middle glyph
			newLine[i] = glyphMap[gm].middle;
		else
			// We can't connect: last glyph
			newLine[i] = glyphMap[gm].last;	
	// We can't connect with previous:
	// either first or alone glyph
	// Now can we connect with next ?
	} else if(zt!=BEF_BREAK  && zt != OTHER) {
	// We can connect with next: first glyph
			newLine[i] = glyphMap[gm].first;	
	} else 
	// We can't connect also with next: alone glyph
			newLine[i] = glyphMap[gm].alone;	
}

// transform shapes according to their position
inline void processShapes() {
	for (int i = 0; i < WIDTH; ++i) {
		if(isArabic(natif[i]))
			reshapeCharAt(i);
	}
	
}


// Process bidirictional text in arabic mode
// Latin letters & numbers order is inverted
// Arab letters remain unchanged
// for other chars: follow the current direction
// space also follow the current direction unless
inline void processBidiAr() {
	int endLatin = 0;  // holds the last latin letter 
	for (int i = 0; i < WIDTH; ++i) {
		// if a latin text is encountred
		// we change its order
		if (isLatin(natif[i]) || isNumber(natif[i])) {
			int j = endLatin = i;
			// we continue scanning until an arab letter
			// is encountred
			for (; j < WIDTH && !isArabic(natif[j]) && 
				natif[j]!=0; ++j) {
				
				if(isLatin(natif[j]) || isNumber(natif[j]))
					endLatin = j;
			}
			// EndLatin points to the last Latin letter
			// j points to the first letter arab or line end
			reverse(&newLine[i], endLatin-i);
			i = j-1;
		// some thing for numbers but others symbols
		// dont follow their directions
		}
	} 
}

// Process bidirictional text in Latin mode
inline void processBidiLat() {
	int endArabic = 0;  // holds the last latin letter 
	// change order of arab letters in latin mode
	for (int i = 0; i < WIDTH; ++i) {
		if(isArabic(natif[i])) {
			int j = endArabic = i;
			for (; j < WIDTH && !isLatin(natif[j])
			&& natif[j]!=0 ; ++j) {
				if(isArabic(natif[j]))
					endArabic = j;
			}
			reverseAr(&newLine[i], endArabic-i);
			i = j-1;
		}
	}
}


// replace old pairs (like lam and alef) withe 
// 2 connected glyphs to simulate 'one' letter
inline int changePair(int ind, CHAR c1, CHAR c2) {
	if(!c2) c2 = ' ';
	for (int j = 0; pairs[j].old1!=0; ++j) {
			if(pairs[j].old1 == c1 && pairs[j].old2 == c2) {
				newLine[ind] = pairs[j].new1;
				newLine[ind+1] = pairs[j].new2;
				return 1;
			}
	}
	return 0;
}

// process pairs
inline void transformPairs() {
	CHAR c1, c2;
	for (int i = 0;  i < WIDTH-1; i++) {
		// if changePair succeed with i & i+1
		// it returns 1 so there is no need to process the
		// following letter
		i+changePair(i, newLine[i], newLine[i+1]);
	}	
}


// find the matching brace
inline CHAR getBrace(CHAR chr) {
	for (int i = 0;braces[i].in!=0 ; ++i) {
		if(braces[i].in == chr) {
			return braces[i].out;
		}
	}	
	return chr;
}


// transform braces orientation in arabic mode
inline void transformBraces() {
	for (int i = 0; i < WIDTH; ++i) {
		newLine[i] = getBrace(newLine[i]);
	}	
}

// process a line int Write mode 'mode'
// buf: the logical line, ie not yet processed
// mode: Current write mode: arabic or latin
CHAR* FontMap::process(CHAR *buf, CHAR* visual, WriteMode mode) {
	line = buf;
	newLine = visual;
	CHAR c;
	// 1-retreives codes and type of chars
	for (int i = 0; i < WIDTH; ++i) {
		c=line[i];
		if(c < 127) {
			natif[i] = newLine[i] = c;
			types[i] = OTHER;
		} else
			findCharAndType(c, i);
	}	
	
	// 2- reshape chars according to their relatif position	
	processShapes();
	
	// 3- transform pairs (like lam alef)
	transformPairs();
	
	if(mode == Arabic) {
		// 4-1 transform braces orientation
		transformBraces();
		
		// 4-2 process the order of Latin letters and numbers
		processBidiAr();
	}
	else
		// 4- process the order of Arab letters
		processBidiLat();
	
	// return the final visual representation	
	return newLine;
}

