/****************************************
Daggorath PC-Port Version 0.2.1
Richard Hunerlach
November 13, 2002

The copyright for Dungeons of Daggorath
is held by Douglas J. Morgan.
(c) 1982, DynaMicro
*****************************************/

// Dungeons of Daggorath
// PC-Port
// Filename: dod.h
//
// This file contains common definitions of secondary objects
// used in the game.  It gets included in nearly every other
// file, so the SDL hearders are included here also.

#ifndef DOD_COMMON_HEADER
#define DOD_COMMON_HEADER

// PSP Headers
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <pspctrl.h>
#include <psppower.h>

// SDL Headers
#include <SDL/SDL_main.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define glColor3fv(x) glColor3f(x[0], x[1], x[2])

#define PSP_SCREEN_HEIGHT 	272
#define PSP_SCREEN_WIDTH	480

#define DISPLAY_STRETCHED	0
#define DISPLAY_FIT			1
#define DISPLAY_ORIGINAL	2

#define SDL_PSP_UP			8
#define SDL_PSP_DOWN		6
#define SDL_PSP_LEFT		7
#define SDL_PSP_RIGHT		9
#define SDL_PSP_CROSS		2
#define SDL_PSP_TRIANGLE	0
#define SDL_PSP_SQUARE		3
#define SDL_PSP_CIRCLE		1
#define SDL_PSP_START		11
#define SDL_PSP_SELECT		10
#define SDL_PSP_L_TRIGGER	4
#define SDL_PSP_R_TRIGGER	5

struct kbdKey{
	int x;
	int y;
	int width;
	int height;
	char key;
};

#define MAX_KEYBOARD_KEYS 30

	const struct kbdKey keyboardKeys[40] = { 		/*{ 92, 155, 23, 23, '1'},
													{115, 155, 23, 23, '2'},
													{138, 155, 23, 23, '3'},
													{161, 155, 23, 23, '4'},
													{184, 155, 23, 23, '5'},
													{207, 155, 23, 23, '6'},
													{230, 155, 23, 23, '7'},
													{253, 155, 23, 23, '8'},
													{276, 155, 23, 23, '9'},
													{299, 155, 23, 23, '0'},*/

													{103, 130, 23, 23, 'Q'},
													{126, 130, 23, 23, 'W'},
													{149, 130, 23, 23, 'E'},
													{172, 130, 23, 23, 'R'},
													{195, 130, 23, 23, 'T'},
													{218, 130, 23, 23, 'Y'},
													{241, 130, 23, 23, 'U'},
													{264, 130, 23, 23, 'I'},
													{287, 130, 23, 23, 'O'},
													{310, 130, 23, 23, 'P'},

													{107, 106, 23, 23, 'A'},
													{130, 106, 23, 23, 'S'},
													{153, 106, 23, 23, 'D'},
													{176, 106, 23, 23, 'F'},
													{199, 106, 23, 23, 'G'},
													{222, 106, 23, 23, 'H'},
													{245, 106, 23, 23, 'J'},
													{268, 106, 23, 23, 'K'},
													{291, 106, 23, 23, 'L'},
													{339, 106, 35, 23, 0x0D},

													{117,  80, 23, 23, 'Z'},
													{140,  80, 23, 23, 'X'},
													{163,  80, 23, 23, 'C'},
													{186,  80, 23, 23, 'V'},
													{211,  80, 23, 23, 'B'},
													{235,  80, 23, 23, 'N'},
													{258,  80, 23, 23, 'M'},
													{281,  80, 23, 23, ','},
													{304,  80, 23, 23, '.'},
													{146,  57, 188, 23, ' '}

												};

// The original source code used mostly 8-bit bytes and 16-bit
// words for RAM variable storages.  Many of the operations in
// the source are bit-level so there is a need to have precise
// types where the number of bits is known.  These types should
// be used until/unless it is certain that more abstract types
// (i.e., native C types) will work correctly.
typedef unsigned char	dodBYTE;
typedef unsigned short	dodSHORT;

// This class is used to translate between original 256x192
// coordinates and whichever modern coordinate system is used.
// It sets 512x384 as the default, but when the video system
// gets set up during initialization, these values are set to
// whatever screen resolution is chosen.
//
// This could be expanded later to do screen centering in cases
// where the available (or user-chosen) screen resolution is
// not an exact multiple of 256x192.
//
// The absolute versions are used for most things, and the
// relative versions are used for drawing the font
class Coordinate
{
public:
	Coordinate() : orgWidth(256.0L), orgHeight(192.0L),
				   curWidth(512.0L), curHeight(384.0L)
	{}

	// Sets the data members based on screen width
	// (assumes a 4/3 width/height ratio)
	void setCurWH(double W)
	{
		float scale = 1.76L;//0.75L;
		curWidth = ((int) W / (int) 256) * (int) 256;
		curHeight = (curWidth * scale);
		offX = (W - curWidth) / 2;
		offY = (offX * scale);
	}

