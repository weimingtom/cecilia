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





























#include <windows.h>
#include <limits.h>
#include <process.h>

typedef struct NRMUTEX {
	LONG   owned;
	DWORD  thread_id;
	HANDLE hevent;
} NRMUTEX, *PNRMUTEX;

typedef PVOID WINAPI interlocked_cmp_xchg_t(PVOID *dest, PVOID exc, PVOID comperand) ;

static PVOID WINAPI interlocked_cmp_xchg(PVOID *dest, PVOID exc, PVOID comperand)
{
	static LONG spinlock = 0 ;
	PVOID result ;
	DWORD dwSleep = 0;

	while (InterlockedExchange(&spinlock, 1))
	{
		Sleep(dwSleep);
		dwSleep = !dwSleep;
	}
	result = *dest;
	if (result == comperand)
	{
		*dest = exc;
	}
	spinlock = 0;
	return result;
}

static interlocked_cmp_xchg_t *ixchg;
BOOL InitializeNonRecursiveMutex(PNRMUTEX mutex)
{
	if (!ixchg)
	{
		HANDLE kernel = GetModuleHandle("kernel32.dll");
		if (!kernel || (ixchg = (interlocked_cmp_xchg_t *)GetProcAddress(kernel, "InterlockedCompareExchange")) == NULL)
		{
			ixchg = interlocked_cmp_xchg;
		}
	}

	mutex->owned = -1; 
	mutex->thread_id = 0;
	mutex->hevent = CreateEvent(NULL, FALSE, FALSE, NULL);
	return mutex->hevent != NULL;
}

#define InterlockedCompareExchange(dest,exchange,comperand) (ixchg((dest), (exchange), (comperand)))

VOID DeleteNonRecursiveMutex(PNRMUTEX mutex)
{
	CloseHandle(mutex->hevent);
	mutex->hevent = NULL;
}

DWORD EnterNonRecursiveMutex(PNRMUTEX mutex, BOOL wait)
{
	DWORD ret;

	if (!wait)
	{
		if (InterlockedCompareExchange((PVOID *)&mutex->owned, (PVOID)0, (PVOID)-1) != (PVOID)-1)
		{
			return WAIT_TIMEOUT;
		}
		ret = WAIT_OBJECT_0;
	}
	else
	{
		ret = InterlockedIncrement(&mutex->owned) ?
			WaitForSingleObject(mutex->hevent, INFINITE) : WAIT_OBJECT_0;
	}

	mutex->thread_id = GetCurrentThreadId();
	return ret ;
}

BOOL LeaveNonRecursiveMutex(PNRMUTEX mutex)
{
	mutex->thread_id = 0 ;
	return
		InterlockedDecrement(&mutex->owned) < 0 ||
		SetEvent(mutex->hevent);
}

PNRMUTEX AllocNonRecursiveMutex()
{
	PNRMUTEX mutex = (PNRMUTEX)malloc(sizeof(NRMUTEX));
	if (mutex && !InitializeNonRecursiveMutex(mutex))
	{
		free(mutex);
		mutex = NULL;
	}
	return mutex ;
}

void FreeNonRecursiveMutex(PNRMUTEX mutex)
{
	if (mutex)
	{
		DeleteNonRecursiveMutex(mutex);
		free(mutex);
	}
}

long PyThread_get_thread_ident();

static void PyThread__init_thread()
{

}

typedef struct {
	void (*func)(void*);
	void *arg;			
	long id;
	HANDLE done;
} callobj;

static int bootstrap(void *call)
{
	callobj *obj = (callobj*)call;
	void (*func)(void*) = obj->func;
	void *arg = obj->arg;

	obj->id = PyThread_get_thread_ident();
	ReleaseSemaphore(obj->done, 1, NULL);
	func(arg);
	return 0;
}

long PyThread_start_new_thread(void (*func)(void *), void *arg)
{
	unsigned long rv;
	int success = 0;
	callobj *obj;
	int id;

	dprintf(("%ld: PyThread_start_new_thread called\n", PyThread_get_thread_ident()));
	if (!initialized)
	{
		PyThread_init_thread();
	}

	obj = malloc(sizeof(callobj)); 
	obj->func = func;
	obj->arg = arg;
	obj->done = CreateSemaphore(NULL, 0, 1, NULL);

	rv = _beginthread(bootstrap, 0, obj);
 
	if (rv != (unsigned long)-1) 
	{
		success = 1;
		dprintf(("%ld: PyThread_start_new_thread succeeded: %p\n", PyThread_get_thread_ident(), rv));
	}

	WaitForSingleObject(obj->done, 5000);
	CloseHandle((HANDLE)obj->done);
	id = obj->id;
	free(obj);
	return id;
}

