//20170403

#include "python.h"
#ifdef MS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

int (*PyOS_InputHook)(void) = NULL;

#ifdef RISCOS
int Py_RISCOSWimpFlag;
#endif
static int my_fgets(char *buf, int len, FILE *fp)
{
	char *p;
	for (;;) 
	{
		if (PyOS_InputHook != NULL)
		{
			(void)(PyOS_InputHook)();
		}
		errno = 0;
		p = fgets(buf, len, fp);
		if (p != NULL)
		{
			return 0;
#ifdef MS_WINDOWS
		}
		if (GetLastError() == ERROR_OPERATION_ABORTED) 
		{
			Sleep(1);
			if (PyOS_InterruptOccurred()) {
				return 1;
			}
		}
#endif
		if (feof(fp)) 
		{
			return -1;
		}
#ifdef EINTR
		if (errno == EINTR) 
		{
			if (PyOS_InterruptOccurred()) 
			{
				return 1;
			}
			continue;
		}
#endif
		if (PyOS_InterruptOccurred()) 
		{
			return 1;
		}
		return -2;
	}
}

char *PyOS_StdioReadline(char *prompt)
{
	size_t n;
	char *p;

	n = 100;
	if ((p = PyMem_MALLOC(n)) == NULL)
	{
		return NULL;
	}
	fflush(stdout);
#ifndef RISCOS
	if (prompt)
	{
		fprintf(stderr, "%s", prompt);
	}
#else
	if (prompt) 
	{
		if (Py_RISCOSWimpFlag)
		{
			fprintf(stderr, "\x0cr%s\x0c", prompt);
		}
		else
		{
			fprintf(stderr, "%s", prompt);
		}
	}
#endif
	fflush(stderr);
	switch (my_fgets(p, (int)n, stdin)) 
	{
	case 0:
		break;
	
	case 1:
		PyMem_FREE(p);
		return NULL;
	
	case -1:
	case -2:
	default:
		*p = '\0';
		break;
	}
#ifdef MPW
	n = strlen(prompt);
	if (strncmp(p, prompt, n) == 0)
	{
		memmove(p, p + n, strlen(p) - n + 1);
	}
#endif
	n = strlen(p);
	while (n > 0 && p[n-1] != '\n') 
	{
		size_t incr = n+2;
		p = PyMem_REALLOC(p, n + incr);
		if (p == NULL)
		{
			return NULL;
		}
		if (incr > INT_MAX) 
		{
			PyErr_SetString(PyExc_OverflowError, "input line too long");
		}
		if (my_fgets(p+n, (int)incr, stdin) != 0)
		{
			break;
		}
		n += strlen(p+n);
	}
	return PyMem_REALLOC(p, n+1);
}

char *(*PyOS_ReadlineFunctionPointer)(char *);

char *PyOS_Readline(char *prompt)
{
	char *rv;
	if (PyOS_ReadlineFunctionPointer == NULL) 
	{
		PyOS_ReadlineFunctionPointer = PyOS_StdioReadline;
	}
	Py_BEGIN_ALLOW_THREADS
	rv = (*PyOS_ReadlineFunctionPointer)(prompt);
	Py_END_ALLOW_THREADS
	return rv;
}