	void setDisplay(int mode)
	{
		switch(mode)
		{
			case DISPLAY_STRETCHED:
				curWidth = PSP_SCREEN_WIDTH;
				curHeight = PSP_SCREEN_HEIGHT;
				offX = 0;
				offY = 0;
				break;
			case DISPLAY_FIT: // Creates a 4/3 ratio
				curWidth = PSP_SCREEN_HEIGHT/0.75;
				curHeight = PSP_SCREEN_HEIGHT;
				offX = (PSP_SCREEN_WIDTH - curWidth)/2;
				offY = 0;
				break;
			case DISPLAY_ORIGINAL:
				curWidth = orgWidth;
				curHeight = orgHeight;
				offX = (PSP_SCREEN_WIDTH - curWidth)/2;
				offY = (PSP_SCREEN_HEIGHT - curHeight)/2;
				break;
			default:
				break;
		}
	}

	// Calculates absolute screen X-coordinate based on DoD X-coordinate
	GLfloat newX(double orgX)
	{
		return ((GLfloat) ((orgX) / orgWidth * curWidth) + (GLfloat) offX);
	}

	// Calculates relative screen X-coordinate based on DoD X-coordinate
	GLfloat newXa(double orgX)
	{
		return ((GLfloat) ((orgX) / orgWidth * curWidth));
	}

	// Calculates absolute screen Y-coordinate based on DoD Y-coordinate
	GLfloat newY(double orgY)
	{
		return ((GLfloat)  ((orgHeight - (orgY)) / orgHeight * curHeight) + (GLfloat) offY);
	}

	// Calculates relative screen Y-coordinate based on DoD Y-coordinate
	GLfloat newYa(double orgY)
	{
		return ((GLfloat)  ((orgY) / orgHeight * curHeight));
	}

private:
	double orgWidth;
	double orgHeight;
	double curWidth;
	double curHeight;
	double offX;	// offsets in case the screen dimension is not
	double offY;	// an exact multiple of 256 or 192
};

// This class stores 32x32 row/column values
class RowCol
{
public:
	// Constructors
	RowCol() : row(0), col(0) {}
	RowCol(dodBYTE r, dodBYTE c) : row(r), col(c) {}
	RowCol(int idx) : row(idx/32), col(idx%32) {}

	// Mutator
	void setRC(dodBYTE r, dodBYTE c) { row=r; col=c; }

	// Fields
	dodBYTE row;
	dodBYTE col;
};

// The original source has several structured blocks.
// They all have their size as the last field, which is
// probably unnecessary for us.  I've made them into
// classes with all public memebers, and some functions.

// Text Block
//
// This has been entirely modified from the original
// for use in the modern system.
class TXB
{
public:
	TXB() {}

	void SetVals(char * a, int c, int l, int t)
	{
		area = a;
		caret = c;
		len = l;
		top = t;
	}

	// Fields
	char *	area;
	int		caret;
	int		len;
	int		top;
};

// Attack Block
// Creatures and players use the same algorithm
// for attacking and for damage infliction.  These
// values are the common ones used.
class ATB
{
public:
	// Fields
	dodSHORT	P_ATPOW;
	dodBYTE		P_ATMGO;
	dodBYTE		P_ATMGD;
	dodBYTE		P_ATPHO;
	dodBYTE		P_ATPHD;
	dodSHORT	P_ATXX1;
	dodSHORT	P_ATXX2;
	dodSHORT	P_ATDAM;
};

// Creature control block
// Holds all the data for a particular creature.
class CCB
{
public:
	// Constructor
	CCB()
	{ clear(); }

	void clear()
	{
	P_CCPOW = 0;
	P_CCMGO = 0;
	P_CCMGD = 0;
	P_CCPHO = 0;
	P_CCPHD = 0;
	P_CCTMV = 0;
	P_CCTAT = 0;
	P_CCOBJ = -1;
	P_CCDAM = 0;
	P_CCUSE = 0;
	creature_id = 0;
	P_CCDIR = 0;
	P_CCROW = 0;
	P_CCCOL = 0;
	}

	// Fields
	dodSHORT	P_CCPOW;
	dodBYTE		P_CCMGO;
	dodBYTE		P_CCMGD;
	dodBYTE		P_CCPHO;
	dodBYTE		P_CCPHD;
	int			P_CCTMV;
	int			P_CCTAT;
	int			P_CCOBJ;
	dodSHORT	P_CCDAM;
	dodBYTE		P_CCUSE;
	dodBYTE		creature_id;
	dodBYTE		P_CCDIR;
	dodBYTE		P_CCROW;
	dodBYTE		P_CCCOL;
};

// Creature definition block
// Holds the data for a creature type.
class CDB
{
public:
	// Constructors
	CDB(dodSHORT pow, dodBYTE mgo, dodBYTE mgd,
		dodBYTE pho, dodBYTE phd, int tmv,
		int tat)
		: P_CDPOW(pow), P_CDMGO(mgo), P_CDMGD(mgd),
		  P_CDPHO(pho), P_CDPHD(phd), P_CDTMV(tmv),
		  P_CDTAT(tat)
	{}

