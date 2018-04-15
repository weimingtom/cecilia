//20180406
#include "python.h"
#include "intrcheck.h"

#include <process.h>

#include <signal.h>

#ifndef SIG_ERR
#define SIG_ERR ((PyOS_sighandler_t)(-1))
#endif

#if defined(PYOS_OS2)
#define NSIG 12
#include <process.h>
#endif

#ifndef NSIG
# if defined(_NSIG)
#  define NSIG _NSIG		
# elif defined(_SIGMAX)
#  define NSIG (_SIGMAX + 1)
# elif defined(SIGMAX)
#  define NSIG (SIGMAX + 1)
# else
#  define NSIG 64
# endif
#endif

#include <sys/types.h>
#include "pythread.h"
static long main_thread;
static pid_t main_pid;

static struct {
    int tripped;
    PyObject *func;
} Handlers[NSIG];

static int is_tripped = 0;

static PyObject *DefaultHandler;
static PyObject *IgnoreHandler;
static PyObject *IntHandler;

static PyOS_sighandler_t old_siginthandler = SIG_DFL;


static PyObject *signal_default_int_handler(PyObject *self, PyObject *args)
{
	PyErr_SetNone(PyExc_KeyboardInterrupt);
	return NULL;
}

static char default_int_handler_doc[] =
	"default_int_handler(...)\n"
	"\n"
	"The default handler for SIGINT instated by Python.\n"
	"It raises KeyboardInterrupt.";


static int checksignals_witharg(void * unused)
{
	return PyErr_CheckSignals();
}

static void signal_handler(int sig_num)
{
#ifdef WITH_PTH
	if (PyThread_get_thread_ident() != main_thread) 
	{
		pth_raise(*(pth_t *) main_thread, sig_num);
		return;
	}
#endif
	if (getpid() == main_pid) 
	{
		is_tripped++;
		Handlers[sig_num].tripped = 1;
		Py_AddPendingCall(checksignals_witharg, NULL);
	}
#ifdef SIGCHLD
	if (sig_num == SIGCHLD) 
	{
		return;
	}
#endif
#ifdef HAVE_SIGINTERRUPT
	siginterrupt(sig_num, 1);
#endif
	PyOS_setsig(sig_num, signal_handler);
}

#ifdef HAVE_ALARM
static PyObject *signal_alarm(PyObject *self, PyObject *args)
{
	int t;
	if (!PyArg_Parse(args, "i", &t))
	{
		return NULL;
	}
	return PyInt_FromLong((long)alarm(t));
}

static char alarm_doc[] =
	"alarm(seconds)\n"
	"\n"
	"Arrange for SIGALRM to arrive after the given number of seconds.";
#endif

