/*
	Example code for rendering the Keyboard and the selection box for it.
*/

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <citro2d.h>
#include <citro3d.h>

//This include a header containing definitions of our image
#include "keyboard_t3x.h"

struct kbdKey{
	int x;
	int y;
	int width;
	int height;
	char key;
};

#define MAX_KEYBOARD_KEYS 30

const struct kbdKey keyboardKeys[40] = { 
							{87, 105, 15, 15, 'Q'},
							{107, 105, 15, 15, 'W'},
							{127, 105, 15, 15, 'E'},
							{144, 105, 15, 15, 'R'},
							{163, 105, 15, 15, 'T'},
							{183, 105, 15, 15, 'Y'},
							{201, 105, 15, 15, 'U'},
							{221, 105, 15, 15, 'I'},
							{240, 105, 15, 15, 'O'},
							{259, 105, 15, 15, 'P'},

							{92, 128, 15, 15, 'A'},
							{110, 128, 15, 15, 'S'},
							{130, 128, 15, 15, 'D'},
							{150, 128, 15, 15, 'F'},
							{169, 128, 15, 15, 'G'},
							{188, 128, 15, 15, 'H'},
							{207, 128, 15, 15, 'J'},
							{225, 128, 15, 15, 'K'},
							{246, 128, 15, 15, 'L'},
							{285, 128, 25, 15, 0x0D},

							{99,  150, 15, 15, 'Z'},
							{119,  150, 15, 15, 'X'},
							{139,  150, 15, 15, 'C'},
							{159,  150, 15, 15, 'V'},
							{178,  150, 15, 15, 'B'},
							{197,  150, 15, 15, 'N'},
							{217,  150, 15, 15, 'M'},
							{237,  150, 15, 15, ','},
							{257,  150, 15, 15, '.'},
							{125,  173, 150, 15, ' '}

						};

void drawLine(float x0, float y0, float x1, float y1)
{
	float width = 1.0;
	u32 color = C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f);

	float slope = (x1 - x0) / (y1 - y0);
	float yWidth = width * sin(atan(slope));
	float xWidth = width * cos(atan(slope));

	C2D_DrawTriangle(x0 - xWidth, y0 + yWidth, color, 
		x0 + xWidth, y0 - yWidth, color,
		x1 - xWidth, y1 + yWidth, color, 0);

	C2D_DrawTriangle(x1 - xWidth, y1 + yWidth, color, 
		x0 + xWidth, y0 - yWidth, color,
		x1 + xWidth, y1 - yWidth, color, 0);
}

int main(int argc, char **argv)
{
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	consoleInit(GFX_BOTTOM, NULL);

	C2D_Image keyboardImg;	
	C3D_Tex tex;

	Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture();
	subtex->left=subtex->top=0; subtex->width=400; subtex->height=240;
	subtex->right=subtex->bottom=1;
	keyboardImg.subtex = subtex;
	keyboardImg.tex = &tex;

	u32 clrClear = C2D_Color32(255, 0, 0, 255);

	Tex3DS_Texture t3x = Tex3DS_TextureImport(keyboard_t3x, keyboard_t3x_size, &tex, NULL, false);

	C3D_TexSetFilter(&tex, GPU_LINEAR, GPU_NEAREST);

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);

	int position = 0;

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu


		if(kDown & KEY_DUP)
		{
			position -= 10;
			if(position < 0)
			{
				// set it to the space bar
				position = MAX_KEYBOARD_KEYS - 1;
			}
		}
		else if(kDown & KEY_DDOWN)
		{
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
		}
		else if(kDown & KEY_DLEFT)
		{
			position --;
			if(position < 0)
				position = MAX_KEYBOARD_KEYS - 1;
		}
		else if(kDown & KEY_DRIGHT)
		{
			position ++;
			if(position >= MAX_KEYBOARD_KEYS)
			position = 0;
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		if(!C2D_DrawImageAt(keyboardImg, 0, 0, 0))
		{
			printf("Failed to draw image\n");
		}

		int squareWidth = keyboardKeys[position].width;
		int squareHeight = keyboardKeys[position].height;
		int highlightY = (keyboardKeys[position].y );
		int highlightX = (keyboardKeys[position].x );

		drawLine(highlightX, highlightY, highlightX + squareWidth, highlightY);
		drawLine(highlightX + squareWidth, highlightY, highlightX + squareWidth, highlightY + squareHeight);
		drawLine(highlightX + squareWidth, highlightY + squareHeight, highlightX, highlightY + squareHeight);
		drawLine(highlightX, highlightY + squareHeight, highlightX, highlightY);

		C3D_FrameEnd(0);
	}

	// Exit services
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
