//20180118
#include "python.h"

#ifndef PLATFORM
#define PLATFORM "unknown"
#endif

const char *Py_GetPlatform()
{
	return PLATFORM;
}
