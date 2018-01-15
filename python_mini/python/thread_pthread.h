//20180115
#include <stdlib.h>
#include <string.h>
#if defined(__APPLE__) || defined(HAVE_PTHREAD_DESTRUCTOR)
#define destructor xxdestructor
#endif
#include <pthread.h>
#if defined(__APPLE__) || defined(HAVE_PTHREAD_DESTRUCTOR)
#undef destructor
#endif
#include <signal.h>

#if defined(__ultrix) && defined(__mips) && defined(_DECTHREADS_)
#  define PY_PTHREAD_D4

#elif defined(__osf__) && defined (__alpha)
#  if !defined(_PTHREAD_ENV_ALPHA) || defined(_PTHREAD_USE_D4) || defined(PTHREAD_USE_D4)
#    define PY_PTHREAD_D4
#  else
#    define PY_PTHREAD_STD
#  endif

#elif defined(_AIX)
#  if !defined(SCHED_BG_NP)
#    define PY_PTHREAD_STD
#  else
#    define PY_PTHREAD_D7
#  endif

#elif defined(__DGUX)
#  define PY_PTHREAD_D6

#elif defined(__hpux) && defined(_DECTHREADS_)
#  define PY_PTHREAD_D4

#else
#  define PY_PTHREAD_STD

#endif

#ifdef USE_GUSI
#define THREAD_STACK_SIZE 0x10000
#endif

#if defined(PY_PTHREAD_D4) || defined(PY_PTHREAD_D7)
#  define pthread_attr_default pthread_attr_default
#  define pthread_mutexattr_default pthread_mutexattr_default
#  define pthread_condattr_default pthread_condattr_default
#elif defined(PY_PTHREAD_STD) || defined(PY_PTHREAD_D6)
#  define pthread_attr_default ((pthread_attr_t *)NULL)
#  define pthread_mutexattr_default ((pthread_mutexattr_t *)NULL)
#  define pthread_condattr_default ((pthread_condattr_t *)NULL)
#endif

#ifdef HAVE_PTHREAD_SIGMASK
#  define SET_THREAD_SIGMASK pthread_sigmask
#else
#  define SET_THREAD_SIGMASK sigprocmask
#endif

typedef struct {
	char             locked;
	pthread_cond_t   lock_released;
	pthread_mutex_t  mut;
} pthread_lock;

#define CHECK_STATUS(name)  if (status != 0) { perror(name); error = 1; }

#ifdef _HAVE_BSDI
static
void _noop(void)
{
}

static void
PyThread__init_thread(void)
{
	static int dummy = 0;
	pthread_t thread1;
	pthread_create(&thread1, NULL, (void *) _noop, &dummy);
	pthread_join(thread1, NULL);
}

#else

static void
PyThread__init_thread(void)
{
#if defined(_AIX) && defined(__GNUC__)
	pthread_init();
#endif
}

#endif

long PyThread_start_new_thread(void (*func)(void *), void *arg)
{
	pthread_t th;
	int success;
 	sigset_t oldmask, newmask;
#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
	pthread_attr_t attrs;
#endif
	dprintf(("PyThread_start_new_thread called\n"));
	if (!initialized)
	{
		PyThread_init_thread();
	}

#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
	pthread_attr_init(&attrs);
#endif
#ifdef THREAD_STACK_SIZE
	pthread_attr_setstacksize(&attrs, THREAD_STACK_SIZE);
#endif
#ifdef PTHREAD_SYSTEM_SCHED_SUPPORTED
        pthread_attr_setscope(&attrs, PTHREAD_SCOPE_SYSTEM);
#endif

	sigfillset(&newmask);
	SET_THREAD_SIGMASK(SIG_BLOCK, &newmask, &oldmask);

	success = pthread_create(&th, 
#if defined(PY_PTHREAD_D4)
				 pthread_attr_default,
				 (pthread_startroutine_t)func, 
				 (pthread_addr_t)arg
#elif defined(PY_PTHREAD_D6)
				 pthread_attr_default,
				 (void* (*)(void *))func,
				 arg
#elif defined(PY_PTHREAD_D7)
				 pthread_attr_default,
				 func,
				 arg
#elif defined(PY_PTHREAD_STD)
#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
				 &attrs,
#else
				 (pthread_attr_t*)NULL,
#endif
				 (void* (*)(void *))func,
				 (void *)arg
#endif
				 );

	SET_THREAD_SIGMASK(SIG_SETMASK, &oldmask, NULL);

#if defined(THREAD_STACK_SIZE) || defined(PTHREAD_SYSTEM_SCHED_SUPPORTED)
	pthread_attr_destroy(&attrs);
#endif
	if (success == 0) {
#if defined(PY_PTHREAD_D4) || defined(PY_PTHREAD_D6) || defined(PY_PTHREAD_D7)
		pthread_detach(&th);
#elif defined(PY_PTHREAD_STD)
		pthread_detach(th);
#endif
	}
#if SIZEOF_PTHREAD_T <= SIZEOF_LONG
	return (long) th;
#else
	return (long) *(long *) &th;
#endif
}

