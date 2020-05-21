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
// Filename: oslink.cpp
//
// Implementation of OS_Link class

#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

#include "oslink.h"
#include "dodgame.h"
#include "viewer.h"
#include "sched.h"
#include "player.h"
#include "dungeon.h"
#include "parser.h"
#include "object.h"
#include "creature.h"
#include "enhanced.h"

#ifdef _3DS
#include "3ds/manual.h"
#include "3ds/ctr_utils.h"
#endif

extern Creature		creature;
extern Object		object;
extern Dungeon		dungeon;
extern Player		player;
extern Coordinate	crd;
extern Viewer		viewer;
extern dodGame		game;
extern Scheduler	scheduler;
extern Parser		parser;

extern void quitGame();

// Constructor
OS_Link::OS_Link() : width(0), height(0),
					 audio_rate(44100), audio_format(AUDIO_S16),
					 audio_channels(2), audio_buffers(512),
					 gamefileLen(50), keylayout(0), keyLen(256),
					  bpp(0), flags(0)

{
#define MACOSX
#ifdef MACOSX
	strcpy(pathSep,"/");
#else
	//strcpy(pathSep,"\\");
strcpy(pathSep,"/");
#endif

#ifdef _3DS
	strcpy(confDir, "/3ds/3dsdod/conf");
#else
	strcpy(confDir, "conf");
#endif

#ifdef _3DS
	strcpy(soundDir, "romfs:/sound");
#else
	strcpy(soundDir, "sound");
#endif

#ifdef _3DS
	strcpy(savedDir, "/3ds/3dsdod/saved");
#else
	strcpy(savedDir, "saved");
#endif
	memset(gamefile,0,gamefileLen);
}

// This routine will eventually need updated to allow
// user customization of screen size and resolution.
// It currently asks for an 1024x768 screen size.
// Updated - Now defaults to whatever is in the opts.ini file
// if opts.ini doesn't exist or has invalid or missing values
// uses defaults set by loadDefaults function (1024x768)
void OS_Link::init()
{
	outputFile = fopen("test.txt", "w");
	fprintf(outputFile, "Initializing\n");
	loadOptFile();

	viewer.initialize();

#ifdef _3DS
	// Create the directories for saving and loading if they don't exist.
	mkdir("/3ds", 0755);
	mkdir("/3ds/3dsdod", 0755);
	mkdir(confDir, 0755);
	mkdir(savedDir, 0755);
#endif

	Uint32 ticks1, ticks2;

#ifdef _3DS
	// We use Citro for rendering instead of SDL.
	if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
	{
		printf( "Video initialization failed: %s\n", SDL_GetError());
		sleep(1);
		quitGame();
	}
#else
	const SDL_VideoInfo * info = NULL;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
	{
		printf( "Video initialization failed: %s\n", SDL_GetError());
		sleep(1);
		quitGame();
	}
#endif

	//Initialize joystick

	SDL_JoystickEventState(SDL_ENABLE);
	joystick = SDL_JoystickOpen(0);
	if(joystick == NULL)
	{
		printf("Failed to initialize joystick\n");
		return;
	}

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
	{
		printf("Unable to open audio: %s!\n", Mix_GetError());
		sleep(1);
		quitGame();
	}

	creature.LoadSounds();
	object.LoadSounds();
	scheduler.LoadSounds();
	player.LoadSounds();

	Mix_AllocateChannels(4);
	Mix_Volume(-1, MIX_MAX_VOLUME);

#ifndef _3DS
	info = SDL_GetVideoInfo();
	if(!info)
	{
		printf("Video query failed: %s\n", SDL_GetError());
		sleep(1);
		quitGame();
	}
	bpp = info->vfmt->BitsPerPixel;
#endif

	changeVideoRes(videoMode); // All changing video res code was moved here

	viewer.Reset();

	memset(keys, parser.C_SP, keyLen);

	if (keylayout == 0) // QWERTY
	{
		keys[SDLK_a] = 'A';
		keys[SDLK_b] = 'B';
		keys[SDLK_c] = 'C';
		keys[SDLK_d] = 'D';
		keys[SDLK_e] = 'E';
		keys[SDLK_f] = 'F';
		keys[SDLK_g] = 'G';
		keys[SDLK_h] = 'H';
		keys[SDLK_i] = 'I';
		keys[SDLK_j] = 'J';
		keys[SDLK_k] = 'K';
		keys[SDLK_l] = 'L';
		keys[SDLK_m] = 'M';
		keys[SDLK_n] = 'N';
		keys[SDLK_o] = 'O';
		keys[SDLK_p] = 'P';
		keys[SDLK_q] = 'Q';
		keys[SDLK_r] = 'R';
		keys[SDLK_s] = 'S';
		keys[SDLK_t] = 'T';
		keys[SDLK_u] = 'U';
		keys[SDLK_v] = 'V';
		keys[SDLK_w] = 'W';
		keys[SDLK_x] = 'X';
		keys[SDLK_y] = 'Y';
		keys[SDLK_z] = 'Z';
		keys[SDLK_BACKSPACE] = parser.C_BS;
		keys[SDLK_RETURN] = parser.C_CR;
		keys[SDLK_SPACE] = parser.C_SP;
	}
	else if (keylayout == 1) // Dvorak
	{
		keys[SDLK_a] = 'A';
		keys[SDLK_n] = 'B';
		keys[SDLK_i] = 'C';
		keys[SDLK_h] = 'D';
		keys[SDLK_d] = 'E';
		keys[SDLK_y] = 'F';
		keys[SDLK_u] = 'G';
		keys[SDLK_j] = 'H';
		keys[SDLK_g] = 'I';
		keys[SDLK_c] = 'J';
		keys[SDLK_v] = 'K';
		keys[SDLK_p] = 'L';
		keys[SDLK_m] = 'M';
		keys[SDLK_l] = 'N';
		keys[SDLK_s] = 'O';
		keys[SDLK_r] = 'P';
		keys[SDLK_x] = 'Q';
		keys[SDLK_o] = 'R';
		keys[SDLK_SEMICOLON] = 'S';
		keys[SDLK_k] = 'T';
		keys[SDLK_f] = 'U';
		keys[SDLK_PERIOD] = 'V';
		keys[SDLK_COMMA] = 'W';
		keys[SDLK_b] = 'X';
		keys[SDLK_t] = 'Y';
		keys[SDLK_SLASH] = 'Z';
		keys[SDLK_BACKSPACE] = parser.C_BS;
		keys[SDLK_RETURN] = parser.C_CR;
		keys[SDLK_SPACE] = parser.C_SP;
	}

	// Delay to wait for monitor to change modes if necessary
	// This ought to be made more intelligent
	ticks1 = SDL_GetTicks();
	do
	{
		ticks2 = SDL_GetTicks();
	} while (ticks2 < ticks1 + 2500);
	
	game.COMINI();
}