	CDB()
		: P_CDPOW(0), P_CDMGO(0), P_CDMGD(0),
		  P_CDPHO(0), P_CDPHD(0), P_CDTMV(0),
		  P_CDTAT(0)
	{}

	// Fields
	dodSHORT	P_CDPOW;
	dodBYTE		P_CDMGO;
	dodBYTE		P_CDMGD;
	dodBYTE		P_CDPHO;
	dodBYTE		P_CDPHD;
	int			P_CDTMV;
	int			P_CDTAT;
};

// Object control block
// Hold the data for a particular object.
class OCB
{
public:
	// Default constructor
	OCB()
		{ clear(); }

	void clear()
	{
		P_OCPTR = -1;
		P_OCROW = 0;
		P_OCCOL = 0;
		P_OCLVL = 0;
		P_OCOWN = 0;
		P_OCXX0 = 0;
		P_OCXX1 = 0;
		P_OCXX2 = 0;
		obj_id = 0;
		obj_type = 0;
		obj_reveal_lvl = 0;
		P_OCMGO = 0;
		P_OCPHO = 0;
	}

	// Fields
	int			P_OCPTR;
	dodBYTE		P_OCROW;
	dodBYTE		P_OCCOL;
	dodBYTE		P_OCLVL;
	dodBYTE		P_OCOWN;
	dodSHORT	P_OCXX0;
	dodSHORT	P_OCXX1;
	dodSHORT	P_OCXX2;
	dodBYTE		obj_id;
	dodBYTE		obj_type;
	dodBYTE		obj_reveal_lvl;
	dodBYTE		P_OCMGO;
	dodBYTE		P_OCPHO;
};

// Object definition block
// Used for constructing specific objects.
class ODB
{
public:
	// Constructors
	ODB(dodBYTE cls, dodBYTE rev, dodBYTE mgo,
		dodBYTE pho)
		: P_ODCLS(cls), P_ODREV(rev), P_ODMGO(mgo),
		  P_ODPHO(pho)
	{}

	ODB()
		: P_ODCLS(0), P_ODREV(0), P_ODMGO(0),
		  P_ODPHO(0)
	{}

	// Fields
	dodBYTE		P_ODCLS;
	dodBYTE		P_ODREV;
	dodBYTE		P_ODMGO;
	dodBYTE		P_ODPHO;
};

// Extra definition block
// Holds extra data for torches, rings, and shields:
// torch timers, ring shot counters and incantation indices,
// and shield magical and physical defense values.
class XDB
{
public:
	// Constructors
	XDB(int idx, dodSHORT x0, dodSHORT x1,
		dodSHORT x2)
		: P_OXIDX(idx), P_OXXX0(x0), P_OXXX1(x1),
		  P_OXXX2(x2)
	{}

	XDB()
		: P_OXIDX(-1), P_OXXX0(0), P_OXXX1(0),
		  P_OXXX2(0)
	{}

	// Fields
	int			P_OXIDX;
	dodSHORT	P_OXXX0;
	dodSHORT	P_OXXX1;
	dodSHORT	P_OXXX2;
};

// The new Task class for use in the rewritten
// Scheduler algorithm.  Not all fields are being
// used currently.  They may go away, if the current
// algorthim tests well, otherwise, they may be used
// for increasing the accuracy of the scheduler.
struct Task
{
public:
	int		type;		// One of the seven task types
	int		data;		// Stores creatures ID
	Uint32	frequency;	// in milliseconds
	Uint32	prev_time;	// previous execution timestamp
	Uint32	next_time;	// next scheduled execution timestamp
	long	count;		// number of times executed

	Task()
		{ clear(); }

	// Convenience Setter
	void setValues(int t, int d, long f, long p, long n, long c, bool e)
	{
		type = t;
		data = d;
		frequency = f;
		prev_time = p;
		next_time = n;
		count = c;
	}

	void clear()
	{
		type = -1;
		data = -1;
		frequency = 0;
		prev_time = 0;
		next_time = 0;
		count = 0;
	}
};

// This class is a port of Daggorath's custom RNG
class RNG
{
public:
	// Constructor
	RNG()
	{
		carry = 0;
		SEED[0] = 0;
		SEED[1] = 0;
		SEED[2] = 0;
	}

	// Accessors
	dodBYTE RANDOM()
	{
		int x, y;
		dodBYTE a, b;
		carry = 0;
		for (x = 8; x != 0; --x)
		{
			b = 0;
			a = (SEED[2] & 0xE1);
			for (y = 8; y != 0; --y)
			{
				a = lsl(a);
				if (carry != 0)
					++b;
			}
			b = lsr(b);
			SEED[0] = rol(SEED[0]);
			SEED[1] = rol(SEED[1]);
			SEED[2] = rol(SEED[2]);
		}
		return SEED[0];
	}

	dodBYTE getSEED(int idx)
	{
		return SEED[idx];
	}

	// Mutators
	void setSEED(int idx, dodBYTE val)
	{
		SEED[idx] = val;
	}