long PyThread_get_thread_ident()
{
	volatile pthread_t threadid;
	if (!initialized)
	{
		PyThread_init_thread();
	}
	threadid = pthread_self();
#if SIZEOF_PTHREAD_T <= SIZEOF_LONG
	return (long) threadid;
#else
	return (long) *(long *) &threadid;
#endif
}

static void do_PyThread_exit_thread(int no_cleanup)
{
	dprintf(("PyThread_exit_thread called\n"));
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
}

void PyThread_exit_thread()
{
	do_PyThread_exit_thread(0);
}

void PyThread__exit_thread()
{
	do_PyThread_exit_thread(1);
}

#ifndef NO_EXIT_PROG
static void do_PyThread_exit_prog(int status, int no_cleanup)
{
	dprintf(("PyThread_exit_prog(%d) called\n", status));
	if (!initialized)
	{
		if (no_cleanup)
		{
			_exit(status);
		}
		else
		{
			exit(status);
		}
	}
}

void PyThread_exit_prog(int status)
{
	do_PyThread_exit_prog(status, 0);
}

void PyThread__exit_prog(int status)
{
	do_PyThread_exit_prog(status, 1);
}

#endif

PyThread_type_lock PyThread_allocate_lock(void)
{
	pthread_lock *lock;
	int status, error = 0;

	dprintf(("PyThread_allocate_lock called\n"));
	if (!initialized)
	{
		PyThread_init_thread();
	}

	lock = (pthread_lock *) malloc(sizeof(pthread_lock));
	memset((void *)lock, '\0', sizeof(pthread_lock));
	if (lock) 
	{
		lock->locked = 0;
		status = pthread_mutex_init(&lock->mut,
					    pthread_mutexattr_default);
		CHECK_STATUS("pthread_mutex_init");
		status = pthread_cond_init(&lock->lock_released,
					   pthread_condattr_default);
		CHECK_STATUS("pthread_cond_init");
		if (error) 
		{
			free((void *)lock);
			lock = 0;
		}
	}
	dprintf(("PyThread_allocate_lock() -> %p\n", lock));
	return (PyThread_type_lock) lock;
}

void PyThread_free_lock(PyThread_type_lock lock)
{
	pthread_lock *thelock = (pthread_lock *)lock;
	int status, error = 0;
	
	dprintf(("PyThread_free_lock(%p) called\n", lock));

	status = pthread_mutex_destroy( &thelock->mut );
	CHECK_STATUS("pthread_mutex_destroy");

	status = pthread_cond_destroy( &thelock->lock_released );
	CHECK_STATUS("pthread_cond_destroy");

	free((void *)thelock);
}

int PyThread_acquire_lock(PyThread_type_lock lock, int waitflag)
{
	int success;
	pthread_lock *thelock = (pthread_lock *)lock;
	int status, error = 0;

	dprintf(("PyThread_acquire_lock(%p, %d) called\n", lock, waitflag));

	status = pthread_mutex_lock( &thelock->mut );
	CHECK_STATUS("pthread_mutex_lock[1]");
	success = thelock->locked == 0;
	if (success) thelock->locked = 1;
	status = pthread_mutex_unlock( &thelock->mut );
	CHECK_STATUS("pthread_mutex_unlock[1]");

	if ( !success && waitflag ) 
	{
		status = pthread_mutex_lock( &thelock->mut );
		CHECK_STATUS("pthread_mutex_lock[2]");
		while ( thelock->locked ) 
		{
			status = pthread_cond_wait(&thelock->lock_released,
						   &thelock->mut);
			CHECK_STATUS("pthread_cond_wait");
		}
		thelock->locked = 1;
		status = pthread_mutex_unlock( &thelock->mut );
		CHECK_STATUS("pthread_mutex_unlock[2]");
		success = 1;
	}
	if (error) 
	{
		success = 0;
	}
	dprintf(("PyThread_acquire_lock(%p, %d) -> %d\n", lock, waitflag, success));
	return success;
}

