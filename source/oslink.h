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
// Filename: oslink.h
//
// This class manages the SDL operations, which abstract
// the link to the operating system.  By keeping these
// separate, it will be somewhat easier to change to a
// different library if necessary.

#ifndef OS_LINK_HEADER
#define OS_LINK_HEADER

#include <SDL.h>
#include <SDL_opengl.h>

#include "dod.h"


 // Arbitrary Length of 80, maybe be changed if needed
#define MAX_FILENAME_LENGTH 80




class OS_Link
{
public:
	// Constructor
	OS_Link();

	// Public Interface
	void init();			// main entry point for dod application
	void quitSDL(int code);	// shuts down SDL before exiting
	void process_events();	// used mainly to retrieve keystrokes
	bool main_menu();       // used to implement the meta-menu
	bool saveOptFile(void);

	// Public Data Fields
	int		width;	// actual screen width after video setup
	int		height;	// same for height
	int     volumeLevel; // Volume level

	char	gamefile[50];
	char	pathSep[2];
	FILE *	fptr;
	char	confDir[5];
	char	soundDir[6];
	char	savedDir[MAX_FILENAME_LENGTH + 1];
	dodBYTE	keys[256];

	int		audio_rate;
	Uint16	audio_format;
	int		audio_channels;
	int		audio_buffers;
	int		gamefileLen;
	int		keylayout;	// 0 = QWERTY, 1 = Dvorak
	int		keyLen;
	SDL_Joystick *joystick;
	FILE* outputFile;
	char commandCreatorBuffer[256];

private:
	// Internal Implementation
	void handle_key_down(SDL_keysym * keysym);	// keyboard handler
	void handle_joybutton_down(int button); //joystick handler
	void type_command(char* command);
	bool menu_return(int, int, menu);		// Used by main menu
	int  menu_list(int x, int y, char *title, char *list[], int listSize);
	void menu_string(char *newString, char *title, int maxLength);
	int  menu_scrollbar(char *title, int min, int max, int current);
	void loadOptFile(void);
	void loadDefaults(void);
	void changeFullScreen(void);
	void changeVideoRes(int mode);
	bool command_menu_return(int *menu_id, int *item, int *prev_menu_id, int *prev_item, command_menu commandMenu);

	// Data Fields
	int  bpp;	    // bits per pixel
	int  flags;	    // SDL flags
	bool FullScreen;    // FullScreen
	int  creatureRegen; // Creature Regen Speed
	int  videoMode;
};

#endif // OS_LINK_HEADER