void OS_Link::execute()
{
		scheduler.SCHED();
		if (scheduler.ZFLAG == 0xFF)
		{
			game.LoadGame();
			scheduler.ZFLAG = 0;
		}
		else
		{
			if (game.AUTFLG)
			{
				if (game.demoRestart)
				{
					// Restart demo
					game.hasWon = false;
					game.DEMOPTR = 0;
					object.Reset();
					creature.Reset();
					parser.Reset();
					player.Reset();
					scheduler.Reset();
					viewer.Reset();
					dungeon.VFTPTR = 0;
					game.COMINI();
				}
				else
				{
					// Start new game
					game.AUTFLG = false;
					game.Restart();
				}
			}
			else
			{
				game.Restart();
			}
		}
}

// Used to check for keystrokes and application termination
void OS_Link::process_events()
{
#ifdef _3DS
	CtrUtils::ScanInput();
	u32 kDown = hidKeysDown();
	u32 keys[] = {KEY_START, KEY_SELECT, KEY_L, KEY_R, KEY_DLEFT, KEY_DRIGHT, KEY_DUP, KEY_DDOWN,
				  KEY_A, KEY_B, KEY_X, KEY_Y};
	int numKeys = sizeof(keys)/sizeof(u32);

	for(int index = 0; index < numKeys; index++)
	{
		if(kDown & keys[index])
		{
			handle_joybutton_down(keys[index]);
		}
	}
#endif
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		printf("Event type: %d\n", event.type);
		switch(event.type)
		{
		case SDL_JOYBUTTONDOWN:
			handle_joybutton_down(event.jbutton.button);
			break;
		case SDL_KEYDOWN:
			handle_key_down(&event.key.keysym);
			break;
		case SDL_QUIT:
			quitGame();
			break;
		case SDL_VIDEOEXPOSE:
			SDL_GL_SwapBuffers();
			break;
		}
	}
}

// Quits application
void OS_Link::deinitialize()
{
	Mix_CloseAudio();
	SDL_JoystickClose(joystick);
	SDL_Quit();
}

void OS_Link::type_command(char* command)
{
	if(command != NULL)
	{
		dodBYTE c;
		int i = 0;
		do
		{
			c = command[i];
			if(c == '\n')
			{
				parser.KBDPUT(parser.C_CR);
			}
			else
			{
				parser.KBDPUT(c);
			}
			i++;

		}while(c != '\0');
		parser.KBDPUT(parser.C_CR);

		fprintf(outputFile, "%s\n", commandCreatorBuffer);
	}

}

void OS_Link::handle_joybutton_down(int button)
{
	static int row = 0, col = 0;
	static int prevRow = 0, prevCol = 0;
	static int position = 0;
	static command_menu commandMenu;
	if (viewer.display_mode == Viewer::MODE_MAP)
	{
		switch(button)
		{
		case SDL_PSP_START:
			main_menu();
			break;
		default:
			viewer.display_mode = Viewer::MODE_3D;
			--viewer.UPDATE;
			parser.KBDPUT(32); // This is a (necessary ???) hack.
		break;
		}
	}
	else if (viewer.display_mode == Viewer::MODE_KEYBOARD)
	{
		bool end = false;
		viewer.drawKeyboard(keyboardKeys[position]);
		switch(button)
		{
		case SDL_PSP_CROSS:
			if(keyboardKeys[position].key == 0x0D)
			{
				type_command(commandCreatorBuffer);
				memset(commandCreatorBuffer, 0, sizeof(commandCreatorBuffer));
				end = true;
				position = 0;
			}
			else
			{
				strcat(commandCreatorBuffer, &keyboardKeys[position].key);
			}
			break;
		case SDL_PSP_CIRCLE:
			end = true;
			position = 0;
			break;
		case SDL_PSP_UP:
			position -= 10;
			if(position < 0)
			{
				// set it to the space bar
				position = MAX_KEYBOARD_KEYS - 1;
			}
			break;
		case SDL_PSP_DOWN:
			if(position == MAX_KEYBOARD_KEYS - 1)
			{
				// We're on the space bar so just set it to the first key
				position = 0;
			}
			else
			{
				position += 10;
				if(position >= MAX_KEYBOARD_KEYS)
				{
					// Not on the space bar yet so just go to it
					position = MAX_KEYBOARD_KEYS - 1;
				}
			}
			break;
		case SDL_PSP_LEFT:
			position --;
			if(position < 0)
				position = MAX_KEYBOARD_KEYS - 1;
			break;
		case SDL_PSP_RIGHT:
			position ++;
			if(position >= MAX_KEYBOARD_KEYS)
			position = 0;
			break;
		}

		if(end == true)
		{
			// Put it back to the game
			viewer.display_mode = Viewer::MODE_3D;
		}
		else
		{
			viewer.drawKeyboard(keyboardKeys[position]);
		}
	}
	else if (viewer.display_mode == Viewer::MODE_COMMAND_CREATOR)
	{
		bool end = false;
		viewer.drawCommandMenu(commandMenu, col, row);
		switch(button)
		{
		case SDL_PSP_CROSS:
 			end = command_menu_return(&col, &row, &prevCol, &prevRow, commandMenu);
	        break;
	    case SDL_PSP_TRIANGLE:
	    	if(col == OBJECTS_MENU_SWITCH)
	    	{
	    		col = OBJECT_TYPES_MENU_SWITCH;
	    		end = command_menu_return(&col, &row, &prevCol, &prevRow, commandMenu);
			}
	    	break;
        case SDL_PSP_UP:
	 		(row < 1) ? row = commandMenu.getMenuSize(col) - 1 : row--;
         	break;
        case SDL_PSP_DOWN:
	 		(row > commandMenu.getMenuSize(col) - 2) ? row = 0 : row++;
         	break;
        case SDL_PSP_LEFT:
         	break;
        case SDL_PSP_RIGHT:
	 		break;
		case SDL_PSP_L_TRIGGER:
			col = 0;
			prevCol = 0;
			row = 0;
			prevRow = 0;
			end = true;
			break;
        case SDL_PSP_CIRCLE:
		/*	if(col == 0)			// TO BE FIXED
			{
				end = true;
			}
			else
			{
				col = prevCol;
				row = prevRow;
			}*/
			col = 0;
			prevCol = 0;
			row = 0;
			prevRow = 0;
			end = true;
	 		break;
        default:
	 		break;
		}
		if(end == true)
		{
			viewer.display_mode = Viewer::MODE_3D;
		}
		else if(viewer.display_mode == Viewer::MODE_COMMAND_CREATOR)
		{
			viewer.drawCommandMenu(commandMenu, col, row);
		}
	}
	else
	{
		switch(button)
		{
			case SDL_PSP_TRIANGLE:
				if(viewer.display_mode == Viewer::MODE_3D)
				{
					type_command("EXAMINE");
				}
				else if(viewer.display_mode == Viewer::MODE_EXAMINE)
				{
					type_command("LOOK");
				}
				break;
			case SDL_PSP_CIRCLE:
				viewer.display_mode = Viewer::MODE_KEYBOARD;
				memset(commandCreatorBuffer, 0, 256*sizeof(char));
				viewer.drawKeyboard(keyboardKeys[position]);
				break;
			case SDL_PSP_CROSS:
				type_command("ATTACK RIGHT");
				break;
			case SDL_PSP_SQUARE:
				type_command("ATTACK LEFT");
				break;
			case SDL_PSP_L_TRIGGER:
				viewer.display_mode = Viewer::MODE_COMMAND_CREATOR;
				memset(commandCreatorBuffer, 0, 256*sizeof(char));
				viewer.drawCommandMenu(commandMenu, col, row);
				break;
			case SDL_PSP_R_TRIGGER:
				type_command("TURN AROUND");
				break;
			case SDL_PSP_DOWN:
				type_command("MOVE BACK");
				break;
			case SDL_PSP_LEFT:
				type_command("TURN LEFT");
				break;
			case SDL_PSP_UP:
				type_command("MOVE");
				break;
			case SDL_PSP_RIGHT:
				type_command("TURN RIGHT");
				break;
			case SDL_PSP_SELECT:
				type_command("P L T\nU L\nP R SW");
				break;
			case SDL_PSP_START:
				main_menu();
				break;
		}
	}

}

