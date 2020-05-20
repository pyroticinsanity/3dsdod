// Comparison of getting write time from stat VS archive_getmtime

#include <3ds.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

aptHookCookie Cookie;

void aptHookFunc(APT_HookType hookType, void* params)
{
	switch(hookType)
	{
		case APTHOOK_ONSUSPEND:
			printf("On Suspend!\n");
			break;
		case APTHOOK_ONSLEEP:
			printf("Sleep!\n");
			break;
		case APTHOOK_ONRESTORE:
			printf("Restore!\n");
			break;
		case APTHOOK_ONWAKEUP:
			printf("Wake Up!\n");
			break;
		case APTHOOK_ONEXIT:
			printf("Exit!\n");
			break;
		default:
			break;
	}
}

void waitForExit()
{
	printf("Press START to Exit\n");
	while (aptMainLoop())
	{
		svcSleepThread(1);
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

	aptHook(&Cookie, aptHookFunc, NULL);

	// Main loop
	waitForExit();

	// Deinit libs
	gfxExit();
	return 0;
}