	void setSEED(dodBYTE val0, dodBYTE val1, dodBYTE val2)
	{
		SEED[0] = val0;
		SEED[1] = val1;
		SEED[2] = val2;
	}

	dodBYTE SEED[3];
	dodBYTE carry;

private:
	// Internal Implementation
	dodBYTE lsl(dodBYTE c)
	{
		carry = (((c & 128) == 128) ? 1 : 0);
		return c << 1;
	}

	dodBYTE lsr(dodBYTE c)
	{
		carry = (((c & 1) == 1) ? 1 : 0);
		return c >> 1;
	}

	dodBYTE rol(dodBYTE c)
	{
		dodBYTE cry;
		cry = (((c & 128) == 128) ? 1 : 0);
		c <<= 1;
		c += carry;
		carry = cry;
		return c;
	}
};

// Not much here at the moment, but it could grow
class Utils
{
public:
	// This could be made into a template someday
	static void LoadFromHex(dodBYTE * b, char * h)
	{
		char hexbuf[3];
		char * end;
		hexbuf[2] = 0;
		int ctr = 0;

		while (*h)
		{
			hexbuf[0] = *h;
			hexbuf[1] = *(h + 1);
			*(b + ctr) = (dodBYTE) strtoul(hexbuf, &end, 16);
			++ctr;
			h += 2;
		}
	}

	// This could be made into a template someday
	static void LoadFromHex(int * b, char * h)
	{
		char hexbuf[3];
		char * end;
		hexbuf[2] = 0;
		int ctr = 0;

		while (*h)
		{
			hexbuf[0] = *h;
			hexbuf[1] = *(h + 1);
			*(b + ctr) = (int) strtoul(hexbuf, &end, 16);
			++ctr;
			h += 2;
		}
	}

	static void LoadFromDecDigit(dodBYTE * b, char * dd)
	{
		while (*dd)
		{
			*b++ = (*dd++ - '0');
		}
	}

	static void LoadFromDecDigit(int * b, char * dd)
	{
		while (*dd)
		{
			*b++ = (*dd++ - '0');
		}
	}

	static Mix_Chunk *LoadSound(char *snd);
};

// Command creator menu class

#define NUM_CMD_MENU  	14
#define NUM_ACTION		15
#define NUM_HAND		2
#define NUM_MOVE		4
#define NUM_TURN		3
#define NUM_CLIMB		2
#define NUM_OBJECTS		6
#define NUM_INCANT		28

#define NUM_TORCH_TYPES		4
#define NUM_SWORD_TYPES		3
#define NUM_SHIELD_TYPES	3
#define NUM_FLASK_TYPES		3
#define NUM_SCROLL_TYPES	2

#define NUM_CMD_LENGTH 	35

#define ACTION_MENU_SWITCH			0
#define HAND_MENU_SWITCH			1
#define MOVE_MENU_SWITCH			2
#define TURN_MENU_SWITCH			3
#define CLIMB_MENU_SWITCH			4
#define OBJECTS_MENU_SWITCH			5
#define HAND_NO_OB_MENU_SWITCH		6
#define INCANT_MENU_SWITCH			7
#define TORCH_TYPES_MENU_SWITCH		8
#define SWORD_TYPES_MENU_SWITCH		9
#define SHIELD_TYPES_MENU_SWITCH	10
#define FLASK_TYPES_MENU_SWITCH		11
#define SCROLL_TYPES_MENU_SWITCH	12
#define OBJECT_TYPES_MENU_SWITCH	13

#define ACTION_MENU_GET		0
#define ACTION_MENU_PULL	1
#define ACTION_MENU_CLIMB	2
#define ACTION_MENU_EXAMINE	3
#define ACTION_MENU_LOOK	4
#define ACTION_MENU_DROP	5
#define ACTION_MENU_STOW	6
#define ACTION_MENU_USE		7
#define ACTION_MENU_REVEAL	8
#define ACTION_MENU_INCANT	9
#define ACTION_MENU_MOVE	10
#define ACTION_MENU_TURN	11
#define ACTION_MENU_ATTACK	12
#define ACTION_MENU_ZSAVE	13
#define ACTION_MENU_ZLOAD	14

#define HAND_MENU_LEFT		0
#define HAND_MENU_RIGHT		1

#define MOVE_MENU_FORWARD	0
#define MOVE_MENU_LEFT		1
#define MOVE_MENU_RIGHT		2
#define MOVE_MENU_BACKWARD	3

#define TURN_MENU_LEFT		0
#define TURN_MENU_RIGHT		1
#define TURN_MENU_AROUND	2

#define CLIMB_MENU_DOWN		0
#define CLIMB_MENU_UP		1

#define OBJECTS_MENU_TORCH	0
#define OBJECTS_MENU_SWORD	1
#define OBJECTS_MENU_SHIELD	2
#define OBJECTS_MENU_FLASK	3
#define OBJECTS_MENU_SCROLL	4
#define OBJECTS_MENU_RING	5