// Processes key strokes.
void OS_Link::handle_key_down(SDL_keysym * keysym)
{
	dodBYTE c;
	if (viewer.display_mode == Viewer::MODE_MAP)
	{
		switch(keysym->sym)
		{
		case SDLK_ESCAPE:
			main_menu();
			break;
		default:
			viewer.display_mode = Viewer::MODE_3D;
			--viewer.UPDATE;
			parser.KBDPUT(32); // This is a (necessary ???) hack.
			break;
		}

	}
	else
	{
		switch(keysym->sym)
		{
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
		case SDLK_RCTRL:
		case SDLK_LCTRL:
		case SDLK_RALT:
		case SDLK_LALT:
		case SDLK_RMETA:
		case SDLK_LMETA:
		case SDLK_LSUPER:
		case SDLK_RSUPER:
		case SDLK_MODE:
		case SDLK_COMPOSE:
		case SDLK_NUMLOCK:
		case SDLK_CAPSLOCK:
		case SDLK_SCROLLOCK:
			// ignore these keys
			return;

		case SDLK_ESCAPE:
			main_menu();   // Enter the meta-menu routine
                        return;

//		case SDLK_EXCLAIM: c = '!'; break;
//		case SDLK_LESS: c = '<'; break;
//		case SDLK_GREATER: c = '>'; break;
//		case SDLK_QUESTION: c = '?'; break;
//		case SDLK_UNDERSCORE: c = '_'; break;
//		case SDLK_PERIOD: c = '.'; break;
//		case SDLK_LEFTBRACKET: c = '{'; break;
//		case SDLK_RIGHTBRACKET: c = '}'; break;

		default:
			c = keys[keysym->sym];
			break;
		}

		parser.KBDPUT(c);
	}
}

bool OS_Link::command_menu_return(int *menu_id, int *item, int *prev_menu_id, int *prev_item, command_menu commandMenu)
{
	switch(*menu_id)
	{
		case ACTION_MENU_SWITCH:
			switch(*item)
			{
				case ACTION_MENU_GET:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = HAND_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_PULL:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = HAND_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_CLIMB:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = CLIMB_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_EXAMINE:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					type_command(commandCreatorBuffer);
					*menu_id = 0;
					*prev_menu_id = 0;
					*item = 0;
					*prev_item = 0;
					return true;
					break;
				case ACTION_MENU_LOOK:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					type_command(commandCreatorBuffer);
					*menu_id = 0;
					*prev_menu_id = 0;
					*item = 0;
					*prev_item = 0;
					return true;
					break;
				case ACTION_MENU_DROP:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = HAND_NO_OB_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_STOW:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = HAND_NO_OB_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_USE:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = HAND_NO_OB_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_REVEAL:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = HAND_NO_OB_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_INCANT:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					viewer.display_mode = Viewer::MODE_KEYBOARD;
					viewer.drawKeyboard(keyboardKeys[0]);
					*item = 0;
					break;
				case ACTION_MENU_MOVE:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = MOVE_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_TURN:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = TURN_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_ATTACK:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					strcat(commandCreatorBuffer, " ");
					*prev_menu_id = *menu_id;
					*prev_item = *item;
					*menu_id = HAND_NO_OB_MENU_SWITCH;
					*item = 0;
					break;
				case ACTION_MENU_ZSAVE:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					type_command(commandCreatorBuffer);
					*menu_id = 0;
					*prev_menu_id = 0;
					*item = 0;
					*prev_item = 0;
					return true;
					break;
				case ACTION_MENU_ZLOAD:
					strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
					type_command(commandCreatorBuffer);
					*menu_id = 0;
					*prev_menu_id = 0;
					*item = 0;
					*prev_item = 0;
					return true;
					break;
				}
			break;
		case HAND_MENU_SWITCH:
			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			*prev_menu_id = *menu_id;
			*prev_item = *item;
			*menu_id = OBJECTS_MENU_SWITCH;
			*item = 0;

			break;
		case HAND_NO_OB_MENU_SWITCH:
			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			type_command(commandCreatorBuffer);

			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;

			break;
		case OBJECTS_MENU_SWITCH:

			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			//strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);

			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case OBJECT_TYPES_MENU_SWITCH:
			switch(*item)
			{
			case OBJECTS_MENU_TORCH:
				*prev_menu_id = *menu_id;
				*prev_item = *item;
				*menu_id = TORCH_TYPES_MENU_SWITCH;
				*item = 0;
				break;
			case OBJECTS_MENU_SWORD:
				*prev_menu_id = *menu_id;
				*prev_item = *item;
				*menu_id = SWORD_TYPES_MENU_SWITCH;
				*item = 0;
				break;
			case OBJECTS_MENU_SHIELD:
				*prev_menu_id = *menu_id;
				*prev_item = *item;
				*menu_id = SHIELD_TYPES_MENU_SWITCH;
				*item = 0;
				break;
			case OBJECTS_MENU_FLASK:
				*prev_menu_id = *menu_id;
				*prev_item = *item;
				*menu_id = FLASK_TYPES_MENU_SWITCH;
				*item = 0;
				break;
			case OBJECTS_MENU_SCROLL:
				*prev_menu_id = *menu_id;
				*prev_item = *item;
				*menu_id = SCROLL_TYPES_MENU_SWITCH;
				*item = 0;
				break;
			default:
				strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
				type_command(commandCreatorBuffer);
				*menu_id = 0;
				*prev_menu_id = 0;
				*item = 0;
				*prev_item = 0;
				return true;
				break;
			}
			break;
		case CLIMB_MENU_SWITCH:

			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);

			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case MOVE_MENU_SWITCH:

			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);

			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case TURN_MENU_SWITCH:

			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);

			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case TORCH_TYPES_MENU_SWITCH:
			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);
			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case SCROLL_TYPES_MENU_SWITCH:
			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);
			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case SWORD_TYPES_MENU_SWITCH:
			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);
			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case SHIELD_TYPES_MENU_SWITCH:
			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);
			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
		case FLASK_TYPES_MENU_SWITCH:
			strcat(commandCreatorBuffer, commandMenu.getMenuItem(*menu_id, *item));
			strcat(commandCreatorBuffer, " ");

			type_command(commandCreatorBuffer);
			*menu_id = 0;
			*prev_menu_id = 0;
			*item = 0;
			*prev_item = 0;
			return true;
			break;
	}
	return false;
}

