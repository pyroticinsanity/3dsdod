// Example of rendering the Dungeons of Daggorath Wizard using lines.
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define VCNTRX 200
#define VCNTRY 120

u8 VYSCAL=0x80;
float VYSCALf=128.0f;
u8 VXSCAL= 0x80;
float VXSCALf=128.0f;

// Scales X-coordinate
int ScaleX(int x)
{
	return ((x-VCNTRX)*VXSCAL)/127;
}

float ScaleXf(float x)
{
	return ((x-(float)VCNTRX)*VXSCALf)/127.0f;
}

float ScaleYf(float y)
{
	return ((y-VCNTRY)*VYSCALf)/127.0f;
}

// Scales Y-coordinate
int ScaleY(int y)
{
	return ((y-VCNTRY)*VYSCAL)/127;
}

void drawLine(float X0, float Y0, float X1, float Y1)
{
			u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
			double width = 0.5;

			double slope = (X1 - X0) / (Y1 - Y0);
			double yMod = width * sin(atan(slope));
			double xMod = width * cos(atan(slope));

			C2D_DrawTriangle(X0 - xMod, Y0 + yMod, clrWhite, 
				X0 + xMod, Y0 - yMod, clrWhite,
				X1 - xMod, Y1 + yMod, clrWhite, 0);

			C2D_DrawTriangle(X1 - xMod, Y1 + yMod, clrWhite, 
				X0 + xMod, Y0 - yMod, clrWhite,
				X1 + xMod, Y1 - yMod, clrWhite, 0);

}

void drawVectorList(int VLA[], int xOffset, int yOffset)
{
	int numLists = VLA[0];
	int curList = 0;
	int numVertices;
	int curVertex;
	int	ctr = 1;

	while (curList < numLists)
	{
		numVertices = VLA[ctr];
		++ctr;
		curVertex = 0;
		while (curVertex < (numVertices - 1) )
		{
			float x0, y0, x1, y1;
			x0 = ScaleXf((float)VLA[ctr]) + (float)VCNTRX;
			y0 = ScaleYf((float)VLA[ctr+1]) + (float)VCNTRY;
			x1 = ScaleXf((float)VLA[ctr+2]) + (float)VCNTRX;
			y1 = ScaleYf((float)VLA[ctr+3]) + (float)VCNTRY;

			float X0 = x0 + xOffset;
			float Y0 = y0 + yOffset;
			float X1 = x1 + xOffset;
			float Y1 = y1 + yOffset;

			u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
			double width = 0.5;

			double slope = (X1 - X0) / (Y1 - Y0);
			double yMod = width * sin(atan(slope));
			double xMod = width * cos(atan(slope));

			C2D_DrawTriangle(X0 - xMod, Y0 + yMod, clrWhite, 
				X0 + xMod, Y0 - yMod, clrWhite,
				X1 - xMod, Y1 + yMod, clrWhite, 0);

			C2D_DrawTriangle(X1 - xMod, Y1 + yMod, clrWhite, 
				X0 + xMod, Y0 - yMod, clrWhite,
				X1 + xMod, Y1 - yMod, clrWhite, 0);

			ctr += 2;
			++curVertex;
		}
		++curList;
		ctr += 2;
	}
}

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
			int result = (int) strtoul(hexbuf, &end, 16);
			*(b + ctr) = result;

			printf( "[%d] %s, %d\n", ctr, hexbuf, result);
			++ctr;
			h += 2;
		}
	}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	gfxSet3D(true);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* topRight = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);

	u32 clrClear = C2D_Color32(0x00, 0x00, 0x00, 0x00);

	int W1_VLA[199];

	LoadFromHex(W1_VLA,
		"0A0D7C40784878406E4E66406E3A763678327A2E74286C2A781C7E1C08821C881C9224"
		"9432882C842E8630882C0986308E36A474848476825E786E5A84846A4805664064425E"
		"38603666400566426244664A684C6E5A027058784809843E80147A347A407C3C807282"
		"508244843E0B822880267C287E2A8228802E80327E327E2A7C287E2E10622E64326236"
		"5C3A5638523056285A265E285C28582A563058345C366232622E109A689C6C9A709474"
		"8E728A6A8E6292609662946290648E6A926E96709A6C9A68");

	int xOffset = 0;
	int yOffset = 0;
	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		u32 hDown = hidKeysHeld();

		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if(hDown & KEY_A)
		{
			xOffset++;
		}

		if(hDown & KEY_B)
		{
			xOffset--;
		}

		if(hDown & KEY_X)
		{
			yOffset++;
		}

		if(hDown & KEY_Y)
		{
			yOffset--;
		}

		printf("\x1b[1;1HxOffset: %d, yOffset: %d", xOffset, yOffset);
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

			C2D_TargetClear(top, clrClear);
			C2D_SceneBegin(top);
			drawVectorList(W1_VLA, -xOffset, -yOffset);

			C2D_SceneBegin(topRight);
			C2D_TargetClear(topRight, clrClear);
			drawVectorList(W1_VLA, xOffset, yOffset);

		C3D_FrameEnd(0);

		gfxSwapBuffers();
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}