long PyThread_get_thread_ident()
{
	if (!initialized)
	{
		PyThread_init_thread();
	}

	return GetCurrentThreadId();
}

static void do_PyThread_exit_thread(int no_cleanup)
{
	dprintf(("%ld: PyThread_exit_thread called\n", PyThread_get_thread_ident()));
	if (!initialized)
	{
		if (no_cleanup)
		{
			_exit(0);
		}
		else
		{
			exit(0);
		}
	}
	_endthread();
}

void PyThread_exit_thread()
{
	do_PyThread_exit_thread(0);
}

void PyThread__exit_thread()
{
	do_PyThread_exit_thread(1);
}

PyThread_type_lock PyThread_allocate_lock()
{
	PNRMUTEX aLock;

	dprintf(("PyThread_allocate_lock called\n"));
	if (!initialized)
	{
		PyThread_init_thread();
	}

	aLock = AllocNonRecursiveMutex() ;

	dprintf(("%ld: PyThread_allocate_lock() -> %p\n", PyThread_get_thread_ident(), aLock));

	return (PyThread_type_lock) aLock;
}

void PyThread_free_lock(PyThread_type_lock aLock)
{
	dprintf(("%ld: PyThread_free_lock(%p) called\n", PyThread_get_thread_ident(),aLock));

	FreeNonRecursiveMutex(aLock) ;
}

int PyThread_acquire_lock(PyThread_type_lock aLock, int waitflag)
{
	int success;
	dprintf(("%ld: PyThread_acquire_lock(%p, %d) called\n", PyThread_get_thread_ident(),aLock, waitflag));
	success = aLock && EnterNonRecursiveMutex((PNRMUTEX) aLock, (waitflag == 1 ? INFINITE : 0)) == WAIT_OBJECT_0;
	dprintf(("%ld: PyThread_acquire_lock(%p, %d) -> %d\n", PyThread_get_thread_ident(),aLock, waitflag, success));
	return success;
}

void PyThread_release_lock(PyThread_type_lock aLock)
{
	dprintf(("%ld: PyThread_release_lock(%p) called\n", PyThread_get_thread_ident(),aLock));
	if (!(aLock && LeaveNonRecursiveMutex((PNRMUTEX) aLock)))
	{
		dprintf(("%ld: Could not PyThread_release_lock(%p) error: %l\n", PyThread_get_thread_ident(), aLock, GetLastError()));
	}
}

PyThread_type_sema PyThread_allocate_sema(int value)
{
	HANDLE aSemaphore;

	dprintf(("%ld: PyThread_allocate_sema called\n", PyThread_get_thread_ident()));
	if (!initialized)
	{
		PyThread_init_thread();
	}

	aSemaphore = CreateSemaphore( NULL,
	                              value,
	                              INT_MAX,
	                              NULL);

	dprintf(("%ld: PyThread_allocate_sema() -> %p\n", PyThread_get_thread_ident(), aSemaphore));

	return (PyThread_type_sema) aSemaphore;
}

void PyThread_free_sema(PyThread_type_sema aSemaphore)
{
	dprintf(("%ld: PyThread_free_sema(%p) called\n", PyThread_get_thread_ident(), aSemaphore));
	CloseHandle((HANDLE) aSemaphore);
}

int PyThread_down_sema(PyThread_type_sema aSemaphore, int waitflag)
{
	DWORD waitResult;
	dprintf(("%ld: PyThread_down_sema(%p) called\n", PyThread_get_thread_ident(), aSemaphore));
	waitResult = WaitForSingleObject( (HANDLE) aSemaphore, INFINITE);
	dprintf(("%ld: PyThread_down_sema(%p) return: %l\n", PyThread_get_thread_ident(), aSemaphore, waitResult));
	return 0;
}

void PyThread_up_sema(PyThread_type_sema aSemaphore)
{
	ReleaseSemaphore(
                (HANDLE) aSemaphore,
                1,
                NULL);                          
	dprintf(("%ld: PyThread_up_sema(%p)\n", PyThread_get_thread_ident(), aSemaphore));
}