/*********************************************************
  Member: main_menu

  Function: Implements the menu, and dispatches commands

  Returns:  true  - If a new game is started
            false - otherwise
*********************************************************/
bool OS_Link::main_menu()
{
 bool end = false;
 static int row = 0, col = 0;
 static menu mainMenu;

 scheduler.pause(true);
 viewer.drawMenu(mainMenu, col, row);

 do
   {
#ifdef _3DS
	CtrUtils::ScanInput();
	u32 kDown = hidKeysDown();

	if(kDown & SDL_PSP_CROSS)
	{
		end = menu_return(col, row, mainMenu);

   		// Used for Wizard fade functions, if it's a new game, it will trigger a key press
	 	if(col == FILE_MENU_SWITCH && row == FILE_MENU_NEW)
	  		return true;
	}
	else if(kDown & SDL_PSP_UP)
	{
		(row < 1) ? row = mainMenu.getMenuSize(col) - 1 : row--;
    }
    else if(kDown & SDL_PSP_DOWN)
	{
	 		(row > mainMenu.getMenuSize(col) - 2) ? row = 0 : row++;
   	}
	else if(kDown & SDL_PSP_LEFT)
	{
	 		(col < 1) ? col = NUM_MENU - 1 : col--;
	 		row = 0;
	}
	else if(kDown & SDL_PSP_RIGHT)
	{
		(col > 1) ? col = 0 : col++;
		row = 0;
	}
	else if(kDown & SDL_PSP_CIRCLE)
	{
    	end = true;
	}
	if(kDown > 0)
	{
		viewer.drawMenu(mainMenu, col, row);
	}

#endif
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
   switch(event.type)
     {
	 case SDL_JOYBUTTONDOWN:
	 	switch(event.jbutton.button)
	 	{
		case SDL_PSP_CROSS:
	 			end = menu_return(col, row, mainMenu);

	   		// Used for Wizard fade functions, if it's a new game, it will trigger a key press
		 	if(col == FILE_MENU_SWITCH && row == FILE_MENU_NEW)
		  	 return true;

	         break;
        case SDL_PSP_UP:
	 		(row < 1) ? row = mainMenu.getMenuSize(col) - 1 : row--;
         	break;
        case SDL_PSP_DOWN:
	 		(row > mainMenu.getMenuSize(col) - 2) ? row = 0 : row++;
         	break;
        case SDL_PSP_LEFT:
	 		(col < 1) ? col = NUM_MENU - 1 : col--;
	 		row = 0;
         	break;
        case SDL_PSP_RIGHT:
	 		(col > 1) ? col = 0 : col++;
	 		row = 0;
	 		break;
        case SDL_PSP_CIRCLE:
       	  	end = true;
	 		break;
        default:
	 		break;
		}
     case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
        {
        case SDLK_RETURN:
	 end = menu_return(col, row, mainMenu);

	   // Used for Wizard fade functions, if it's a new game, it will trigger a key press
	 if(col == FILE_MENU_SWITCH && row == FILE_MENU_NEW)
	   return true;

         break;
        case SDLK_UP:
	 (row < 1) ? row = mainMenu.getMenuSize(col) - 1 : row--;
         break;
        case SDLK_DOWN:
	 (row > mainMenu.getMenuSize(col) - 2) ? row = 0 : row++;
         break;
        case SDLK_LEFT:
	 (col < 1) ? col = NUM_MENU - 1 : col--;
	 row = 0;
         break;
        case SDLK_RIGHT:
	 (col > 1) ? col = 0 : col++;
	 row = 0;
	 break;
        case SDLK_ESCAPE:
         end = true;
	 break;
        default:
	 break;
        }
      viewer.drawMenu(mainMenu, col, row);
      break;
     case SDL_QUIT:
      quitGame();
      break;
     case SDL_VIDEOEXPOSE:
      SDL_GL_SwapBuffers();
      break;
      }
     }
  } while(!end);

  scheduler.pause(false);

  return false;
}

/* Function to process menu commands
*
*  Returns:  false - if menu should be redrawn
*            true  - otherwise */