void PyThread_release_lock(PyThread_type_lock lock)
{
	pthread_lock *thelock = (pthread_lock *)lock;
	int status, error = 0;

	dprintf(("PyThread_release_lock(%p) called\n", lock));

	status = pthread_mutex_lock( &thelock->mut );
	CHECK_STATUS("pthread_mutex_lock[3]");

	thelock->locked = 0;

	status = pthread_mutex_unlock( &thelock->mut );
	CHECK_STATUS("pthread_mutex_unlock[3]");

	status = pthread_cond_signal( &thelock->lock_released );
	CHECK_STATUS("pthread_cond_signal");
}

struct semaphore {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int value;
};

PyThread_type_sema PyThread_allocate_sema(int value)
{
	struct semaphore *sema;
	int status, error = 0;

	dprintf(("PyThread_allocate_sema called\n"));
	if (!initialized)
	{
		PyThread_init_thread();
	}

	sema = (struct semaphore *) malloc(sizeof(struct semaphore));
	if (sema != NULL) 
	{
		sema->value = value;
		status = pthread_mutex_init(&sema->mutex,
					    pthread_mutexattr_default);
		CHECK_STATUS("pthread_mutex_init");
		status = pthread_cond_init(&sema->cond,
					   pthread_condattr_default);
		CHECK_STATUS("pthread_cond_init");
		if (error) 
		{
			free((void *) sema);
			sema = NULL;
		}
	}
	dprintf(("PyThread_allocate_sema() -> %p\n",  sema));
	return (PyThread_type_sema) sema;
}

void PyThread_free_sema(PyThread_type_sema sema)
{
	int status, error = 0;
	struct semaphore *thesema = (struct semaphore *) sema;

	dprintf(("PyThread_free_sema(%p) called\n",  sema));
	status = pthread_cond_destroy(&thesema->cond);
	CHECK_STATUS("pthread_cond_destroy");
	status = pthread_mutex_destroy(&thesema->mutex);
	CHECK_STATUS("pthread_mutex_destroy");
	free((void *) thesema);
}

int PyThread_down_sema(PyThread_type_sema sema, int waitflag)
{
	int status, error = 0, success;
	struct semaphore *thesema = (struct semaphore *) sema;

	dprintf(("PyThread_down_sema(%p, %d) called\n",  sema, waitflag));
	status = pthread_mutex_lock(&thesema->mutex);
	CHECK_STATUS("pthread_mutex_lock");
	if (waitflag) 
	{
		while (!error && thesema->value <= 0) 
		{
			status = pthread_cond_wait(&thesema->cond,
						   &thesema->mutex);
			CHECK_STATUS("pthread_cond_wait");
		}
	}
	if (error)
	{
		success = 0;
	}
	else if (thesema->value > 0) 
	{
		thesema->value--;
		success = 1;
	}
	else
	{
		success = 0;
	}
	status = pthread_mutex_unlock(&thesema->mutex);
	CHECK_STATUS("pthread_mutex_unlock");
	dprintf(("PyThread_down_sema(%p) return\n",  sema));
	return success;
}

void PyThread_up_sema(PyThread_type_sema sema)
{
	int status, error = 0;
	struct semaphore *thesema = (struct semaphore *) sema;

	dprintf(("PyThread_up_sema(%p)\n",  sema));
	status = pthread_mutex_lock(&thesema->mutex);
	CHECK_STATUS("pthread_mutex_lock");
	thesema->value++;
	status = pthread_cond_signal(&thesema->cond);
	CHECK_STATUS("pthread_cond_signal");
	status = pthread_mutex_unlock(&thesema->mutex);
	CHECK_STATUS("pthread_mutex_unlock");
}
