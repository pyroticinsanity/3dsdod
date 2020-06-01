# Introduction

For those of you that don't know, Dungeons of Daggorath is a "3D" dungeon 
crawler from 1982. It was originally created by DynaMicro for the TRS-80
and is hailed as one of the first 3D shooters.

# Copyright

The copyright for Dungeons of Daggorath is still held by the original author, Douglas J. Morgan 
but he has granted a non-exclusive permanent world-wide license to reproduce it - See GRANT.
(c) 1982, DynaMicro

## Playing the Game

So how do you play a game that was originally text based? With an awesome UI created 
by yours truly! Every command is executable so there are no problems
beating the game (I've done it so I know it can be done!).

3DS	|	TRS-80
------------ | -------------
UP 	|	MOVE
DOWN |		MOVE BACKWARD
LEFT	|	TURN LEFT
RIGHT|		TURN RIGHT
A		|	ATTACK RIGHT
X		|	ATTACK LEFT
B		|	Brings up the keyboard
Y		|	EXAMINE / LOOK (depending on what mode you're in)
L TRIGGER	|Brings up the command creator menu
R TRIGGER	|TURN AROUND
START	|	Brings up the game menu
SELECT	|	Executes P L T, U L, P R SW (only good for getting started)

## Command Creator Menu
This menu allows you to create all combinations of commands you will ever need.  
ZSAVE and ZLOAD will use the default save file game.dod.

For buttons, A is Confirm and B is Cancel. While choosing an item, you can
press X to specify the type of item.

## Keyboard
The keyboard lets you type in whatever command you want. It is restricted to the characters that are
accepted by the game. Once you have your command go to the Enter key to execute it.

## Menu
You can navigate the menu using the following buttons:
* D-Pad Left - Previous Menu
* D-Pad Right - Next Menu
* D-Pad Down - Move Down
* D-Pad Up - Move Up
* A - Confirm
* B - Cancel/Return to Game

### File
This menu is used for saving, loading, restarting, or exiting the game.
You can Save and Load the game from 10 different slots to give some variety for those 
that don't feel like typing in a save command.

### Configure
* FULL SCREEN - does nothing
* VIDEO MODE - allows you to change the resolution from the original size, to fit to height, 
or stretch to screen. I recommend leaving it on Original Size for the 3DS
* GRAPHICS MODE - switch between vector and normal mode

### Help
I've added the manual under the How To Play section. Due to the 3DS resolution it is a little distorted
but still readable. 

You can use the following buttons:
* D-Pad Up - Scroll Up
* D-Pad Down - Scroll Down
* D-Pad Left - Previous Page
* D-Pad Right - Next Page
* B - Cancel

# Compiling
Before compiling, you'll need the following libraries installed:
```
sudo dkp-pacman -S 3ds-sdl 3ds-sdl_mixer 3ds-sdl_image 3ds-zlib 3ds-libpng 3ds-libvorbisidec 3ds-libjpeg-turbo 3ds-sdl_ttf
```

You also need to have bannertool and makerom in your path.

To compile it just run the following in the root directory:
```
make
make cia
```

Hopefully you guys enjoy this wonderful game!

Darkweb