bool OS_Link::menu_return(int menu_id, int item, menu Menu)
{
switch(menu_id)
 {
  // File Menu
 case FILE_MENU_SWITCH:
 switch(item)
  {
  case FILE_MENU_NEW:
   //New Game
   scheduler.pause(false);  // Needed so that the game can be paused again later

   if(!game.AUTFLG)
    {
    game.hasWon = true;
    game.demoRestart = false;
    }
   return true;
   break;

  case FILE_MENU_SAVE:
  {
	char *menuList[10];
	
	for(int i = 0; i < 10; i++)
	{
		menuList[i] = new char[50];
		memset(menuList[i], 0, 50);
	}

	int error;
	struct stat fstats;
	struct tm* time;
	char date[34];
	char filename[34];
	memset(filename, 0, sizeof(filename));

	sprintf(filename, "%s%sgame.dod", savedDir, pathSep);

#ifdef _3DS
	u64 mtime;
	error = archive_getmtime(filename, &mtime);
	fstats.st_mtime = mtime;
#else
	error = stat(filename, &fstats);
#endif
	memset(gamefile, 0, gamefileLen);
	strcpy(gamefile,savedDir);
	strcat(gamefile,pathSep);
	if(error == 0)
	{
		time = localtime(&fstats.st_mtime);
		strftime(date, sizeof(date), "DEFAULT - %Y.%m.%d %I:%M %p", time);
		strncpy(menuList[0], date, sizeof(date));
	}
	else
	{
		strcpy(menuList[0], "DEFAULT");
	}

	for(int i = 1; i < 10; i++)
	{
		sprintf(filename, "%s%sgame%d.dod", savedDir, pathSep, i);
#ifdef _3DS
	u64 mtime;
	error = archive_getmtime(filename, &mtime);
	fstats.st_mtime = mtime;
#else
	error = stat(filename, &fstats);
#endif

		if(error == 0)
		{
			time = localtime(&fstats.st_mtime);
			strftime(date, sizeof(date), "%Y.%m.%d %I:%M %p", time);

			snprintf(menuList[i], sizeof(date), "SAVE %d  - %s", i, date);
		}
		else
		{
			sprintf(date, "SAVE %d", i);
			strcpy(menuList[i], date);
		}
	}

	char *overwriteMenuList[]={ "YES", "NO" };
	memset(gamefile, 0, gamefileLen);
	strcpy(gamefile,savedDir);
	strcat(gamefile,pathSep);

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 10))
    {
	case 0:
		strcat(gamefile, "game.dod");
		break;
	case 1:
		strcat(gamefile, "game1.dod");
		break;
	case 2:
		strcat(gamefile, "game2.dod");
		break;
	case 3:
		strcat(gamefile, "game3.dod");
		break;
	case 4:
		strcat(gamefile, "game4.dod");
		break;
	case 5:
		strcat(gamefile, "game5.dod");
		break;
	case 6:
		strcat(gamefile, "game6.dod");
		break;
	case 7:
		strcat(gamefile, "game7.dod");
		break;
	case 8:
		strcat(gamefile, "game8.dod");
		break;
	case 9:
		strcat(gamefile, "game9.dod");
		break;
	}

	for(int i = 0; i < 10; i++)
	{
		delete menuList[i];
	}
	
	// Does the save file exist?
	if ((fptr = fopen(gamefile,"r")) != NULL)
	{
		// Remove all ties to the file
		fclose(fptr);
		switch(menu_list(menu_id * 5, item + 2, "SAVE FILE EXISTS. OVERWRITE IT?", overwriteMenuList, 2))
		{
			case 0:
				//remove(gamefile);
				//scheduler.SAVE();
				++scheduler.ZFLAG;
				fprintf(outputFile, "overwriting file: %s\n", gamefile);
				return true;
			break;
			default:
			break;
		}
	}
	else
	{
		//scheduler.SAVE();
		++scheduler.ZFLAG;
		fprintf(outputFile, "saving to file: %s\n", gamefile);
		return true;
	}
  return false;
  break;
  }
    case FILE_MENU_LOAD:
  {
	char *menuList[10]; 
	
	for(int i = 0; i < 10; i++)
	{
		menuList[i] = new char[50];
		memset(menuList[i], 0, 50);
	}
	
	int error;
	struct stat fstats;
	struct tm* time;
	char date[34];
	char filename[34];
	memset(filename, 0, sizeof(filename));

	sprintf(filename, "%s%sgame.dod", savedDir, pathSep);

#ifdef _3DS
	u64 mtime;
	error = archive_getmtime(filename, &mtime);
	fstats.st_mtime = mtime;
#else
	error = stat(filename, &fstats);
#endif
	memset(gamefile, 0, gamefileLen);
	strcpy(gamefile,savedDir);
	strcat(gamefile,pathSep);
	if(error == 0)
	{
		time = localtime(&fstats.st_mtime);
		
		strftime(date, sizeof(date), "DEFAULT - %Y.%m.%d %I:%M %p", time);
		strncpy(menuList[0], date, sizeof(date));
	}
	else
	{
		strcpy(menuList[0], "DEFAULT");
	}

	for(int i = 1; i < 10; i++)
	{
		sprintf(filename, "%s%sgame%d.dod", savedDir, pathSep, i);
#ifdef _3DS
	u64 mtime;
	error = archive_getmtime(filename, &mtime);
	fstats.st_mtime = mtime;
#else
	error = stat(filename, &fstats);
#endif

		if(error == 0)
		{
			time = localtime(&fstats.st_mtime);
			strftime(date, sizeof(date), "%Y.%m.%d %I:%M %p", time);
			snprintf(menuList[i], sizeof(date), "LOAD %d  - %s", i, date);
		}
		else
		{
			sprintf(date, "LOAD %d", i);
			strcpy(menuList[i], date);
		}
	}

	memset(gamefile, 0, gamefileLen);
	strcpy(gamefile, savedDir);
	strcat(gamefile, pathSep);
	
   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 10))
    {
	case 0:
		strcat(gamefile, "game.dod");
		break;
	case 1:
		strcat(gamefile, "game1.dod");
		break;
	case 2:
		strcat(gamefile, "game2.dod");
		break;
	case 3:
		strcat(gamefile, "game3.dod");
		break;
	case 4:
		strcat(gamefile, "game4.dod");
		break;
	case 5:
		strcat(gamefile, "game5.dod");
		break;
	case 6:
		strcat(gamefile, "game6.dod");
		break;
	case 7:
		strcat(gamefile, "game7.dod");
		break;
	case 8:
		strcat(gamefile, "game8.dod");
		break;
	case 9:
		strcat(gamefile, "game9.dod");
		break;
	}

	for(int i = 0; i < 10; i++)
	{
		delete menuList[i];
	}


	if ((fptr = fopen(gamefile,"r")) != NULL)
	{
		fclose(fptr);

		//scheduler.LOAD();
		//viewer.STATUS();
		--scheduler.ZFLAG;
		fprintf(outputFile, "Loading from: %s\n", gamefile);
		return true;
	}
  return false;
  break;
  }
  case FILE_MENU_RETURN:
   //Return
   return true;
   break;

  case FILE_MENU_ABORT:
   //Abort (Restart)
   scheduler.pause(false);  // Needed so that the game can be paused again later

   if(!game.AUTFLG)
    {
    game.AUTFLG = true;
    game.hasWon = true;
    game.demoRestart = true;
    }
   return true;
   break;

  case FILE_MENU_EXIT:
   //Exit
   quitGame();
  }

  // Configuration Menu
 case CONFIG_MENU_SWITCH:
 switch(item)
  {
  case CONFIG_MENU_FULL_SCREEN:
   //Full Screen
   {
   char *menuList[]={ "ON", "OFF" };

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 2))
    {
    case 0:
      if(!FullScreen)
       changeFullScreen();
      break;

    case 1:
      if(FullScreen)
       changeFullScreen();
      break;

    default:
      return false;
      break;
    }
   }
   return false;
   break;

  case CONFIG_MENU_VIDEO_RES:
   // Video Res
   {
   char *menuList[]={ "STRETCH TO SCREEN", "FIT TO HEIGHT", "ORIGINAL SIZE"};

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 3))
    {
    case 0:
	 videoMode = DISPLAY_STRETCHED;
     break;
    case 1:
     videoMode = DISPLAY_FIT;
     break;
    case 2:
     videoMode = DISPLAY_ORIGINAL;
     break;
    default:
     return false;
     break;
    }

    crd.setDisplay(videoMode);
   }
   return false;
   break;

  case CONFIG_MENU_GRAPHICS:
    // Graphics (Normal /HIRes / vect)
   {
   char *menuList[]={ "NORMAL GRAPHICS", "HIRES GRAPHICS", "VECTOR GRAPHICS" };

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 3))
    {
    case 0:
     g_options &= ~(OPT_VECTOR|OPT_HIRES);
     break;

    case 1:
     g_options &= ~(OPT_VECTOR);
     g_options |= OPT_HIRES;
     break;

    case 2:
     g_options &= ~(OPT_HIRES);
     g_options |= OPT_VECTOR;
     break;

    default:
     return false;
     break;
    }
   }
   return true;
   break;

  case CONFIG_MENU_COLOR:
   // Color (B&W / Art. / Full)
   {
   char *menuList[]={ "BLACK WHITE" };

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 1))
    {
    default:
     return false;
     break;
    }
   }
   return true;
   break;

  case CONFIG_MENU_VOLUME:
   // Volume
   {
   volumeLevel = menu_scrollbar("VOLUME LEVEL", 0, 128, volumeLevel);
   Mix_Volume(-1, volumeLevel);
   }
   return false;
   break;

  case CONFIG_MENU_SAVEDIR:
   // Save Dir
   {
   char *menuList[]={ "EDIT OPTS.INI FILE" };

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 1))
    {
    default:
     return false;
     break;
    }
   }
   return false;
   break;

  case CONFIG_MENU_CREATURE_SPEED:
    // Creature Speed
   {
   char *menuList[2] = {"COCO", "CUSTOM"};

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 2))
    {
    case 0:
     //Coco Speed
     creature.creSpeedMul = 200;
     creature.UpdateCreSpeed();
     break;
    case 1:
     //Custom Speed
     creature.creSpeedMul = menu_scrollbar("CREATURE SPEED", 50, 200, volumeLevel);
     creature.UpdateCreSpeed();
     return false;
     break;

    default:
     return false;
     break;
    }
   }
   break;

  case CONFIG_MENU_REGEN_SPEED:
   // Regen Speed
   {
   char *menuList[] = { "5 MINUTES", "3 MINUTES", "1 MINUTE" };

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 3))
     {
     case 0:
      creatureRegen = 5;
      scheduler.updateCreatureRegen(creatureRegen);
      break;
     case 1:
      creatureRegen = 3;
      scheduler.updateCreatureRegen(creatureRegen);
      break;
     case 2:
      creatureRegen = 1;
      scheduler.updateCreatureRegen(creatureRegen);
      break;
     default:
      return false;
      break;
     }
   }
   return true;
   break;

  case CONFIG_MENU_RANDOM_MAZE:
   // Random Mazes
   {
   char *menuList[]={ "ON", "OFF" };

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 2))
    {
	case 0:
		game.RandomMaze = true;
		break;
	case 1:
		game.RandomMaze = false;
	    break;
    default:
     return false;
     break;
    }
   }
   return false;
   break;

  case CONFIG_MENU_SND_MODE:
   // Sound Style (Sync, Stereo)
   {
   char *menuList[2] = {"STEREO", "MONO"};

   switch(menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 2))
    {
    case 0:
    g_options |= OPT_STEREO;
    break;

    case 1:
    g_options &= ~OPT_STEREO;
    break;

    default:
    return false;
    break;
    }
   }
   break;

  case CONFIG_MENU_SAVE_OPT:
   saveOptFile();
   return true;
   break;

  case CONFIG_MENU_DEFAULTS:
   loadDefaults();
   crd.setDisplay(videoMode);
   return true;
   break;
  }

  // Help menu
 case HELP_MENU_SWITCH:
 switch(item)
  {
  case HELP_MENU_HOWTOPLAY:
   // How to play
   {
 #ifdef _3DS
	   if(load_manual() == -1)
	   {
		    char *menuList[]={ "COULD NOT LOAD MANUAL" };

   			menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 1);
	   }
#else
   char *menuList[]={ "SEE FILE HOWTOPLAY.TXT" };

   menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 1);