#ifdef HAVE_PAUSE
static PyObject *signal_pause(PyObject *self, PyObject *args)
{
	if (!PyArg_NoArgs(args))
	{
		return NULL;
	}

	Py_BEGIN_ALLOW_THREADS
	pause();
	Py_END_ALLOW_THREADS
	if (PyErr_CheckSignals())
	{
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static char pause_doc[] =
	"pause()\n"
	"\n"
	"Wait until a signal arrives.";

#endif


static PyObject *signal_signal(PyObject *self, PyObject *args)
{
	PyObject *obj;
	int sig_num;
	PyObject *old_handler;
	void (*func)(int);
	if (!PyArg_Parse(args, "(iO)", &sig_num, &obj))
	{
		return NULL;
	}
	if (PyThread_get_thread_ident() != main_thread) 
	{
		PyErr_SetString(PyExc_ValueError,
				"signal only works in main thread");
		return NULL;
	}
	if (sig_num < 1 || sig_num >= NSIG) 
	{
		PyErr_SetString(PyExc_ValueError,
				"signal number out of range");
		return NULL;
	}
	if (obj == IgnoreHandler)
	{
		func = SIG_IGN;
	}
	else if (obj == DefaultHandler)
	{
		func = SIG_DFL;
	}
	else if (!PyCallable_Check(obj)) 
	{
		PyErr_SetString(PyExc_TypeError,
			"signal handler must be signal.SIG_IGN, signal.SIG_DFL, or a callable object");
		return NULL;
	}
	else
	{
		func = signal_handler;
	}
#ifdef HAVE_SIGINTERRUPT
	siginterrupt(sig_num, 1);
#endif
	if (PyOS_setsig(sig_num, func) == SIG_ERR) 
	{
		PyErr_SetFromErrno(PyExc_RuntimeError);
		return NULL;
	}
	old_handler = Handlers[sig_num].func;
	Handlers[sig_num].tripped = 0;
	Py_INCREF(obj);
	Handlers[sig_num].func = obj;
	return old_handler;
}

static char signal_doc[] =
	"signal(sig, action) -> action\n"
	"\n"
	"Set the action for the given signal.  The action can be SIG_DFL,\n"
	"SIG_IGN, or a callable Python object.  The previous action is\n"
	"returned.  See getsignal() for possible return values.\n"
	"\n"
	"*** IMPORTANT NOTICE ***\n"
	"A signal handler function is called with two arguments:\n"
	"the first is the signal number, the second is the interrupted stack frame.";


static PyObject *signal_getsignal(PyObject *self, PyObject *args)
{
	int sig_num;
	PyObject *old_handler;
	if (!PyArg_Parse(args, "i", &sig_num))
	{
		return NULL;
	}
	if (sig_num < 1 || sig_num >= NSIG) 
	{
		PyErr_SetString(PyExc_ValueError,
				"signal number out of range");
		return NULL;
	}
	old_handler = Handlers[sig_num].func;
	Py_INCREF(old_handler);
	return old_handler;
}

static char getsignal_doc[] =
	"getsignal(sig) -> action\n"
	"\n"
	"Return the current action for the given signal.  The return value can be:\n"
	"SIG_IGN -- if the signal is being ignored\n"
	"SIG_DFL -- if the default action for the signal is in effect\n"
	"None -- if an unknown handler is in effect\n"
	"anything else -- the callable Python object used as a handler\n"
	"";


static PyMethodDef signal_methods[] = {
#ifdef HAVE_ALARM
	{"alarm",	        signal_alarm, METH_OLDARGS, alarm_doc},
#endif
	{"signal",	        signal_signal, METH_OLDARGS, signal_doc},
	{"getsignal",	        signal_getsignal, METH_OLDARGS, getsignal_doc},
#ifdef HAVE_PAUSE
	{"pause",	        signal_pause, METH_OLDARGS, pause_doc},
#endif
	{"default_int_handler", signal_default_int_handler, 
	 METH_OLDARGS, default_int_handler_doc},
	{NULL,			NULL}
};


static char module_doc[] =
	"This module provides mechanisms to use signal handlers in Python.\n"
	"\n"
	"Functions:\n"
	"\n"
	"alarm() -- cause SIGALRM after a specified time [Unix only]\n"
	"signal() -- set the action for a given signal\n"
	"getsignal() -- get the signal action for a given signal\n"
	"pause() -- wait until a signal arrives [Unix only]\n"
	"default_int_handler() -- default SIGINT handler\n"
	"\n"
	"Constants:\n"
	"\n"
	"SIG_DFL -- used to refer to the system default handler\n"
	"SIG_IGN -- used to ignore the signal\n"
	"NSIG -- number of defined signals\n"
	"\n"
	"SIGINT, SIGTERM, etc. -- signal numbers\n"
	"\n"
	"*** IMPORTANT NOTICE ***\n"
	"A signal handler function is called with two arguments:\n"
	"the first is the signal number, the second is the interrupted stack frame.";

DL_EXPORT(void) initsignal()
{
	PyObject *m, *d, *x;
	int i;

	main_thread = PyThread_get_thread_ident();
	main_pid = getpid();

	m = Py_InitModule3("signal", signal_methods, module_doc);

	d = PyModule_GetDict(m);

	x = DefaultHandler = PyLong_FromVoidPtr((void *)SIG_DFL);
    if (!x || PyDict_SetItemString(d, "SIG_DFL", x) < 0)
    {
		goto finally;
	}

	x = IgnoreHandler = PyLong_FromVoidPtr((void *)SIG_IGN);
    if (!x || PyDict_SetItemString(d, "SIG_IGN", x) < 0)
	{
		goto finally;
	}

    x = PyInt_FromLong((long)NSIG);
    if (!x || PyDict_SetItemString(d, "NSIG", x) < 0)
	{
		goto finally;
    }
	Py_DECREF(x);

	x = IntHandler = PyDict_GetItemString(d, "default_int_handler");
    if (!x)
    {
        goto finally;
	}
	Py_INCREF(IntHandler);

	Handlers[0].tripped = 0;
	for (i = 1; i < NSIG; i++) 
	{
		void (*t)(int);
		t = PyOS_getsig(i);
		Handlers[i].tripped = 0;
		if (t == SIG_DFL)
		{
			Handlers[i].func = DefaultHandler;
		}
		else if (t == SIG_IGN)
		{
			Handlers[i].func = IgnoreHandler;
		}
		else
		{
			Handlers[i].func = Py_None;
		}
		Py_INCREF(Handlers[i].func);
	}
	if (Handlers[SIGINT].func == DefaultHandler) 
	{
		Py_INCREF(IntHandler);
		Py_DECREF(Handlers[SIGINT].func);
		Handlers[SIGINT].func = IntHandler;
		old_siginthandler = PyOS_setsig(SIGINT, signal_handler);
	}

#ifdef SIGHUP
	x = PyInt_FromLong(SIGHUP);
	PyDict_SetItemString(d, "SIGHUP", x);
    Py_XDECREF(x);
#endif
#ifdef SIGINT
	x = PyInt_FromLong(SIGINT);
	PyDict_SetItemString(d, "SIGINT", x);
    Py_XDECREF(x);
#endif
#ifdef SIGBREAK
	x = PyInt_FromLong(SIGBREAK);
	PyDict_SetItemString(d, "SIGBREAK", x);
    Py_XDECREF(x);
#endif
#ifdef SIGQUIT
	x = PyInt_FromLong(SIGQUIT);
	PyDict_SetItemString(d, "SIGQUIT", x);
    Py_XDECREF(x);
#endif
#ifdef SIGILL
	x = PyInt_FromLong(SIGILL);
	PyDict_SetItemString(d, "SIGILL", x);
    Py_XDECREF(x);
#endif
#ifdef SIGTRAP
	x = PyInt_FromLong(SIGTRAP);
	PyDict_SetItemString(d, "SIGTRAP", x);
    Py_XDECREF(x);
#endif
#ifdef SIGIOT
	x = PyInt_FromLong(SIGIOT);
	PyDict_SetItemString(d, "SIGIOT", x);
    Py_XDECREF(x);
#endif
#ifdef SIGABRT
	x = PyInt_FromLong(SIGABRT);
	PyDict_SetItemString(d, "SIGABRT", x);
    Py_XDECREF(x);
#endif
#ifdef SIGEMT
	x = PyInt_FromLong(SIGEMT);
	PyDict_SetItemString(d, "SIGEMT", x);
    Py_XDECREF(x);
#endif
#ifdef SIGFPE
	x = PyInt_FromLong(SIGFPE);
	PyDict_SetItemString(d, "SIGFPE", x);
    Py_XDECREF(x);
#endif
#ifdef SIGKILL
	x = PyInt_FromLong(SIGKILL);
	PyDict_SetItemString(d, "SIGKILL", x);
    Py_XDECREF(x);
#endif
#ifdef SIGBUS
	x = PyInt_FromLong(SIGBUS);
	PyDict_SetItemString(d, "SIGBUS", x);
    Py_XDECREF(x);
#endif
#ifdef SIGSEGV
	x = PyInt_FromLong(SIGSEGV);
	PyDict_SetItemString(d, "SIGSEGV", x);
    Py_XDECREF(x);
#endif
#ifdef SIGSYS
	x = PyInt_FromLong(SIGSYS);
	PyDict_SetItemString(d, "SIGSYS", x);
    Py_XDECREF(x);
#endif
#ifdef SIGPIPE
	x = PyInt_FromLong(SIGPIPE);
	PyDict_SetItemString(d, "SIGPIPE", x);
    Py_XDECREF(x);
#endif
#ifdef SIGALRM
	x = PyInt_FromLong(SIGALRM);
	PyDict_SetItemString(d, "SIGALRM", x);
    Py_XDECREF(x);
#endif
#ifdef SIGTERM
	x = PyInt_FromLong(SIGTERM);
	PyDict_SetItemString(d, "SIGTERM", x);
    Py_XDECREF(x);
#endif
#ifdef SIGUSR1
	x = PyInt_FromLong(SIGUSR1);
	PyDict_SetItemString(d, "SIGUSR1", x);
    Py_XDECREF(x);
#endif
#ifdef SIGUSR2
	x = PyInt_FromLong(SIGUSR2);
	PyDict_SetItemString(d, "SIGUSR2", x);
    Py_XDECREF(x);
#endif
#ifdef SIGCLD
	x = PyInt_FromLong(SIGCLD);
	PyDict_SetItemString(d, "SIGCLD", x);
    Py_XDECREF(x);
#endif
#ifdef SIGCHLD
	x = PyInt_FromLong(SIGCHLD);
	PyDict_SetItemString(d, "SIGCHLD", x);
    Py_XDECREF(x);
#endif
#ifdef SIGPWR
	x = PyInt_FromLong(SIGPWR);
	PyDict_SetItemString(d, "SIGPWR", x);
    Py_XDECREF(x);
#endif
#ifdef SIGIO
	x = PyInt_FromLong(SIGIO);
	PyDict_SetItemString(d, "SIGIO", x);
    Py_XDECREF(x);
#endif
#ifdef SIGURG
	x = PyInt_FromLong(SIGURG);
	PyDict_SetItemString(d, "SIGURG", x);
    Py_XDECREF(x);
#endif
#ifdef SIGWINCH
	x = PyInt_FromLong(SIGWINCH);
	PyDict_SetItemString(d, "SIGWINCH", x);
    Py_XDECREF(x);
#endif
#ifdef SIGPOLL
	x = PyInt_FromLong(SIGPOLL);
	PyDict_SetItemString(d, "SIGPOLL", x);
    Py_XDECREF(x);
#endif
#ifdef SIGSTOP
	x = PyInt_FromLong(SIGSTOP);
	PyDict_SetItemString(d, "SIGSTOP", x);
    Py_XDECREF(x);
#endif
#ifdef SIGTSTP
	x = PyInt_FromLong(SIGTSTP);
	PyDict_SetItemString(d, "SIGTSTP", x);
    Py_XDECREF(x);
#endif
#ifdef SIGCONT
	x = PyInt_FromLong(SIGCONT);
	PyDict_SetItemString(d, "SIGCONT", x);
    Py_XDECREF(x);
#endif
#ifdef SIGTTIN
	x = PyInt_FromLong(SIGTTIN);
	PyDict_SetItemString(d, "SIGTTIN", x);
    Py_XDECREF(x);
#endif
#ifdef SIGTTOU
	x = PyInt_FromLong(SIGTTOU);
	PyDict_SetItemString(d, "SIGTTOU", x);
    Py_XDECREF(x);
#endif
#ifdef SIGVTALRM
	x = PyInt_FromLong(SIGVTALRM);
	PyDict_SetItemString(d, "SIGVTALRM", x);
    Py_XDECREF(x);
#endif
#ifdef SIGPROF
	x = PyInt_FromLong(SIGPROF);
	PyDict_SetItemString(d, "SIGPROF", x);
    Py_XDECREF(x);
#endif
#ifdef SIGXCPU
	x = PyInt_FromLong(SIGXCPU);
	PyDict_SetItemString(d, "SIGXCPU", x);
    Py_XDECREF(x);
#endif
#ifdef SIGXFSZ
	x = PyInt_FromLong(SIGXFSZ);
	PyDict_SetItemString(d, "SIGXFSZ", x);
    Py_XDECREF(x);
#endif
    if (!PyErr_Occurred())
	{
		return;
	}

finally:
	return;
}

static void finisignal()
{
	int i;
	PyObject *func;

	PyOS_setsig(SIGINT, old_siginthandler);
	old_siginthandler = SIG_DFL;

	for (i = 1; i < NSIG; i++) 
	{
		func = Handlers[i].func;
		Handlers[i].tripped = 0;
		Handlers[i].func = NULL;
		if (i != SIGINT && func != NULL && func != Py_None &&
		    func != DefaultHandler && func != IgnoreHandler)
		{
			PyOS_setsig(i, SIG_DFL);
		}
		Py_XDECREF(func);
	}

	Py_XDECREF(IntHandler);
	IntHandler = NULL;
	Py_XDECREF(DefaultHandler);
	DefaultHandler = NULL;
	Py_XDECREF(IgnoreHandler);
	IgnoreHandler = NULL;
}


int PyErr_CheckSignals()
{
	int i;
	PyObject *f;

	if (!is_tripped)
	{
		return 0;
	}
	if (PyThread_get_thread_ident() != main_thread)
	{
		return 0;
	}
	if (!(f = PyEval_GetFrame()))
	{
		f = Py_None;
	}

	for (i = 1; i < NSIG; i++) 
	{
		if (Handlers[i].tripped) 
		{
			PyObject *result = NULL;
			PyObject *arglist = Py_BuildValue("(iO)", i, f);
			Handlers[i].tripped = 0;

			if (arglist) 
			{
				result = PyEval_CallObject(Handlers[i].func,
							   arglist);
				Py_DECREF(arglist);
			}
			if (!result)
			{
				return -1;
			}

			Py_DECREF(result);
		}
	}
	is_tripped = 0;
	return 0;
}


void PyErr_SetInterrupt()
{
	is_tripped++;
	Handlers[SIGINT].tripped = 1;
	Py_AddPendingCall((int (*)(void *))PyErr_CheckSignals, NULL);
}

void PyOS_InitInterrupts()
{
	initsignal();
	_PyImport_FixupExtension("signal", "signal");
}

void PyOS_FiniInterrupts()
{
	finisignal();
}

int PyOS_InterruptOccurred()
{
	if (Handlers[SIGINT].tripped) 
	{
		if (PyThread_get_thread_ident() != main_thread)
		{
			return 0;
		}
		Handlers[SIGINT].tripped = 0;
		return 1;
	}
	return 0;
}

void PyOS_AfterFork()
{
	PyEval_ReInitThreads();
	main_thread = PyThread_get_thread_ident();
	main_pid = getpid();
}
