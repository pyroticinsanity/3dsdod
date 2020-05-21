// Testing threads for handling aptMainLoop

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool RunThread = true;
Thread threadHandle;

void threadMain(void* arg)
{
	while(aptMainLoop() && RunThread)
	{
		printf("Checking\n");
		svcSleepThread(100000000);
	}
}

void waitForExit()
{
	printf("Press START to Exit\n");
	while (true)
	{
		svcSleepThread(100000000);
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

	}
}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);

	threadHandle = threadCreate(threadMain, 0, 4 * 1024, 0x19, -2, false);

	// Main loop
	waitForExit();

	RunThread = false;
    threadJoin(threadHandle, U64_MAX);
    threadFree(threadHandle);

	// Deinit libs
	gfxExit();
	return 0;
}