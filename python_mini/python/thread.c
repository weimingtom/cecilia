//20180114
#include "python.h"
#include <stdio.h>
#include <stdlib.h>
#include "pythread.h"

#ifdef _DEBUG
static int thread_debug = 0;
#define dprintf(args)	((thread_debug & 1) && printf args)
#define d2printf(args)	((thread_debug & 8) && printf args)
#else
#define dprintf(args)
#define d2printf(args)
#endif

static int initialized;

static void PyThread__init_thread();

void PyThread_init_thread()
{
#ifdef _DEBUG
	char *p = getenv("THREADDEBUG");

	if (p) 
	{
		if (*p)
		{
			thread_debug = atoi(p);
		}
		else
		{
			thread_debug = 1;
		}
	}
#endif
	if (initialized)
	{
		return;
	}
	initialized = 1;
	dprintf(("PyThread_init_thread called\n"));
	PyThread__init_thread();
}

#include "thread_nt.h"
