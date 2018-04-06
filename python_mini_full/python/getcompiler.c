//20180118
#include "python.h"

#ifndef COMPILER

#ifdef __GNUC__
#define COMPILER "\n[GCC " __VERSION__ "]"
#endif

#endif

#ifndef COMPILER

#ifdef __cplusplus
#define COMPILER "[C++]"
#else
#define COMPILER "[C]"
#endif

#endif

const char *Py_GetCompiler()
{
	return COMPILER;
}