#define TORCH_TYPES_MENU_PINE	0
#define TORCH_TYPES_MENU_LUNAR	1
#define TORCH_TYPES_MENU_SOLAR	2
#define TORCH_TYPES_MENU_DEAD	3

#define SWORD_TYPES_MENU_WOODEN	0
#define SWORD_TYPES_MENU_IRON	1
#define SWORD_TYPES_MENU_ELVISH	2

#define SHIELD_TYPES_MENU_LEATHER	0
#define SHIELD_TYPES_MENU_BRONZE	1
#define SHIELD_TYPES_MENU_MITHRIL	2

#define FLASK_TYPES_MENU_HALE	0
#define FLASK_TYPES_MENU_ABYE	1
#define FLASK_TYPES_MENU_THEWS	2

#define SCROLL_TYPES_MENU_VISION	0
#define SCROLL_TYPES_MENU_SEER		1

#define INCANT_MENU_END		0
#define INCANT_MENU_A		1
#define INCANT_MENU_B		2
#define INCANT_MENU_C		3
#define INCANT_MENU_D		4
#define INCANT_MENU_E		5
#define INCANT_MENU_F		6
#define INCANT_MENU_G		7
#define INCANT_MENU_H		8
#define INCANT_MENU_I		9
#define INCANT_MENU_J		10
#define INCANT_MENU_K		11
#define INCANT_MENU_L		12
#define INCANT_MENU_M		13
#define INCANT_MENU_N		14
#define INCANT_MENU_O		15
#define INCANT_MENU_P		16
#define INCANT_MENU_Q		17
#define INCANT_MENU_R		18
#define INCANT_MENU_S		19
#define INCANT_MENU_T		20
#define INCANT_MENU_U		21
#define INCANT_MENU_V		22
#define INCANT_MENU_W		23
#define INCANT_MENU_X		24
#define INCANT_MENU_Y		25
#define INCANT_MENU_Z		26
#define INCANT_MENU_BACK	27

// Possibly have types of items?

class command_menu
{
private:
	char MENU_NAME[NUM_CMD_MENU][NUM_CMD_LENGTH];
	int MENU_SIZE[NUM_CMD_MENU];
	char ACTION_MENU[NUM_ACTION][NUM_CMD_LENGTH];
	char HAND_MENU[NUM_HAND][NUM_CMD_LENGTH];
	char MOVE_MENU[NUM_MOVE][NUM_CMD_LENGTH];
	char TURN_MENU[NUM_TURN][NUM_CMD_LENGTH];
	char CLIMB_MENU[NUM_CLIMB][NUM_CMD_LENGTH];
	char OBJECTS_MENU[NUM_OBJECTS][NUM_CMD_LENGTH];
	char INCANT_MENU[NUM_INCANT][NUM_CMD_LENGTH];
	char TORCH_TYPES_MENU[NUM_TORCH_TYPES][NUM_CMD_LENGTH];
	char SWORD_TYPES_MENU[NUM_SWORD_TYPES][NUM_CMD_LENGTH];
	char SHIELD_TYPES_MENU[NUM_SHIELD_TYPES][NUM_CMD_LENGTH];
	char FLASK_TYPES_MENU[NUM_FLASK_TYPES][NUM_CMD_LENGTH];
	char SCROLL_TYPES_MENU[NUM_SCROLL_TYPES][NUM_CMD_LENGTH];

public:

	 // Constructor -- initalizes strings
	command_menu()
	 {
	 strncpy(&MENU_NAME[ACTION_MENU_SWITCH][0], "COMMAND CREATOR - ACTION", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[MOVE_MENU_SWITCH][0], "COMMAND CREATOR - MOVE", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[HAND_MENU_SWITCH][0], "COMMAND CREATOR - HAND", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[TURN_MENU_SWITCH][0], "COMMAND CREATOR - TURN", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[CLIMB_MENU_SWITCH][0], "COMMAND CREATOR - CLIMB", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[OBJECTS_MENU_SWITCH][0], "COMMAND CREATOR - OBJECTS", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[HAND_NO_OB_MENU_SWITCH][0], "COMMAND CREATOR - HAND", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[INCANT_MENU_SWITCH][0], "COMMAND CREATOR - INCANT", NUM_CMD_LENGTH);

	 strncpy(&MENU_NAME[TORCH_TYPES_MENU_SWITCH][0], "COMMAND CREATOR - TORCH TYPES", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[SWORD_TYPES_MENU_SWITCH][0], "COMMAND CREATOR - SWORD TYPES", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[SHIELD_TYPES_MENU_SWITCH][0], "COMMAND CREATOR - SHIELD TYPES", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[FLASK_TYPES_MENU_SWITCH][0], "COMMAND CREATOR - FLASK TYPES", NUM_CMD_LENGTH);
	 strncpy(&MENU_NAME[SCROLL_TYPES_MENU_SWITCH][0], "COMMAND CREATOR - SCROLL TYPES", NUM_CMD_LENGTH);

	 MENU_SIZE[0] = NUM_ACTION;
	 MENU_SIZE[1] = NUM_HAND;
	 MENU_SIZE[2] = NUM_MOVE;
	 MENU_SIZE[3] = NUM_TURN;
	 MENU_SIZE[4] = NUM_CLIMB;
	 MENU_SIZE[5] = NUM_OBJECTS;
	 MENU_SIZE[6] = NUM_HAND;
	 MENU_SIZE[7] = NUM_INCANT;
	 MENU_SIZE[8] = NUM_TORCH_TYPES;
	 MENU_SIZE[9] = NUM_SWORD_TYPES;
	 MENU_SIZE[10] = NUM_SHIELD_TYPES;
	 MENU_SIZE[11] = NUM_FLASK_TYPES;
	 MENU_SIZE[12] = NUM_SCROLL_TYPES;

	 strncpy(&ACTION_MENU[ACTION_MENU_GET][0], "GET", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_PULL][0], "PULL", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_CLIMB][0], "CLIMB", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_EXAMINE][0], "EXAMINE", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_LOOK][0], "LOOK", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_DROP][0], "DROP", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_STOW][0], "STOW", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_USE][0], "USE", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_REVEAL][0], "REVEAL", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_INCANT][0], "INCANT", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_MOVE][0], "MOVE", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_TURN][0], "TURN", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_ATTACK][0], "ATTACK", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_ZSAVE][0], "ZSAVE", NUM_CMD_LENGTH);
	 strncpy(&ACTION_MENU[ACTION_MENU_ZLOAD][0], "ZLOAD", NUM_CMD_LENGTH);

	 strncpy(&HAND_MENU[HAND_MENU_LEFT][0], "LEFT", NUM_CMD_LENGTH);
	 strncpy(&HAND_MENU[HAND_MENU_RIGHT][0], "RIGHT", NUM_CMD_LENGTH);

	 strncpy(&MOVE_MENU[MOVE_MENU_FORWARD][0], "FORWARD", NUM_CMD_LENGTH);
	 strncpy(&MOVE_MENU[MOVE_MENU_LEFT][0], "LEFT", NUM_CMD_LENGTH);
	 strncpy(&MOVE_MENU[MOVE_MENU_RIGHT][0], "RIGHT", NUM_CMD_LENGTH);
	 strncpy(&MOVE_MENU[MOVE_MENU_BACKWARD][0], "BACKWARD", NUM_CMD_LENGTH);

	 strncpy(&TURN_MENU[TURN_MENU_LEFT][0], "LEFT", NUM_CMD_LENGTH);
	 strncpy(&TURN_MENU[TURN_MENU_RIGHT][0], "RIGHT", NUM_CMD_LENGTH);
	 strncpy(&TURN_MENU[TURN_MENU_AROUND][0], "AROUND", NUM_CMD_LENGTH);

	 strncpy(&CLIMB_MENU[CLIMB_MENU_DOWN][0], "DOWN", NUM_CMD_LENGTH);
	 strncpy(&CLIMB_MENU[CLIMB_MENU_UP][0], "UP", NUM_CMD_LENGTH);

	 strncpy(&OBJECTS_MENU[OBJECTS_MENU_TORCH][0], "TORCH", NUM_CMD_LENGTH);
	 strncpy(&OBJECTS_MENU[OBJECTS_MENU_SWORD][0], "SWORD", NUM_CMD_LENGTH);
	 strncpy(&OBJECTS_MENU[OBJECTS_MENU_SHIELD][0], "SHIELD", NUM_CMD_LENGTH);
	 strncpy(&OBJECTS_MENU[OBJECTS_MENU_FLASK][0], "FLASK", NUM_CMD_LENGTH);
	 strncpy(&OBJECTS_MENU[OBJECTS_MENU_SCROLL][0], "SCROLL", NUM_CMD_LENGTH);
	 strncpy(&OBJECTS_MENU[OBJECTS_MENU_RING][0], "RING", NUM_CMD_LENGTH);

	 strncpy(&INCANT_MENU[INCANT_MENU_END][0], "END", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_A][0], "A", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_B][0], "B", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_C][0], "C", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_D][0], "D", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_E][0], "E", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_F][0], "F", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_G][0], "G", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_H][0], "H", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_I][0], "I", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_J][0], "J", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_K][0], "K", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_L][0], "L", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_M][0], "M", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_N][0], "N", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_O][0], "O", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_P][0], "P", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_Q][0], "Q", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_R][0], "R", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_S][0], "S", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_T][0], "T", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_U][0], "U", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_V][0], "V", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_W][0], "W", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_X][0], "X", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_Y][0], "Y", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_Z][0], "Z", NUM_CMD_LENGTH);
	 strncpy(&INCANT_MENU[INCANT_MENU_BACK][0], "BACK", NUM_CMD_LENGTH);

	 strncpy(&TORCH_TYPES_MENU[TORCH_TYPES_MENU_PINE][0], "PINE TORCH", NUM_CMD_LENGTH);
	 strncpy(&TORCH_TYPES_MENU[TORCH_TYPES_MENU_LUNAR][0], "LUNAR TORCH", NUM_CMD_LENGTH);
	 strncpy(&TORCH_TYPES_MENU[TORCH_TYPES_MENU_SOLAR][0], "SOLAR TORCH", NUM_CMD_LENGTH);
	 strncpy(&TORCH_TYPES_MENU[TORCH_TYPES_MENU_DEAD][0], "DEAD TORCH", NUM_CMD_LENGTH);

	 strncpy(&SWORD_TYPES_MENU[SWORD_TYPES_MENU_WOODEN][0], "WOODEN SWORD", NUM_CMD_LENGTH);
	 strncpy(&SWORD_TYPES_MENU[SWORD_TYPES_MENU_IRON][0], "IRON SWORD", NUM_CMD_LENGTH);
	 strncpy(&SWORD_TYPES_MENU[SWORD_TYPES_MENU_ELVISH][0], "ELVISH SWORD", NUM_CMD_LENGTH);

	 strncpy(&SHIELD_TYPES_MENU[SHIELD_TYPES_MENU_LEATHER][0], "LEATHER SHIELD", NUM_CMD_LENGTH);
	 strncpy(&SHIELD_TYPES_MENU[SHIELD_TYPES_MENU_BRONZE][0], "BRONZE SHIELD", NUM_CMD_LENGTH);
	 strncpy(&SHIELD_TYPES_MENU[SHIELD_TYPES_MENU_MITHRIL][0], "MITHRIL SHIELD", NUM_CMD_LENGTH);

	 strncpy(&FLASK_TYPES_MENU[FLASK_TYPES_MENU_HALE][0], "HALE FLASK", NUM_CMD_LENGTH);
	 strncpy(&FLASK_TYPES_MENU[FLASK_TYPES_MENU_ABYE][0], "ABYE FLASK", NUM_CMD_LENGTH);
	 strncpy(&FLASK_TYPES_MENU[FLASK_TYPES_MENU_THEWS][0], "THEWS FLASK", NUM_CMD_LENGTH);

	 strncpy(&SCROLL_TYPES_MENU[SCROLL_TYPES_MENU_VISION][0], "VISION SCROLL", NUM_CMD_LENGTH);
	 strncpy(&SCROLL_TYPES_MENU[SCROLL_TYPES_MENU_SEER][0], "SEER SCROLL", NUM_CMD_LENGTH);
	 }

	 // Retrieves the menu name based on an id
	char *getMenuName(int menu_id)
	 {
	 return &MENU_NAME[menu_id][0];
	 }

	 // Retrieves the menu item specified
	char *getMenuItem(int menu_id, int item)
	 {
	 switch(menu_id)
	 {
	 case ACTION_MENU_SWITCH:
	 	return &ACTION_MENU[item][0];
	   	break;
     case HAND_MENU_SWITCH:
	   	return &HAND_MENU[item][0];
	   	break;
     case MOVE_MENU_SWITCH:
	   	return &MOVE_MENU[item][0];
	   	break;
	 case TURN_MENU_SWITCH:
	   	return &TURN_MENU[item][0];
	   	break;
	 case CLIMB_MENU_SWITCH:
	   	return &CLIMB_MENU[item][0];
	   	break;
	 case OBJECTS_MENU_SWITCH:
	   	return &OBJECTS_MENU[item][0];
	   	break;
	 case HAND_NO_OB_MENU_SWITCH:
	   	return &HAND_MENU[item][0];
	   	break;
	 case OBJECT_TYPES_MENU_SWITCH:
	 	return &OBJECTS_MENU[item][0];
	 	break;
	 case INCANT_MENU_SWITCH:
	   	return &INCANT_MENU[item][0];
	   	break;
	 case TORCH_TYPES_MENU_SWITCH:
	 	return &TORCH_TYPES_MENU[item][0];
	 	break;
	case SWORD_TYPES_MENU_SWITCH:
		return &SWORD_TYPES_MENU[item][0];
	 	break;
	 case SHIELD_TYPES_MENU_SWITCH:
	 	return &SHIELD_TYPES_MENU[item][0];
	 	break;
	 case FLASK_TYPES_MENU_SWITCH:
	 	return &FLASK_TYPES_MENU[item][0];
	 	break;
	 case SCROLL_TYPES_MENU_SWITCH:
	 	return &SCROLL_TYPES_MENU[item][0];
	 	break;
          }
	 return NULL;
	 }

	 // Returns the size of the specified menu
	int getMenuSize(int menu_id)
	 {
	 return MENU_SIZE[menu_id];
	 }
} ;