#endif
   }
   return false;
   break;

  case HELP_MENU_LICENSE:
   // License
   {
   char *menuList[]={ "SEE FILE README.TXT" };

   menu_list(menu_id * 5, item + 2, Menu.getMenuItem(menu_id, item), menuList, 1);
   }
   return false;
   break;

  case HELP_MENU_ABOUT:
   // About
   {
   SDL_Event event;

   viewer.aboutBox();
   while(true)
    {
#ifdef _3DS
		CtrUtils::ScanInput();
		if(hidKeysDown() > 0)
		{
			return false;
		}
#endif
    while(SDL_PollEvent(&event))
     {
     switch(event.type)
      {
	  case SDL_JOYBUTTONDOWN:
		return false;
		break;
      case SDL_KEYDOWN:
       return false;
       break;
      case SDL_QUIT:
       quitGame();
       break;
      case SDL_VIDEOEXPOSE:
       SDL_GL_SwapBuffers();
       break;
      }
     }
    }
   return false;
   }
   break;
  }
 }
 return true;
}

int OS_Link::load_manual()
{
	Manual manual("/3ds/3dsdod/manual");

	if(!manual.isInitialized())
	{
		return -1;
	}

	bool redraw = true;
	while(true)
	{
		if(redraw)
		{
			viewer.drawManual(&manual);
			redraw = false;
		}
#ifdef _3DS
		CtrUtils::ScanInput();
		u32 kDown = hidKeysDown();
		u32 hDown = hidKeysHeld();

		if (kDown & SDL_PSP_RIGHT)
		{
			manual.nextPage();
			redraw = true;
		}

		if (kDown & SDL_PSP_LEFT)
		{
			manual.previousPage();
			redraw = true;
		}

		if(kDown & SDL_PSP_CIRCLE)
		{
			return 0;
		}

		if (hDown & SDL_PSP_DOWN)
		{
			manual.scrollDown();
			redraw = true;
		}

		if (hDown & SDL_PSP_UP)
		{
			manual.scrollUp();
			redraw = true;
		}

#endif
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_JOYBUTTONDOWN:
				switch(event.jbutton.button)
				{
					case SDL_PSP_RIGHT:
						manual.nextPage();
						redraw = true;
						break;
					case SDL_PSP_LEFT:
						manual.previousPage();
						redraw = true;
						break;

					case SDL_PSP_CIRCLE:
						return 0;
					case SDL_PSP_DOWN:
						manual.scrollDown();
						redraw = true;
						break;

					case SDL_PSP_UP:
						manual.scrollUp();
						redraw = true;
						break;
					default:
						break;
				}
			}
		}
	}
}

/*****************************************************************************
*  Function used to draw a list, move among that list, and return the item selected
*
*  Arguments: x        - The top-left x-coordinate to draw list at
*             y        - The top-left y-coordinate to draw list at
*             title    - The title of the list
*             list     - An array of strings (the list to be chosen from
*             listSize - The size of the array
******************************************************************************/
int OS_Link::menu_list(int x, int y, char *title, char *list[], int listSize)
 {
 int currentChoice = 0;

 while(true)
   {
   viewer.drawMenuList(x, y, title, list, listSize, currentChoice);
#ifdef _3DS
	CtrUtils::ScanInput();
	u32 kDown = hidKeysDown();

	if(kDown & SDL_PSP_CROSS)
	{
		return(currentChoice);
	}
	else if(kDown & SDL_PSP_UP)
	{
		(currentChoice < 1) ? currentChoice = listSize - 1 : currentChoice--;
	}
	else if(kDown & SDL_PSP_DOWN)
	{
		(currentChoice > listSize - 2) ? currentChoice = 0 : currentChoice++;
	}
	else if(kDown & SDL_PSP_CIRCLE)
	{
		return(-1);
	}

#endif
   SDL_Event event;
	while(SDL_PollEvent(&event))
	{
	switch(event.type)
	{
	case SDL_JOYBUTTONDOWN:
		switch(event.jbutton.button)
		{
		case SDL_PSP_CROSS:
			return(currentChoice);
		break;

		case SDL_PSP_UP:
			(currentChoice < 1) ? currentChoice = listSize - 1 : currentChoice--;
		break;

		case SDL_PSP_DOWN:
			(currentChoice > listSize - 2) ? currentChoice = 0 : currentChoice++;
		break;

		case SDL_PSP_CIRCLE:
			return(-1);
		break;

        default:
		break;

		}
	 break;
     case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
        {
        case SDLK_RETURN:
         return(currentChoice);
         break;

        case SDLK_UP:
	 (currentChoice < 1) ? currentChoice = listSize - 1 : currentChoice--;
         break;

        case SDLK_DOWN:
	 (currentChoice > listSize - 2) ? currentChoice = 0 : currentChoice++;
         break;

        case SDLK_ESCAPE:
	 return(-1);
	 break;

        default:
	 break;
        }
      break;
     case SDL_QUIT:
      quitGame();
      break;
     case SDL_VIDEOEXPOSE:
      SDL_GL_SwapBuffers();
      break;
      }
     }
  } // End of while loop

 return(-1);
 }

