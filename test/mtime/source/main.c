// Comparison of getting write time from stat VS archive_getmtime

#include <3ds.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

	const char* filename = "/3ds/test.txt";

	FILE* file = fopen(filename, "w");

	if(file == NULL)
	{
		printf("Failed to open file\n");
		waitForExit();
	}

	fprintf(file, "Testing!\n");

	fclose(file);


	struct stat stats;
	struct tm* time;
	u64 mtime;
	char date[50];

	int error = stat(filename, &stats);

	if(error != 0)
	{
		printf("Failed to stat!\n");
		waitForExit();
	}

	error = archive_getmtime(filename, &mtime);

	if(error != 0)
	{
		printf("failed to get mtime\n");
		waitForExit();
	}

	time = localtime(&stats.st_mtime);
	strftime(date, sizeof(date), "%Y.%m.%d %I:%M %p", time);

	// piepie62 confirmed that stat does nothing to mtime
	// It appears that it won't be fixed either:
	// https://github.com/smealum/ctrulib/pull/458#issuecomment-631587673
	// https://github.com/devkitPro/newlib/issues/21
	printf("Stat: %s\n", date);

	stats.st_mtime = mtime;
	time = localtime(&stats.st_mtime);
	strftime(date, sizeof(date), "%Y.%m.%d %I:%M %p", time);
	
	printf("Archive_getmtime: %s\n", date);

	// Main loop
	waitForExit();

	// Deinit libs
	gfxExit();
	return 0;
}