/*******************************************************************
  Class: menu

  Function: implements the menus

  Note: implementation of commands is at OS_Link::menu_return
*******************************************************************/
#define NUM_MENU   	3
#define NUM_FILE   	6
#define NUM_CONFIG 	12
#define NUM_HELP   	3
#define NUM_LENGTH 	35

#define FILE_MENU_SWITCH   0
#define CONFIG_MENU_SWITCH 1
#define HELP_MENU_SWITCH   2

#define FILE_MENU_NEW    	0
#define FILE_MENU_SAVE	 	1
#define FILE_MENU_LOAD		2
#define FILE_MENU_RETURN 	3
#define FILE_MENU_ABORT  	4
#define FILE_MENU_EXIT   	5

#define CONFIG_MENU_FULL_SCREEN    0
#define CONFIG_MENU_VIDEO_RES      1
#define CONFIG_MENU_GRAPHICS       2
#define CONFIG_MENU_COLOR          3
#define CONFIG_MENU_VOLUME         4
#define CONFIG_MENU_SAVEDIR        5
#define CONFIG_MENU_CREATURE_SPEED 6
#define CONFIG_MENU_REGEN_SPEED    7
#define CONFIG_MENU_RANDOM_MAZE    8
#define CONFIG_MENU_SND_MODE       9
#define CONFIG_MENU_SAVE_OPT      10
#define CONFIG_MENU_DEFAULTS      11