/*****************************************************************************
*  Function used to draw a scrollbar, and return the value
*
*  Arguments: title     - The title of the entry
*             min       - The minimum value the scroll bar can take
*             max       - The maximum value the scroll bar can take
*             current   - The current position of the scrollbar
*
*  Returns: The value the user entered, or if they hit escape, the original
*           value.
******************************************************************************/
int OS_Link::menu_scrollbar(char *title, int min, int max, int current)
 {
 int oldvalue  = current; //Save the old value in case the user escapes
 int increment = (max - min) / 31;  // 31 is the number of columns

   // Calculate a relative max and min and corresponding current number
 int newMax    = increment * 31;
 int newMin    = 0;
     current   = current - min;

 viewer.drawMenuScrollbar(title, (current - newMin) / increment);

 while(true)
   {

#ifdef _3DS
	CtrUtils::ScanInput();
	u32 kDown = hidKeysDown();

	if(kDown & SDL_PSP_CROSS)
	{
		return(current + min);  // Readjust back to absolute value
	}
	else if(kDown & SDL_PSP_LEFT)
	{
		(current > newMin) ? current -= increment : current = newMin;
	}
	else if(kDown &  SDL_PSP_RIGHT)
	{
		(current < newMax) ? current += increment : current = newMax;
	}
	else if(kDown & SDL_PSP_CIRCLE)
	{
		return(oldvalue);
	}
	viewer.drawMenuScrollbar(title, (current - newMin) / increment);
#endif
   SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_JOYBUTTONDOWN:
			switch(event.jbutton.button)
	 		{
			case SDL_PSP_CROSS:
				return(current + min);  // Readjust back to absolute value
         	break;

			case SDL_PSP_LEFT:
				(current > newMin) ? current -= increment : current = newMin;
			break;
			case SDL_PSP_RIGHT:
				(current < newMax) ? current += increment : current = newMax;
			break;
			case SDL_PSP_CIRCLE:
				return(oldvalue);
			break;

			default:

			break;
			}
		viewer.drawMenuScrollbar(title, (current - newMin) / increment);
		break;
      case SDL_KEYDOWN:
       switch(event.key.keysym.sym)
        {
        case SDLK_RETURN:
         return(current + min);  // Readjust back to absolute value
         break;

        case SDLK_LEFT:
	 (current > newMin) ? current -= increment : current = newMin;
         break;

        case SDLK_RIGHT:
	 (current < newMax) ? current += increment : current = newMax;
         break;

        case SDLK_ESCAPE:
	 return(oldvalue);
	 break;

        default:
	 break;
	}
       viewer.drawMenuScrollbar(title, (current - newMin) / increment);
       break;
      case SDL_QUIT:
       quitGame();
       break;
      case SDL_VIDEOEXPOSE:
       SDL_GL_SwapBuffers();
       break;
      }
    }
   }
 }

/*****************************************************************************
*  Function used to draw a box for a string entry, then return it
*
*  Arguments: newString - The string to be returned
*             title     - The title of the entry
*             maxLength - The maximum size of the entry
******************************************************************************/
void OS_Link::menu_string(char *newString, char *title, int maxLength)
 {
 *newString = '\0';
 viewer.drawMenuStringTitle(title);
 viewer.drawMenuString(newString);

 while(true)
   {
   SDL_Event event;

   while(SDL_PollEvent(&event))
   {
   switch(event.type)
     {
     case SDL_KEYDOWN:
      switch(event.key.keysym.sym)
        {
        case SDLK_RETURN:
         return;
         break;

        case SDLK_RSHIFT:
        case SDLK_LSHIFT:
        case SDLK_RCTRL:
        case SDLK_LCTRL:
        case SDLK_RALT:
        case SDLK_LALT:
        case SDLK_RMETA:
        case SDLK_LMETA:
        case SDLK_LSUPER:
        case SDLK_RSUPER:
        case SDLK_MODE:
        case SDLK_COMPOSE:
        case SDLK_NUMLOCK:
        case SDLK_CAPSLOCK:
        case SDLK_SCROLLOCK:
        case SDLK_UP:
        case SDLK_DOWN:
          // ignore these keys
          break;

        case SDLK_BACKSPACE:
	case SDLK_LEFT:
	 if(strlen(newString) > 0)
	  {
	  *(newString + strlen(newString) - 1) = '\0';
          viewer.drawMenuStringTitle(title);  // Update with the new word
          viewer.drawMenuString(newString);
	  }
         break;

        case SDLK_ESCAPE:
         *(newString) = '\0';
	 return;
	 break;

        default:
	 if(strlen(newString) < (unsigned int) maxLength)
	  {
	  *(newString + strlen(newString) + 1) = '\0';
	  *(newString + strlen(newString)) = keys[event.key.keysym.sym];
          viewer.drawMenuStringTitle(title);  // Update with the new word
          viewer.drawMenuString(newString);
	  }
	 break;
        }
      break;
     case SDL_QUIT:
      quitGame();
      break;
     case SDL_VIDEOEXPOSE:
      SDL_GL_SwapBuffers();
      break;
      }
     }
  } // End of while loop
 }

