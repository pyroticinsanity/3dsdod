/*
 * Example code for reading a set of images like a manual from the SD card at /3ds/3dsdod/manual.
*/

#include <3ds.h>
#include <stdio.h>
#include <citro2d.h>
#include <citro3d.h>

#include "manual.h"

int main(int argc, char **argv)
{
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screens
	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	consoleInit(GFX_BOTTOM, NULL);

	Manual manual("/3ds/3dsdod/manual");

	u32 clrClear = C2D_Color32(255, 0, 0, 255);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		u32 hDown = hidKeysHeld();

		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if (kDown & KEY_R)
		{
			manual.nextPage();
		}

		if (kDown & KEY_L)
		{
			manual.previousPage();
		}

		if (hDown & KEY_DDOWN)
		{
			manual.scrollDown();
		}

		if (hDown & KEY_DUP)
		{
			manual.scrollUp();
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);

		manual.drawPage(top);

		C3D_FrameEnd(0);
	}

	// Exit services
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
