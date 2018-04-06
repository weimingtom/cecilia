//20180406
#include "python.h"
#include <stdio.h>

#ifdef _MSC_VER
#include <windows.h>
#include <crtdbg.h>

#if USE_VC6_MEMORY_LEAK
#define __INSURE__ 1
#endif

#endif

#if defined ANDROID
#include <jni.h>
#include <stdlib.h>
#include <android/log.h>
#endif

extern DL_EXPORT(int) Py_Main(int, char **);

int main(int argc, char **argv)
{
	return Py_Main(argc, argv);
}