#define HELP_MENU_HOWTOPLAY 0
#define HELP_MENU_LICENSE   1
#define HELP_MENU_ABOUT     2

class menu
{
private:
	char MENU_NAME[NUM_MENU][NUM_LENGTH];
	int MENU_SIZE[NUM_MENU];
	char FILE_MENU[NUM_FILE][NUM_LENGTH];
	char CONFIG[NUM_CONFIG][NUM_LENGTH];
	char HELP[NUM_HELP][NUM_LENGTH];

public:

	 // Constructor -- initalizes strings
	menu()
	 {
	 strncpy(&MENU_NAME[FILE_MENU_SWITCH][0], "FILE", NUM_LENGTH);
	 strncpy(&MENU_NAME[CONFIG_MENU_SWITCH][0], "CONFIGURE", NUM_LENGTH);
	 strncpy(&MENU_NAME[HELP_MENU_SWITCH][0], "HELP", NUM_LENGTH);

	 MENU_SIZE[0] = NUM_FILE;
	 MENU_SIZE[1] = NUM_CONFIG;
	 MENU_SIZE[2] = NUM_HELP;

	 strncpy(&FILE_MENU[FILE_MENU_NEW][0], "START NEW GAME", NUM_LENGTH);
	 strncpy(&FILE_MENU[FILE_MENU_SAVE][0], "SAVE GAME", NUM_LENGTH);
	 strncpy(&FILE_MENU[FILE_MENU_LOAD][0], "LOAD GAME", NUM_LENGTH);
	 strncpy(&FILE_MENU[FILE_MENU_RETURN][0], "RETURN TO GAME", NUM_LENGTH);
	 strncpy(&FILE_MENU[FILE_MENU_ABORT][0], "ABORT GAME", NUM_LENGTH);
	 strncpy(&FILE_MENU[FILE_MENU_EXIT][0], "EXIT", NUM_LENGTH);

	 strncpy(&CONFIG[CONFIG_MENU_FULL_SCREEN][0], "FULL SCREEN", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_VIDEO_RES][0], "VIDEO MODE", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_GRAPHICS][0], "GRAPHICS MODE", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_COLOR][0], "COLOR MODE", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_VOLUME][0], "SOUND VOLUME", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_SAVEDIR][0], "SAVE DIRECTORY", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_CREATURE_SPEED][0], "CREATURE SPEED", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_REGEN_SPEED][0], "REGEN SPEED", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_RANDOM_MAZE][0], "RANDOM MAZES", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_SND_MODE][0], "SOUND MODES", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_SAVE_OPT][0], "SAVE CURRENT OPTIONS", NUM_LENGTH);
	 strncpy(&CONFIG[CONFIG_MENU_DEFAULTS][0], "RESTORE DEFAULTS", NUM_LENGTH);

	 strncpy(&HELP[HELP_MENU_HOWTOPLAY][0], "HOW TO PLAY", NUM_LENGTH);
	 strncpy(&HELP[HELP_MENU_LICENSE][0], "LICENSE", NUM_LENGTH);
	 strncpy(&HELP[HELP_MENU_ABOUT][0], "ABOUT DOD", NUM_LENGTH);
	 }

	 // Retrieves the menu name based on an id
	char *getMenuName(int menu_id)
	 {
	 return &MENU_NAME[menu_id][0];
	 }

	 // Retrieves the menu item specified
	char *getMenuItem(int menu_id, int item)
	 {
	 switch(menu_id)
	  {
	  case FILE_MENU_SWITCH:
	   return &FILE_MENU[item][0];
	   break;
          case CONFIG_MENU_SWITCH:
	   return &CONFIG[item][0];
	   break;
          case HELP_MENU_SWITCH:
	   return &HELP[item][0];
	   break;
          }
	 return NULL;
	 }

	 // Returns the size of the specified menu
	int getMenuSize(int menu_id)
	 {
	 return MENU_SIZE[menu_id];
	 }
} ;

#endif // DOD_COMMON_HEADER
