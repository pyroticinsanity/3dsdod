/*
	Hello World example made by Aurelio Mannara for ctrulib
	This code was modified for the last time on: 12/13/2014 01:00 UTC+1

	This wouldn't be possible without the amazing work done by:
	-Smealum
	-fincs
	-WinterMute
	-yellows8
	-plutoo
	-mtheall
	-Many others who worked on 3DS and I'm surely forgetting about
*/

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <citro2d.h>
#include <citro3d.h>

#include "manual.h"


#include "dod01_t3x.h"
#include "dod02_t3x.h"
#include "dod03_t3x.h"
#include "dod04_t3x.h"
#include "dod05_t3x.h"
#include "dod06_t3x.h"
#include "dod07_t3x.h"
#include "dod08_t3x.h"
#include "dod09_t3x.h"
#include "dod10_t3x.h"
#include "dod11_t3x.h"
#include "dod12_t3x.h"
#include "dod13_t3x.h"
#include "dod14_t3x.h"
#include "dod15_t3x.h"
#include "dod16_t3x.h"
#include "dod17_t3x.h"
#include "dod18_t3x.h"
#include "dod19_t3x.h"
#include "dod20_t3x.h"
#include "dod21_t3x.h"
#include "dod22_t3x.h"
#include "dod23_t3x.h"
#include "dod24_t3x.h"
#include "dod25_t3x.h"
#include "dod26_t3x.h"
#include "dod27_t3x.h"
#include "dod28_t3x.h"
#include "dod29_t3x.h"
#include "dod30_t3x.h"
#include "dod31_t3x.h"
#include "dod32_t3x.h"
#include "dod33_t3x.h"
#include "dod34_t3x.h"
#include "dod35_t3x.h"
#include "dod36_t3x.h"
#include "dod37_t3x.h"
#include "dod38_t3x.h"

   struct Page
{
	const u8* t3x;
	const size_t size;
};

void loadImage(const u8* input, size_t insize, C3D_Tex* tex, C2D_Image* image)
{
	Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture();
	subtex->left=subtex->top=0; subtex->width=600; subtex->height=600;
	subtex->right=subtex->bottom=1;
	image->subtex = subtex;
	image->tex = tex;

   printf("Loading Image size: %d\n", insize);

	Tex3DS_Texture t3x = Tex3DS_TextureImport(input, insize, tex, NULL, false);

	C3D_TexSetFilter(tex, GPU_LINEAR, GPU_NEAREST);

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);
}

struct Page Pages[] =
{
        {dod01_t3x, dod01_t3x_size},
        {dod02_t3x, dod02_t3x_size},
        {dod03_t3x, dod03_t3x_size},
        {dod04_t3x, dod04_t3x_size},
        {dod05_t3x, dod05_t3x_size},
        {dod06_t3x, dod06_t3x_size},
        {dod07_t3x, dod07_t3x_size},
        {dod08_t3x, dod08_t3x_size},
        {dod09_t3x, dod09_t3x_size},
        {dod10_t3x, dod10_t3x_size},
        {dod11_t3x, dod11_t3x_size},
        {dod12_t3x, dod12_t3x_size},
        {dod13_t3x, dod13_t3x_size},
        {dod14_t3x, dod14_t3x_size},
        {dod15_t3x, dod15_t3x_size},
        {dod16_t3x, dod16_t3x_size},
        {dod17_t3x, dod17_t3x_size},
        {dod18_t3x, dod18_t3x_size},
        {dod19_t3x, dod19_t3x_size},
        {dod20_t3x, dod20_t3x_size},
        {dod21_t3x, dod21_t3x_size},
        {dod22_t3x, dod22_t3x_size},
        {dod23_t3x, dod23_t3x_size},
        {dod24_t3x, dod24_t3x_size},
        {dod25_t3x, dod25_t3x_size},
        {dod26_t3x, dod26_t3x_size},
        {dod27_t3x, dod27_t3x_size},
        {dod28_t3x, dod28_t3x_size},
        {dod29_t3x, dod29_t3x_size},
        {dod30_t3x, dod30_t3x_size},
        {dod31_t3x, dod31_t3x_size},
        {dod32_t3x, dod32_t3x_size},
        {dod33_t3x, dod33_t3x_size},
        {dod34_t3x, dod34_t3x_size},
        {dod35_t3x, dod35_t3x_size},
        {dod36_t3x, dod36_t3x_size},
        {dod37_t3x, dod37_t3x_size},
        {dod38_t3x, dod38_t3x_size}
};

#define NUMBER_OF_PAGES 38

int main(int argc, char **argv)
{
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	consoleInit(GFX_BOTTOM, NULL);

	C2D_Image img;	
	C3D_Tex tex;

	loadImage(Pages[0].t3x, Pages[0].size, &tex, &img);

	u32 clrClear = C2D_Color32(255, 0, 0, 255);

	int position = 0;
	int xScroll = 0;
	int scroll = 0;
	float scale = 0.87;
	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		u32 hDown = hidKeysHeld();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		if(kDown & KEY_R and position < NUMBER_OF_PAGES - 1)
		{
			position++;
			C3D_TexDelete(&tex);
			loadImage(Pages[position].t3x, Pages[position].size, &tex, &img);

			scroll = 0;
		}

		if(kDown & KEY_L and position > 0)
		{
			position--;
			scroll = 0;
			C3D_TexDelete(&tex);
			loadImage(Pages[position].t3x, Pages[position].size, &tex, &img);
		}

		if(hDown & KEY_DDOWN and scroll > -160)
		{
			scroll-=5;
		}

		if(hDown & KEY_DUP and scroll < 0)
		{
			scroll+=5;
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		 C2D_DrawImageAt(img, -120, scroll, 0, NULL, scale, scale);

		printf("\x1b[1;1HPage: %d x: %d, y: %d, scale: %f", position, xScroll, scroll, scale);
		
		C3D_FrameEnd(0);
	}

	// Exit services
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
