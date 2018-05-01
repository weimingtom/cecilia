//20180406
#include "python.h"
#include <stdio.h>

#include <windows.h>
#include <crtdbg.h>

extern DL_EXPORT(int) Py_Main(int, char **);

int main(int argc, char **argv)
{
	return Py_Main(argc, argv);
}
