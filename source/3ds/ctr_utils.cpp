
#include <3ds.h>

#include "ctr_utils.h"

void CtrUtils::ScanInput()
{
	aptMainLoop();
	hidScanInput();
	svcSleepThread(1);

}