/******************************************************************************
*  Function used to load & parse options file
*
*  Arguments: None
******************************************************************************/
void OS_Link::loadOptFile(void)
 {
 char     inputString[80];
 char     fn[20];
 int      in;
 ifstream fin;
 char *   breakPoint;

 loadDefaults(); // In case some variables aren't in the opts file, and if no file exists

 sprintf(fn, "%s%s%s", confDir, pathSep, "opts.ini");

 fin.open(fn);
 if (!fin)
 {
 return;
 }

 fin >> inputString;
 while(fin)
  {
  breakPoint = strchr(inputString, '=');

   // Ignore strings that have no equals, or are only an equals, or have no end
  if(breakPoint || breakPoint == inputString || breakPoint == (inputString + strlen(inputString) - 1))
   {
   *(breakPoint) = '\0';
   breakPoint++;

   if(!strcmp(inputString, "creatureSpeed"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) creature.creSpeedMul = in;
    }
   else if(!strcmp(inputString, "turnDelay"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) player.turnDelay = in;
    }
   else if(!strcmp(inputString, "moveDelay"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) player.moveDelay = in;
    }
   else if(!strcmp(inputString, "keylayout"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) keylayout = in;
    }
   else if(!strcmp(inputString, "graphicsMode"))
    {
    if(!strcmp(breakPoint, "NORMAL"))
     g_options &= ~(OPT_VECTOR|OPT_HIRES);
    else if(!strcmp(breakPoint, "HIRES"))
     {
     g_options &= ~(OPT_VECTOR);
     g_options |= OPT_HIRES;
     }
    else if(!strcmp(breakPoint, "VECTOR"))
     {
     g_options &= ~(OPT_HIRES);
     g_options |= OPT_VECTOR;
     }
    }
   else if(!strcmp(inputString, "stereoMode"))
    {
    if(!strcmp(breakPoint, "STEREO"))
     g_options |= OPT_STEREO;
    else if(!strcmp(breakPoint, "MONO"))
     g_options &= ~OPT_STEREO;
    }
   else if(!strcmp(inputString, "volumeLevel"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) volumeLevel = in;
    }
   else if(!strcmp(inputString, "saveDirectory"))
    {
#ifdef _3DS
	strncpy(savedDir, "/3ds/3dsdod/saved", MAX_FILENAME_LENGTH);
#else
	strncpy(savedDir, "saved", MAX_FILENAME_LENGTH);
#endif
    }
   else if(!strcmp(inputString, "fullScreen"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) FullScreen = in;
    }
   else if(!strcmp(inputString, "screenWidth"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) width = in;
    }
   else if(!strcmp(inputString, "creatureRegen"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) creatureRegen = in;
    }
   else if(!strcmp(inputString, "RandomMaze"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) game.RandomMaze = in;
    }
   else if(!strcmp(inputString, "ShieldFix"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) game.ShieldFix = in;
    }
   else if(!strcmp(inputString, "VisionScroll"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) game.VisionScroll = in;
    }
   else if(!strcmp(inputString, "CreaturesIgnoreObjects"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) game.CreaturesIgnoreObjects = in;
    }
   else if(!strcmp(inputString, "CreaturesInstaRegen"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) game.CreaturesInstaRegen = in;
    }
   else if(!strcmp(inputString, "MarkDoorsOnScrollMaps"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) game.MarkDoorsOnScrollMaps = in;
    }
   else if(!strcmp(inputString, "videoMode"))
    {
    if (1==sscanf(breakPoint, "%d", &in)) videoMode = in;
    }
   }

  fin >> inputString;
  }

 fin.close();
 scheduler.updateCreatureRegen(creatureRegen);
 creature.UpdateCreSpeed();
 }

/******************************************************************************
*  Function used to save the options file from current settings
*
*  Arguments: None
*
*  Returns:   true - file saved successfully, false - file not saved
******************************************************************************/
bool OS_Link::saveOptFile(void)
 {
 ofstream fout;
 char     fn[MAX_FILENAME_LENGTH];

 sprintf(fn, "%s%s%s", confDir, pathSep, "opts.ini");

 fout.open(fn);
 if(!fout)
   return false;

 fout << "creatureSpeed=" << creature.creSpeedMul << endl;
 fout << "turnDelay=" << player.turnDelay << endl;
 fout << "moveDelay=" << player.moveDelay << endl;
 fout << "keylayout=" << keylayout << endl;
 fout << "volumeLevel=" << volumeLevel << endl;
 fout << "saveDirectory=" << savedDir << endl;
 fout << "fullScreen=" << FullScreen << endl;
 fout << "screenWidth=" << width << endl;
 fout << "videoMode=" << videoMode << endl;
 fout << "creatureRegen=" << creatureRegen << endl;

 fout << "graphicsMode=";
 if(g_options & OPT_VECTOR)
   fout << "VECTOR" << endl;
 else if(g_options & OPT_HIRES)
   fout << "HIRES" << endl;
 else
   fout << "NORMAL" << endl;

 fout << "stereoMode=";
 if(g_options & OPT_STEREO)
   fout << "STEREO" << endl;
 else
   fout << "MONO" << endl;

 fout << "RandomMaze=" << game.RandomMaze << endl;
 fout << "ShieldFix=" << game.ShieldFix << endl;
 fout << "VisionScroll=" << game.VisionScroll << endl;
 fout << "CreaturesIgnoreObjects=" << game.CreaturesIgnoreObjects << endl;
 fout << "CreaturesInstaRegen=" << game.CreaturesInstaRegen << endl;
 fout << "MarkDoorsOnScrollMaps=" << game.MarkDoorsOnScrollMaps << endl;

 fout.close();

 return true;
 }

/******************************************************************************
*  Function used to load the options file from current settings
*
*  Arguments: None
******************************************************************************/
void OS_Link::loadDefaults(void)
 {
 player.turnDelay = 37;
 player.moveDelay = 500;
 keylayout   = 0;
 volumeLevel = MIX_MAX_VOLUME;
 creature.creSpeedMul = 200;
 creature.UpdateCreSpeed();
#ifdef _3DS
	strncpy(savedDir, "/3ds/3dsdod/saved", MAX_FILENAME_LENGTH);
#else
	strncpy(savedDir, "saved", MAX_FILENAME_LENGTH);
#endif
 FullScreen = true;
 width = 1024;
 creatureRegen = 5;
 scheduler.updateCreatureRegen(creatureRegen);
 videoMode = DISPLAY_ORIGINAL;

 g_options &= ~(OPT_VECTOR|OPT_HIRES);
 g_options |= OPT_STEREO;
 }

/******************************************************************************
*  Function used to swap fullscreen mode
*
*  Arguments: None
******************************************************************************/
void OS_Link::changeFullScreen(void)
 {
 FullScreen = !FullScreen;
 changeVideoRes(width);
 }

/******************************************************************************
*  Function used to change the video resolution
*
*  Arguments: newWidth - The screen width to change to
******************************************************************************/
 void OS_Link::changeVideoRes(int mode)
 {
	 int newWidth;
	 int newHeight;

	 Renderer *renderer = RendererFactory::GetRenderer();

#ifdef _3DS
	 newHeight = renderer->getScreenHeight();
	 newWidth = renderer->getScreenWidth();
	 width = newWidth;
	 height = newHeight;

	 crd.setDisplay(mode);
#else

	 SDL_Surface *surface;
	 const SDL_VideoInfo *info = NULL;
	 surface = SDL_GetVideoSurface();

	 info = SDL_GetVideoInfo();
	 if (!info)
	 {
		 fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		 quitGame();
	 }

	 newHeight = renderer->getScreenHeight();
	 newWidth = renderer->getScreenWidth();

	 //	flags = flags | SDL_FULLSCREEN | SDL_SWSURFACE|SDL_HWPALETTE|SDL_HWACCEL; //PC Flags
	 flags = renderer->getVideoModeFlags();
	 if ((surface = SDL_SetVideoMode(newWidth, newHeight, bpp, flags)) == 0)
	 {
		 fprintf(stderr, "Video mode set failed: %s\nReturning to old mode\n", SDL_GetError());
		 if ((surface = SDL_SetVideoMode(renderer->getScreenWidth(), renderer->getScreenHeight(), bpp, flags)) == 0)
		 {
			 fprintf(stderr, "Video mode set failed, this should be impossible\n Debug OS_Link.changeVideoRes\nSDL Reported %s\n", SDL_GetError());
			 exit(1);
		 }
	 }
	 else
	 {
		 width = newWidth;
		 height = newHeight;
		 //crd.setCurWH((double) width);
		 crd.setDisplay(mode);
	 }
#endif

	 viewer.initialize();
 }
