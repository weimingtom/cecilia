//20170809
//20171105
#include "python.h"
#include "compile.h"
#include "frameobject.h"
#include "eval.h"
#include "opcode.h"
#include "structmember.h"

#include <ctype.h>

typedef PyObject *(*callproc)(PyObject *, PyObject *, PyObject *);

static PyObject *eval_frame(PyFrameObject *);
static PyObject *fast_function(PyObject *, PyObject ***, int, int, int);
static PyObject *fast_cfunction(PyObject *, PyObject ***, int);
static PyObject *do_call(PyObject *, PyObject ***, int, int);
static PyObject *ext_do_call(PyObject *, PyObject ***, int, int, int);
static PyObject *update_keyword_args(PyObject *, int, PyObject ***,PyObject *);
static PyObject *update_star_args(int, int, PyObject *, PyObject ***);
static PyObject *load_args(PyObject ***, int);
#define CALL_FLAG_VAR 1
#define CALL_FLAG_KW 2

#ifdef _DEBUG
static int prtrace(PyObject *, char *);
#endif

static int call_trace(Py_tracefunc, PyObject *, PyFrameObject *,
		      int, PyObject *);
static void call_trace_protected(Py_tracefunc, PyObject *,
				 PyFrameObject *, int);
static void call_exc_trace(Py_tracefunc, PyObject *, PyFrameObject *);
static PyObject *loop_subscript(PyObject *, PyObject *);
static PyObject *apply_slice(PyObject *, PyObject *, PyObject *);
static int assign_slice(PyObject *, PyObject *,
			PyObject *, PyObject *);
static PyObject *cmp_outcome(int, PyObject *, PyObject *);
static PyObject *import_from(PyObject *, PyObject *);
static int import_all_from(PyObject *, PyObject *);
static PyObject *build_class(PyObject *, PyObject *, PyObject *);
static int exec_statement(PyFrameObject *,
			  PyObject *, PyObject *, PyObject *);
static void set_exc_info(PyThreadState *, PyObject *, PyObject *, PyObject *);
static void reset_exc_info(PyThreadState *);
static void format_exc_check_arg(PyObject *, char *, PyObject *);

#define NAME_ERROR_MSG \
	"name '%.200s' is not defined"
#define GLOBAL_NAME_ERROR_MSG \
	"global name '%.200s' is not defined"
#define UNBOUNDLOCAL_ERROR_MSG \
	"local variable '%.200s' referenced before assignment"
#define UNBOUNDFREE_ERROR_MSG \
	"free variable '%.200s' referenced before assignment" \
        " in enclosing scope"

staticforward PyTypeObject gentype;

typedef struct {
	PyObject_HEAD
	PyFrameObject *gi_frame;
	int gi_running;
} genobject;

static PyObject *gen_new(PyFrameObject *f)
{
	genobject *gen = PyObject_New(genobject, &gentype);
	if (gen == NULL) 
	{
		Py_DECREF(f);
		return NULL;
	}
	gen->gi_frame = f;
	gen->gi_running = 0;
	return (PyObject *)gen;
}

static int gen_traverse(genobject *gen, visitproc visit, void *arg)
{
	return visit((PyObject *)gen->gi_frame, arg);
}

static void gen_dealloc(genobject *gen)
{
	Py_DECREF(gen->gi_frame);
	PyObject_Del(gen);
}

static PyObject *gen_iternext(genobject *gen)
{
	PyThreadState *tstate = PyThreadState_GET();
	PyFrameObject *f = gen->gi_frame;
	PyObject *result;

	if (gen->gi_running) 
	{
		PyErr_SetString(PyExc_ValueError,
				"generator already executing");
		return NULL;
	}
	if (f->f_stacktop == NULL)
	{
		return NULL;
	}

	Py_XINCREF(tstate->frame);
	assert(f->f_back == NULL);
	f->f_back = tstate->frame;

	gen->gi_running = 1;
	result = eval_frame(f);
	gen->gi_running = 0;

	Py_XDECREF(f->f_back);
	f->f_back = NULL;

	if (result == Py_None && f->f_stacktop == NULL) 
	{
		Py_DECREF(result);
		result = NULL;
	}

	return result;
}

static PyObject *gen_next(genobject *gen)
{
	PyObject *result;

	result = gen_iternext(gen);

	if (result == NULL && !PyErr_Occurred()) 
	{
		PyErr_SetObject(PyExc_StopIteration, Py_None);
		return NULL;
	}

	return result;
}

static PyObject *gen_getiter(PyObject *gen)
{
	Py_INCREF(gen);
	return gen;
}

static struct PyMethodDef gen_methods[] = {
	{"next",     (PyCFunction)gen_next, METH_NOARGS,
	 	"next() -- get the next value, or raise StopIteration"},
	{NULL,          NULL}
};

static PyMemberDef gen_memberlist[] = {
	{"gi_frame",	T_OBJECT, offsetof(genobject, gi_frame),	RO},
	{"gi_running",	T_INT,    offsetof(genobject, gi_running),	RO},
	{NULL}
};

statichere PyTypeObject gentype = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"generator",
	sizeof(genobject),
	0,
	(destructor)gen_dealloc,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	PyObject_GenericGetAttr,
	0,
	0,
	Py_TPFLAGS_DEFAULT,
 	0,
 	(traverseproc)gen_traverse,
 	0,
	0,
	0,
	(getiterfunc)gen_getiter,
	(iternextfunc)gen_iternext,
	gen_methods,
	gen_memberlist,
	0,
	0,
	0,
};


#include <errno.h>

#include "pythread.h"

extern int _PyThread_Started;

static PyThread_type_lock interpreter_lock = 0;
static long main_thread = 0;

void PyEval_InitThreads()
{
	if (interpreter_lock)
	{
		return;
	}
	_PyThread_Started = 1;
	interpreter_lock = PyThread_allocate_lock();
	PyThread_acquire_lock(interpreter_lock, 1);
	main_thread = PyThread_get_thread_ident();
}

void PyEval_AcquireLock()
{
	PyThread_acquire_lock(interpreter_lock, 1);
}

void PyEval_ReleaseLock()
{
	PyThread_release_lock(interpreter_lock);
}

void PyEval_AcquireThread(PyThreadState *tstate)
{
	if (tstate == NULL)
	{
		Py_FatalError("PyEval_AcquireThread: NULL new thread state");
	}
	PyThread_acquire_lock(interpreter_lock, 1);
	if (PyThreadState_Swap(tstate) != NULL)
	{
		Py_FatalError(
			"PyEval_AcquireThread: non-NULL old thread state");
	}
}

void PyEval_ReleaseThread(PyThreadState *tstate)
{
	if (tstate == NULL)
	{
		Py_FatalError("PyEval_ReleaseThread: NULL thread state");
	}
	if (PyThreadState_Swap(NULL) != tstate)
	{
		Py_FatalError("PyEval_ReleaseThread: wrong thread state");
	}
	PyThread_release_lock(interpreter_lock);
}

void PyEval_ReInitThreads()
{
	if (!interpreter_lock)
	{
		return;
	}
	interpreter_lock = PyThread_allocate_lock();
	PyThread_acquire_lock(interpreter_lock, 1);
	main_thread = PyThread_get_thread_ident();
}

PyThreadState *PyEval_SaveThread()
{
	PyThreadState *tstate = PyThreadState_Swap(NULL);
	if (tstate == NULL)
	{
		Py_FatalError("PyEval_SaveThread: NULL tstate");
	}
	if (interpreter_lock)
	{
		PyThread_release_lock(interpreter_lock);
	}
	return tstate;
}

void PyEval_RestoreThread(PyThreadState *tstate)
{
	if (tstate == NULL)
	{
		Py_FatalError("PyEval_RestoreThread: NULL tstate");
	}
	if (interpreter_lock) 
	{
		int err = errno;
		PyThread_acquire_lock(interpreter_lock, 1);
		errno = err;
	}
	PyThreadState_Swap(tstate);
}

#define NPENDINGCALLS 32

static struct {
	int (*func)(void *);
	void *arg;
} pendingcalls[NPENDINGCALLS];

static volatile int pendingfirst = 0;
static volatile int pendinglast = 0;
static volatile int things_to_do = 0;

int Py_AddPendingCall(int (*func)(void *), void *arg)
{
	static int busy = 0;
	int i, j;

	if (busy)
	{
		return -1;
	}
	busy = 1;
	i = pendinglast;
	j = (i + 1) % NPENDINGCALLS;
	if (j == pendingfirst)
	{
		return -1;
	}
	pendingcalls[i].func = func;
	pendingcalls[i].arg = arg;
	pendinglast = j;
	things_to_do = 1;
	busy = 0;
	return 0;
}

int Py_MakePendingCalls()
{
	static int busy = 0;
	if (main_thread && PyThread_get_thread_ident() != main_thread)
	{
		return 0;
	}
	if (busy)
	{
		return 0;
	}
	busy = 1;
	things_to_do = 0;
	for (;;) 
	{
		int i;
		int (*func)(void *);
		void *arg;
		i = pendingfirst;
		if (i == pendinglast)
		{
			break;
		}
		func = pendingcalls[i].func;
		arg = pendingcalls[i].arg;
		pendingfirst = (i + 1) % NPENDINGCALLS;
		if (func(arg) < 0) 
		{
			busy = 0;
			things_to_do = 1;
			return -1;
		}
	}
	busy = 0;
	return 0;
}

static int recursion_limit = 1000;

int Py_GetRecursionLimit()
{
	return recursion_limit;
}

void Py_SetRecursionLimit(int new_limit)
{
	recursion_limit = new_limit;
}

enum why_code {
		WHY_NOT,
		WHY_EXCEPTION,
		WHY_RERAISE,
		WHY_RETURN,
		WHY_BREAK,
		WHY_CONTINUE,
		WHY_YIELD
};

static enum why_code do_raise(PyObject *, PyObject *, PyObject *);
static int unpack_iterable(PyObject *, int, PyObject **);

PyObject *PyEval_EvalCode(PyCodeObject *co, PyObject *globals, PyObject *locals)
{
	return PyEval_EvalCodeEx(co,
			  globals, locals,
			  (PyObject **)NULL, 0,
			  (PyObject **)NULL, 0,
			  (PyObject **)NULL, 0,
			  NULL);
}

static PyObject *eval_frame(PyFrameObject *f)
{
	PyObject **stack_pointer;
	unsigned char *next_instr;
	int opcode=0;
	int oparg=0;
	enum why_code why;
	int err;
	PyObject *x;
	PyObject *v;
	PyObject *w;
	PyObject *u;
	PyObject *t;
	PyObject *stream = NULL;
	PyObject **fastlocals, **freevars;
	PyObject *retval = NULL;
	PyThreadState *tstate = PyThreadState_GET();
	PyCodeObject *co;
	unsigned char *first_instr;
#ifdef _DEBUG
	int lltrace;
	char *filename;
#endif

#define GETCONST(i)	Getconst(f, i)
#define GETNAME(i)	Getname(f, i)
#define GETNAMEV(i)	Getnamev(f, i)
#define INSTR_OFFSET()	(next_instr - first_instr)
#define NEXTOP()	(*next_instr++)
#define NEXTARG()	(next_instr += 2, (next_instr[-1]<<8) + next_instr[-2])
#define JUMPTO(x)	(next_instr = first_instr + (x))
#define JUMPBY(x)	(next_instr += (x))

#define STACK_LEVEL()	(stack_pointer - f->f_valuestack)
#define EMPTY()		(STACK_LEVEL() == 0)
#define TOP()		(stack_pointer[-1])
#define BASIC_PUSH(v)	(*stack_pointer++ = (v))
#define BASIC_POP()	(*--stack_pointer)

#ifdef _DEBUG
#define PUSH(v)		{ (BASIC_PUSH(v), \
                               lltrace && prtrace(TOP(), "push")); \
                               assert(STACK_LEVEL() <= f->f_stacksize); }
#define POP()		((lltrace && prtrace(TOP(), "pop")), BASIC_POP())
#else
#define PUSH(v)		BASIC_PUSH(v)
#define POP()		BASIC_POP()
#endif

#define GETLOCAL(i)	(fastlocals[i])

#define SETLOCAL(i, value)	do { PyObject *tmp = GETLOCAL(i); \
				     GETLOCAL(i) = value; \
                                     Py_XDECREF(tmp); } while (0)


	if (f == NULL)
	{
		return NULL;
	}

	if (tstate->recursion_depth%10 == 0 && PyOS_CheckStack()) 
	{
		PyErr_SetString(PyExc_MemoryError, "Stack overflow");
		return NULL;
	}

	if (++tstate->recursion_depth > recursion_limit) 
	{
		--tstate->recursion_depth;
		PyErr_SetString(PyExc_RuntimeError,
				"maximum recursion depth exceeded");
		tstate->frame = f->f_back;
		return NULL;
	}

	tstate->frame = f;

	if (tstate->use_tracing) 
	{
		if (tstate->c_tracefunc != NULL) 
		{
			if (call_trace(tstate->c_tracefunc, tstate->c_traceobj,
				       f, PyTrace_CALL, Py_None)) 
			{
				--tstate->recursion_depth;
				tstate->frame = f->f_back;
				return NULL;
			}
		}
		if (tstate->c_profilefunc != NULL) 
		{
			if (call_trace(tstate->c_profilefunc,
				       tstate->c_profileobj,
				       f, PyTrace_CALL, Py_None)) 
			{
				--tstate->recursion_depth;
				tstate->frame = f->f_back;
				return NULL;
			}
		}
	}

	co = f->f_code;
	fastlocals = f->f_localsplus;
	freevars = f->f_localsplus + f->f_nlocals;
	_PyCode_GETCODEPTR(co, &first_instr);
	next_instr = first_instr + f->f_lasti;
	stack_pointer = f->f_stacktop;
	assert(stack_pointer != NULL);
	f->f_stacktop = NULL;

#ifdef _DEBUG
	lltrace = PyDict_GetItemString(f->f_globals, "__lltrace__") != NULL;
	filename = PyString_AsString(co->co_filename);
#endif

	why = WHY_NOT;
	err = 0;
	x = Py_None;
	w = NULL;

	for (;;) 
	{
		assert(stack_pointer >= f->f_valuestack);
		assert(STACK_LEVEL() <= f->f_stacksize);

		if (things_to_do || --tstate->ticker < 0) 
		{
			tstate->ticker = tstate->interp->checkinterval;
			tstate->tick_counter++;
			if (things_to_do) 
			{
				if (Py_MakePendingCalls() < 0) 
				{
					why = WHY_EXCEPTION;
					goto on_error;
				}
			}

			if (interpreter_lock) 
			{
				if (PyThreadState_Swap(NULL) != tstate)
				{
					Py_FatalError("ceval: tstate mix-up");
				}
				PyThread_release_lock(interpreter_lock);

				PyThread_acquire_lock(interpreter_lock, 1);
				if (PyThreadState_Swap(tstate) != NULL)
				{
					Py_FatalError("ceval: orphan tstate");
				}
			}
		}


#if defined(_DEBUG)
		f->f_lasti = INSTR_OFFSET();
#endif

		opcode = NEXTOP();
		if (HAS_ARG(opcode))
		{
			oparg = NEXTARG();
		}

dispatch_opcode:

#ifdef _DEBUG
		if (lltrace) 
		{
			if (HAS_ARG(opcode)) 
			{
				printf("%d: %d, %d\n",
					(int) (INSTR_OFFSET() - 3),
					opcode, oparg);
			}
			else 
			{
				printf("%d: %d\n",
					(int) (INSTR_OFFSET() - 1), opcode);
			}
		}
#endif

		switch (opcode) 
		{
		case POP_TOP:
			v = POP();
			Py_DECREF(v);
			continue;

		case ROT_TWO:
			v = POP();
			w = POP();
			PUSH(v);
			PUSH(w);
			continue;

		case ROT_THREE:
			v = POP();
			w = POP();
			x = POP();
			PUSH(v);
			PUSH(x);
			PUSH(w);
			continue;

		case ROT_FOUR:
			u = POP();
			v = POP();
			w = POP();
			x = POP();
			PUSH(u);
			PUSH(x);
			PUSH(w);
			PUSH(v);
			continue;

		case DUP_TOP:
			v = TOP();
			Py_INCREF(v);
			PUSH(v);
			continue;

		case DUP_TOPX:
			switch (oparg) 
			{
			case 1:
				x = TOP();
				Py_INCREF(x);
				PUSH(x);
				continue;
			
			case 2:
				x = POP();
				Py_INCREF(x);
				w = TOP();
				Py_INCREF(w);
				PUSH(x);
				PUSH(w);
				PUSH(x);
				continue;
			
			case 3:
				x = POP();
				Py_INCREF(x);
				w = POP();
				Py_INCREF(w);
				v = TOP();
				Py_INCREF(v);
				PUSH(w);
				PUSH(x);
				PUSH(v);
				PUSH(w);
				PUSH(x);
				continue;
			
			case 4:
				x = POP();
				Py_INCREF(x);
				w = POP();
				Py_INCREF(w);
				v = POP();
				Py_INCREF(v);
				u = TOP();
				Py_INCREF(u);
				PUSH(v);
				PUSH(w);
				PUSH(x);
				PUSH(u);
				PUSH(v);
				PUSH(w);
				PUSH(x);
				continue;
			
			case 5:
				x = POP();
				Py_INCREF(x);
				w = POP();
				Py_INCREF(w);
				v = POP();
				Py_INCREF(v);
				u = POP();
				Py_INCREF(u);
				t = TOP();
				Py_INCREF(t);
				PUSH(u);
				PUSH(v);
				PUSH(w);
				PUSH(x);
				PUSH(t);
				PUSH(u);
				PUSH(v);
				PUSH(w);
				PUSH(x);
				continue;
			
			default:
				Py_FatalError("invalid argument to DUP_TOPX"
					      " (bytecode corruption?)");
			}
			break;

		case UNARY_POSITIVE:
			v = POP();
			x = PyNumber_Positive(v);
			Py_DECREF(v);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case UNARY_NEGATIVE:
			v = POP();
			x = PyNumber_Negative(v);
			Py_DECREF(v);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case UNARY_NOT:
			v = POP();
			err = PyObject_IsTrue(v);
			Py_DECREF(v);
			if (err == 0) 
			{
				Py_INCREF(Py_True);
				PUSH(Py_True);
				continue;
			}
			else if (err > 0) 
			{
				Py_INCREF(Py_False);
				PUSH(Py_False);
				err = 0;
				continue;
			}
			break;

		case UNARY_CONVERT:
			v = POP();
			x = PyObject_Repr(v);
			Py_DECREF(v);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case UNARY_INVERT:
			v = POP();
			x = PyNumber_Invert(v);
			Py_DECREF(v);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_POWER:
			w = POP();
			v = POP();
			x = PyNumber_Power(v, w, Py_None);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_MULTIPLY:
			w = POP();
			v = POP();
			x = PyNumber_Multiply(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_DIVIDE:
			if (!_Py_QnewFlag) 
			{
				w = POP();
				v = POP();
				x = PyNumber_Divide(v, w);
				Py_DECREF(v);
				Py_DECREF(w);
				PUSH(x);
				if (x != NULL) 
				{
					continue;
				}
				break;
			}
			//no break;

		case BINARY_TRUE_DIVIDE:
			w = POP();
			v = POP();
			x = PyNumber_TrueDivide(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_FLOOR_DIVIDE:
			w = POP();
			v = POP();
			x = PyNumber_FloorDivide(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_MODULO:
			w = POP();
			v = POP();
			x = PyNumber_Remainder(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_ADD:
			w = POP();
			v = POP();
			if (PyInt_CheckExact(v) && PyInt_CheckExact(w)) 
			{
				long a, b, i;
				a = PyInt_AS_LONG(v);
				b = PyInt_AS_LONG(w);
				i = a + b;
				if ((i^a) < 0 && (i^b) < 0)
				{
					goto slow_add;
				}
				x = PyInt_FromLong(i);
			}
			else 
			{
slow_add:
				x = PyNumber_Add(v, w);
			}
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_SUBTRACT:
			w = POP();
			v = POP();
			if (PyInt_CheckExact(v) && PyInt_CheckExact(w)) 
			{
				long a, b, i;
				a = PyInt_AS_LONG(v);
				b = PyInt_AS_LONG(w);
				i = a - b;
				if ((i^a) < 0 && (i^~b) < 0)
				{
					goto slow_sub;
				}
				x = PyInt_FromLong(i);
			}
			else 
			{
slow_sub:
				x = PyNumber_Subtract(v, w);
			}
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_SUBSCR:
			w = POP();
			v = POP();
			if (PyList_CheckExact(v) && PyInt_CheckExact(w)) 
			{
				long i = PyInt_AsLong(w);
				if (i < 0)
				{
					i += PyList_GET_SIZE(v);
				}
				if (i < 0 ||
				    i >= PyList_GET_SIZE(v)) 
				{
					PyErr_SetString(PyExc_IndexError,
						"list index out of range");
					x = NULL;
				}
				else 
				{
					x = PyList_GET_ITEM(v, i);
					Py_INCREF(x);
				}
			}
			else
			{
				x = PyObject_GetItem(v, w);
			}
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_LSHIFT:
			w = POP();
			v = POP();
			x = PyNumber_Lshift(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_RSHIFT:
			w = POP();
			v = POP();
			x = PyNumber_Rshift(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_AND:
			w = POP();
			v = POP();
			x = PyNumber_And(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_XOR:
			w = POP();
			v = POP();
			x = PyNumber_Xor(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case BINARY_OR:
			w = POP();
			v = POP();
			x = PyNumber_Or(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_POWER:
			w = POP();
			v = POP();
			x = PyNumber_InPlacePower(v, w, Py_None);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_MULTIPLY:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceMultiply(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_DIVIDE:
			if (!_Py_QnewFlag) 
			{
				w = POP();
				v = POP();
				x = PyNumber_InPlaceDivide(v, w);
				Py_DECREF(v);
				Py_DECREF(w);
				PUSH(x);
				if (x != NULL) 
				{
					continue;
				}
				break;
			}
			//no break;

		case INPLACE_TRUE_DIVIDE:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceTrueDivide(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_FLOOR_DIVIDE:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceFloorDivide(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_MODULO:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceRemainder(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_ADD:
			w = POP();
			v = POP();
			if (PyInt_CheckExact(v) && PyInt_CheckExact(w)) 
			{
				long a, b, i;
				a = PyInt_AS_LONG(v);
				b = PyInt_AS_LONG(w);
				i = a + b;
				if ((i^a) < 0 && (i^b) < 0)
				{
					goto slow_iadd;
				}
				x = PyInt_FromLong(i);
			}
			else 
			{
slow_iadd:
				x = PyNumber_InPlaceAdd(v, w);
			}
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_SUBTRACT:
			w = POP();
			v = POP();
			if (PyInt_CheckExact(v) && PyInt_CheckExact(w)) 
			{
				long a, b, i;
				a = PyInt_AS_LONG(v);
				b = PyInt_AS_LONG(w);
				i = a - b;
				if ((i^a) < 0 && (i^~b) < 0)
				{
					goto slow_isub;
				}
				x = PyInt_FromLong(i);
			}
			else 
			{
slow_isub:
				x = PyNumber_InPlaceSubtract(v, w);
			}
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_LSHIFT:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceLshift(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_RSHIFT:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceRshift(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_AND:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceAnd(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_XOR:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceXor(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case INPLACE_OR:
			w = POP();
			v = POP();
			x = PyNumber_InPlaceOr(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case SLICE+0:
		case SLICE+1:
		case SLICE+2:
		case SLICE+3:
			if ((opcode - SLICE) & 2)
			{
				w = POP();
			}
			else
			{
				w = NULL;
			}
			if ((opcode - SLICE) & 1)
			{
				v = POP();
			}
			else
			{
				v = NULL;
			}
			u = POP();
			x = apply_slice(u, v, w);
			Py_DECREF(u);
			Py_XDECREF(v);
			Py_XDECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case STORE_SLICE+0:
		case STORE_SLICE+1:
		case STORE_SLICE+2:
		case STORE_SLICE+3:
			if ((opcode - STORE_SLICE) & 2)
			{
				w = POP();
			}
			else
			{
				w = NULL;
			}
			if ((opcode-STORE_SLICE) & 1)
			{
				v = POP();
			}
			else
			{
				v = NULL;
			}
			u = POP();
			t = POP();
			err = assign_slice(u, v, w, t);
			Py_DECREF(t);
			Py_DECREF(u);
			Py_XDECREF(v);
			Py_XDECREF(w);
			if (err == 0) 
			{
				continue;
			}
			break;

		case DELETE_SLICE+0:
		case DELETE_SLICE+1:
		case DELETE_SLICE+2:
		case DELETE_SLICE+3:
			if ((opcode-DELETE_SLICE) & 2)
			{
				w = POP();
			}
			else
			{
				w = NULL;
			}
			if ((opcode-DELETE_SLICE) & 1)
			{
				v = POP();
			}
			else
			{
				v = NULL;
			}
			u = POP();
			err = assign_slice(u, v, w, (PyObject *)NULL);
			Py_DECREF(u);
			Py_XDECREF(v);
			Py_XDECREF(w);
			if (err == 0) 
			{
				continue;
			}
			break;

		case STORE_SUBSCR:
			w = POP();
			v = POP();
			u = POP();
			err = PyObject_SetItem(v, w, u);
			Py_DECREF(u);
			Py_DECREF(v);
			Py_DECREF(w);
			if (err == 0) 
			{
				continue;
			}
			break;

		case DELETE_SUBSCR:
			w = POP();
			v = POP();
			err = PyObject_DelItem(v, w);
			Py_DECREF(v);
			Py_DECREF(w);
			if (err == 0) 
			{
				continue;
			}
			break;

		case PRINT_EXPR:
			v = POP();
			w = PySys_GetObject("displayhook");
			if (w == NULL) 
			{
				PyErr_SetString(PyExc_RuntimeError,
						"lost sys.displayhook");
				err = -1;
				x = NULL;
			}
			if (err == 0) 
			{
				x = Py_BuildValue("(O)", v);
				if (x == NULL)
				{
					err = -1;
				}
			}
			if (err == 0) 
			{
				w = PyEval_CallObject(w, x);
				Py_XDECREF(w);
				if (w == NULL)
				{
					err = -1;
				}
			}
			Py_DECREF(v);
			Py_XDECREF(x);
			break;

		case PRINT_ITEM_TO:
			w = stream = POP();
			//no break;

		case PRINT_ITEM:
			v = POP();
			if (stream == NULL || stream == Py_None) 
			{
				w = PySys_GetObject("stdout");
				if (w == NULL) 
				{
					PyErr_SetString(PyExc_RuntimeError,
							"lost sys.stdout");
					err = -1;
				}
			}
			if (w != NULL && PyFile_SoftSpace(w, 1))
			{
				err = PyFile_WriteString(" ", w);
			}
			if (err == 0)
			{
				err = PyFile_WriteObject(v, w, Py_PRINT_RAW);
			}
			if (err == 0) 
			{
				if (PyString_Check(v)) 
				{
					char *s = PyString_AS_STRING(v);
					int len = PyString_GET_SIZE(v);
					if (len > 0 &&
						isspace(Py_CHARMASK(s[len-1])) &&
						s[len-1] != ' ')
					{
						PyFile_SoftSpace(w, 0);
					}
			    } 
			    else if (PyUnicode_Check(v)) 
				{
					Py_UNICODE *s = PyUnicode_AS_UNICODE(v);
					int len = PyUnicode_GET_SIZE(v);
					if (len > 0 &&
						Py_UNICODE_ISSPACE(s[len-1]) &&
						s[len-1] != ' ')
					{
						PyFile_SoftSpace(w, 0);
					}
			    }
			}
			Py_DECREF(v);
			Py_XDECREF(stream);
			stream = NULL;
			if (err == 0)
			{
				continue;
			}
			break;

		case PRINT_NEWLINE_TO:
			w = stream = POP();
			//no break;

		case PRINT_NEWLINE:
			if (stream == NULL || stream == Py_None) 
			{
				w = PySys_GetObject("stdout");
				if (w == NULL)
				{
					PyErr_SetString(PyExc_RuntimeError,
							"lost sys.stdout");
				}
			}
			if (w != NULL) 
			{
				err = PyFile_WriteString("\n", w);
				if (err == 0)
				{
					PyFile_SoftSpace(w, 0);
				}
			}
			Py_XDECREF(stream);
			stream = NULL;
			break;

		case BREAK_LOOP:
			why = WHY_BREAK;
			break;

		case CONTINUE_LOOP:
			retval = PyInt_FromLong(oparg);
			why = WHY_CONTINUE;
			break;

		case RAISE_VARARGS:
			u = v = w = NULL;
			switch (oparg) 
			{
			case 3:
				u = POP();
				//no break;
			case 2:
				v = POP();
				//no break;
			case 1:
				w = POP();
				//no break;
			case 0:
				why = do_raise(w, v, u);
				break;

			default:
				PyErr_SetString(PyExc_SystemError,
					   "bad RAISE_VARARGS oparg");
				why = WHY_EXCEPTION;
				break;
			}
			break;

		case LOAD_LOCALS:
			if ((x = f->f_locals) == NULL) 
			{
				PyErr_SetString(PyExc_SystemError,
						"no locals");
				break;
			}
			Py_INCREF(x);
			PUSH(x);
			break;

		case RETURN_VALUE:
			retval = POP();
			why = WHY_RETURN;
			break;

		case YIELD_VALUE:
			retval = POP();
			f->f_stacktop = stack_pointer;
			f->f_lasti = INSTR_OFFSET();
			why = WHY_YIELD;
			break;

		case EXEC_STMT:
			w = POP();
			v = POP();
			u = POP();
			err = exec_statement(f, u, v, w);
			Py_DECREF(u);
			Py_DECREF(v);
			Py_DECREF(w);
			break;

		case POP_BLOCK:
			{
				PyTryBlock *b = PyFrame_BlockPop(f);
				while (STACK_LEVEL() > b->b_level) 
				{
					v = POP();
					Py_DECREF(v);
				}
			}
			break;

		case END_FINALLY:
			v = POP();
			if (PyInt_Check(v)) 
			{
				why = (enum why_code)PyInt_AsLong(v);
				if (why == WHY_RETURN ||
				    why == WHY_YIELD ||
				    why == WHY_CONTINUE)
				{
					retval = POP();
				}
			}
			else if (PyString_Check(v) || PyClass_Check(v)) 
			{
				w = POP();
				u = POP();
				PyErr_Restore(v, w, u);
				why = WHY_RERAISE;
				break;
			}
			else if (v != Py_None) 
			{
				PyErr_SetString(PyExc_SystemError,
					"'finally' pops bad exception");
				why = WHY_EXCEPTION;
			}
			Py_DECREF(v);
			break;

		case BUILD_CLASS:
			u = POP();
			v = POP();
			w = POP();
			x = build_class(u, v, w);
			PUSH(x);
			Py_DECREF(u);
			Py_DECREF(v);
			Py_DECREF(w);
			break;

		case STORE_NAME:
			w = GETNAMEV(oparg);
			v = POP();
			if ((x = f->f_locals) == NULL) 
			{
				PyErr_Format(PyExc_SystemError,
					     "no locals found when storing %s",
					     PyObject_REPR(w));
				break;
			}
			err = PyDict_SetItem(x, w, v);
			Py_DECREF(v);
			break;

		case DELETE_NAME:
			w = GETNAMEV(oparg);
			if ((x = f->f_locals) == NULL) 
			{
				PyErr_Format(PyExc_SystemError,
					     "no locals when deleting %s",
					     PyObject_REPR(w));
				break;
			}
			if ((err = PyDict_DelItem(x, w)) != 0)
			{
				format_exc_check_arg(PyExc_NameError,
							NAME_ERROR_MSG ,w);
			}
			break;

		case UNPACK_SEQUENCE:
			v = POP();
			if (PyTuple_Check(v)) 
			{
				if (PyTuple_Size(v) != oparg) 
				{
					PyErr_SetString(PyExc_ValueError,
						 "unpack tuple of wrong size");
					why = WHY_EXCEPTION;
				}
				else 
				{
					for (; --oparg >= 0; ) 
					{
						w = PyTuple_GET_ITEM(v, oparg);
						Py_INCREF(w);
						PUSH(w);
					}
				}
			}
			else if (PyList_Check(v)) 
			{
				if (PyList_Size(v) != oparg) 
				{
					PyErr_SetString(PyExc_ValueError,
						  "unpack list of wrong size");
					why = WHY_EXCEPTION;
				}
				else 
				{
					for (; --oparg >= 0; ) 
					{
						w = PyList_GET_ITEM(v, oparg);
						Py_INCREF(w);
						PUSH(w);
					}
				}
			}
			else if (unpack_iterable(v, oparg,
						 stack_pointer + oparg))
			{
				stack_pointer += oparg;
			}
			else 
			{
				if (PyErr_ExceptionMatches(PyExc_TypeError))
				{
					PyErr_SetString(PyExc_TypeError,
						"unpack non-sequence");
				}
				why = WHY_EXCEPTION;
			}
			Py_DECREF(v);
			break;

		case STORE_ATTR:
			w = GETNAMEV(oparg);
			v = POP();
			u = POP();
			err = PyObject_SetAttr(v, w, u);
			Py_DECREF(v);
			Py_DECREF(u);
			break;

		case DELETE_ATTR:
			w = GETNAMEV(oparg);
			v = POP();
			err = PyObject_SetAttr(v, w, (PyObject *)NULL);
			Py_DECREF(v);
			break;

		case STORE_GLOBAL:
			w = GETNAMEV(oparg);
			v = POP();
			err = PyDict_SetItem(f->f_globals, w, v);
			Py_DECREF(v);
			break;

		case DELETE_GLOBAL:
			w = GETNAMEV(oparg);
			if ((err = PyDict_DelItem(f->f_globals, w)) != 0)
			{
				format_exc_check_arg(
				    PyExc_NameError, GLOBAL_NAME_ERROR_MSG, w);
			}
			break;

		case LOAD_CONST:
			x = GETCONST(oparg);
			Py_INCREF(x);
			PUSH(x);
			break;

		case LOAD_NAME:
			w = GETNAMEV(oparg);
			if ((x = f->f_locals) == NULL) 
			{
				PyErr_Format(PyExc_SystemError,
					     "no locals when loading %s",
					     PyObject_REPR(w));
				break;
			}
			x = PyDict_GetItem(x, w);
			if (x == NULL) 
			{
				x = PyDict_GetItem(f->f_globals, w);
				if (x == NULL) 
				{
					x = PyDict_GetItem(f->f_builtins, w);
					if (x == NULL) 
					{
						format_exc_check_arg(
							    PyExc_NameError,
							    NAME_ERROR_MSG ,w);
						break;
					}
				}
			}
			Py_INCREF(x);
			PUSH(x);
			break;

		case LOAD_GLOBAL:
			w = GETNAMEV(oparg);
			x = PyDict_GetItem(f->f_globals, w);
			if (x == NULL) 
			{
				x = PyDict_GetItem(f->f_builtins, w);
				if (x == NULL) 
				{
					format_exc_check_arg(
						    PyExc_NameError,
						    GLOBAL_NAME_ERROR_MSG ,w);
					break;
				}
			}
			Py_INCREF(x);
			PUSH(x);
			break;

		case LOAD_FAST:
			x = GETLOCAL(oparg);
			if (x == NULL) 
			{
				format_exc_check_arg(
					PyExc_UnboundLocalError,
					UNBOUNDLOCAL_ERROR_MSG,
					PyTuple_GetItem(co->co_varnames, oparg)
					);
				break;
			}
			Py_INCREF(x);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case STORE_FAST:
			v = POP();
			SETLOCAL(oparg, v);
			continue;

		case DELETE_FAST:
			x = GETLOCAL(oparg);
			if (x == NULL) 
			{
				format_exc_check_arg(
					PyExc_UnboundLocalError,
					UNBOUNDLOCAL_ERROR_MSG,
					PyTuple_GetItem(co->co_varnames, oparg)
					);
				break;
			}
			SETLOCAL(oparg, NULL);
			continue;

		case LOAD_CLOSURE:
			x = freevars[oparg];
			Py_INCREF(x);
			PUSH(x);
			break;

		case LOAD_DEREF:
			x = freevars[oparg];
			w = PyCell_Get(x);
			if (w == NULL) 
			{
				if (oparg < f->f_ncells) 
				{
					v = PyTuple_GetItem(co->co_cellvars,
							       oparg);
				       format_exc_check_arg(
					       PyExc_UnboundLocalError,
					       UNBOUNDLOCAL_ERROR_MSG,
					       v);
				} 
				else 
				{
				       v = PyTuple_GetItem(
						      co->co_freevars,
						      oparg - f->f_ncells);
				       format_exc_check_arg(
					       PyExc_NameError,
					       UNBOUNDFREE_ERROR_MSG,
					       v);
				}
				err = -1;
				break;
			}
			PUSH(w);
			break;

		case STORE_DEREF:
			w = POP();
			x = freevars[oparg];
			PyCell_Set(x, w);
			Py_DECREF(w);
			continue;

		case BUILD_TUPLE:
			x = PyTuple_New(oparg);
			if (x != NULL) 
			{
				for (; --oparg >= 0;) 
				{
					w = POP();
					PyTuple_SET_ITEM(x, oparg, w);
				}
				PUSH(x);
				continue;
			}
			break;

		case BUILD_LIST:
			x =  PyList_New(oparg);
			if (x != NULL) 
			{
				for (; --oparg >= 0;) 
				{
					w = POP();
					PyList_SET_ITEM(x, oparg, w);
				}
				PUSH(x);
				continue;
			}
			break;

		case BUILD_MAP:
			x = PyDict_New();
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case LOAD_ATTR:
			w = GETNAMEV(oparg);
			v = POP();
			x = PyObject_GetAttr(v, w);
			Py_DECREF(v);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case COMPARE_OP:
			w = POP();
			v = POP();
			if (PyInt_CheckExact(v) && PyInt_CheckExact(w)) 
			{
				long a, b;
				int res;
				a = PyInt_AS_LONG(v);
				b = PyInt_AS_LONG(w);
				switch (oparg) 
				{
				case LT: 
					res = a <  b; 
					break;
				
				case LE: 
					res = a <= b; 
					break;
				
				case EQ: 
					res = a == b; 
					break;
				
				case NE: 
					res = a != b; 
					break;
				
				case GT: 
					res = a >  b; 
					break;
				
				case GE: 
					res = a >= b; 
					break;
				
				case IS: 
					res = v == w; 
					break;
				
				case IS_NOT: 
					res = v != w; 
					break;
				
				default: 
					goto slow_compare;
				}
				x = res ? Py_True : Py_False;
				Py_INCREF(x);
			}
			else 
			{
slow_compare:
				x = cmp_outcome(oparg, v, w);
			}
			Py_DECREF(v);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case IMPORT_NAME:
			w = GETNAMEV(oparg);
			x = PyDict_GetItemString(f->f_builtins, "__import__");
			if (x == NULL) 
			{
				PyErr_SetString(PyExc_ImportError,
						"__import__ not found");
				break;
			}
			u = POP();
			w = Py_BuildValue("(OOOO)",
				    w,
				    f->f_globals,
				    f->f_locals == NULL ?
					  Py_None : f->f_locals,
				    u);
			Py_DECREF(u);
			if (w == NULL) 
			{
				x = NULL;
				break;
			}
			x = PyEval_CallObject(x, w);
			Py_DECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case IMPORT_STAR:
			v = POP();
			PyFrame_FastToLocals(f);
			if ((x = f->f_locals) == NULL) 
			{
				PyErr_SetString(PyExc_SystemError,
					"no locals found during 'import *'");
				break;
			}
			err = import_all_from(x, v);
			PyFrame_LocalsToFast(f, 0);
			Py_DECREF(v);
			if (err == 0) 
			{
				continue;
			}
			break;

		case IMPORT_FROM:
			w = GETNAMEV(oparg);
			v = TOP();
			x = import_from(v, w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case JUMP_FORWARD:
			JUMPBY(oparg);
			continue;

		case JUMP_IF_FALSE:
			err = PyObject_IsTrue(TOP());
			if (err > 0)
			{
				err = 0;
			}
			else if (err == 0)
			{
				JUMPBY(oparg);
			}
			else
			{
				break;
			}
			continue;

		case JUMP_IF_TRUE:
			err = PyObject_IsTrue(TOP());
			if (err > 0) 
			{
				err = 0;
				JUMPBY(oparg);
			}
			else if (err == 0)
			{
				;
			}
			else
			{
				break;
			}
			continue;

		case JUMP_ABSOLUTE:
			JUMPTO(oparg);
			continue;

		case GET_ITER:
			v = POP();
			x = PyObject_GetIter(v);
			Py_DECREF(v);
			if (x != NULL) 
			{
				PUSH(x);
				continue;
			}
			break;

		case FOR_ITER:
			v = TOP();
			x = PyIter_Next(v);
			if (x != NULL) 
			{
				PUSH(x);
				continue;
			}
			if (!PyErr_Occurred()) 
			{
				x = v = POP();
				Py_DECREF(v);
				JUMPBY(oparg);
				continue;
			}
			break;

		case FOR_LOOP:
			w = POP();
			v = POP();
			u = loop_subscript(v, w);
			if (u != NULL) 
			{
				PUSH(v);
				x = PyInt_FromLong(PyInt_AsLong(w)+1);
				PUSH(x);
				Py_DECREF(w);
				PUSH(u);
				if (x != NULL) 
				{
					continue;
				}
			}
			else 
			{
				Py_DECREF(v);
				Py_DECREF(w);
				if (PyErr_Occurred())
				{
					why = WHY_EXCEPTION;
				}
				else 
				{
					JUMPBY(oparg);
					continue;
				}
			}
			break;

		case SETUP_LOOP:
		case SETUP_EXCEPT:
		case SETUP_FINALLY:
			PyFrame_BlockSetup(f, opcode, INSTR_OFFSET() + oparg,
					   STACK_LEVEL());
			continue;

		case SET_LINENO:
#ifdef _DEBUG
			if (lltrace)
			{
				printf("--- %s:%d \n", filename, oparg);
			}
#endif
			f->f_lineno = oparg;
			if (tstate->c_tracefunc == NULL || tstate->tracing)
			{
				continue;
			}
			f->f_lasti = INSTR_OFFSET();
			f->f_stacktop = stack_pointer;
			tstate->tracing++;
			tstate->use_tracing = 0;
			err = (tstate->c_tracefunc)(tstate->c_traceobj, f,
						    PyTrace_LINE, Py_None);
			tstate->use_tracing = (tstate->c_tracefunc
					       || tstate->c_profilefunc);
			tstate->tracing--;
			JUMPTO(f->f_lasti);
			stack_pointer = f->f_stacktop;
			assert(stack_pointer != NULL);
			f->f_stacktop = NULL;
			break;

		case CALL_FUNCTION:
			{
				int na = oparg & 0xff;
				int nk = (oparg>>8) & 0xff;
				int n = na + 2 * nk;
				PyObject **pfunc = stack_pointer - n - 1;
				PyObject *func = *pfunc;
				f->f_lasti = INSTR_OFFSET() - 3;
				if (PyCFunction_Check(func)) 
				{
					int flags = PyCFunction_GET_FLAGS(func);
					if (nk != 0 || (flags & METH_KEYWORDS))
					{
						x = do_call(func, &stack_pointer,
							na, nk);
					}
					else if (flags == METH_VARARGS) 
					{
						PyObject *callargs;
						callargs = load_args(&stack_pointer, na);
						x = PyCFunction_Call(func, callargs, NULL);
						Py_XDECREF(callargs); 
					} 
					else
					{
						x = fast_cfunction(func,
								   &stack_pointer, na);
					}
				} 
				else 
				{
					if (PyMethod_Check(func)
					&& PyMethod_GET_SELF(func) != NULL) 
					{
						PyObject *self = PyMethod_GET_SELF(func);
						Py_INCREF(self);
						func = PyMethod_GET_FUNCTION(func);
						Py_INCREF(func);
						Py_DECREF(*pfunc);
						*pfunc = self;
						na++;
						n++;
					} 
					else
					{
						Py_INCREF(func);
					}
					if (PyFunction_Check(func)) 
					{
						x = fast_function(func, &stack_pointer,
								  n, na, nk);
					} 
					else 
					{
						x = do_call(func, &stack_pointer,
							na, nk);
					}
					Py_DECREF(func);
				}

				while (stack_pointer > pfunc) 
				{
					w = POP();
					Py_DECREF(w);
				}
				PUSH(x);
				if (x != NULL)
				{
					continue;
				}
				break;
			}

		case CALL_FUNCTION_VAR:
		case CALL_FUNCTION_KW:
		case CALL_FUNCTION_VAR_KW:
			{
				int na = oparg & 0xff;
				int nk = (oparg>>8) & 0xff;
				int flags = (opcode - CALL_FUNCTION) & 3;
				int n = na + 2 * nk;
				PyObject **pfunc, *func;
				if (flags & CALL_FLAG_VAR)
				{
					n++;
				}
				if (flags & CALL_FLAG_KW)
				{
					n++;
				}
				pfunc = stack_pointer - n - 1;
				func = *pfunc;
				f->f_lasti = INSTR_OFFSET() - 3;

				if (PyMethod_Check(func)
				&& PyMethod_GET_SELF(func) != NULL) 
				{
					PyObject *self = PyMethod_GET_SELF(func);
					Py_INCREF(self);
					func = PyMethod_GET_FUNCTION(func);
					Py_INCREF(func);
					Py_DECREF(*pfunc);
					*pfunc = self;
					na++;
					n++;
				} 
				else
				{
					Py_INCREF(func);
				}
				x = ext_do_call(func, &stack_pointer, flags, na, nk);
				Py_DECREF(func);

				while (stack_pointer > pfunc) 
				{
					w = POP();
					Py_DECREF(w);
				}
				PUSH(x);
				if (x != NULL)
				{
					continue;
				}
				break;
			}

		case MAKE_FUNCTION:
			v = POP();
			x = PyFunction_New(v, f->f_globals);
			Py_DECREF(v);
			if (x != NULL && oparg > 0) 
			{
				v = PyTuple_New(oparg);
				if (v == NULL) 
				{
					Py_DECREF(x);
					x = NULL;
					break;
				}
				while (--oparg >= 0) 
				{
					w = POP();
					PyTuple_SET_ITEM(v, oparg, w);
				}
				err = PyFunction_SetDefaults(x, v);
				Py_DECREF(v);
			}
			PUSH(x);
			break;

		case MAKE_CLOSURE:
			{
				int nfree;
				v = POP();
				x = PyFunction_New(v, f->f_globals);
				nfree = PyCode_GetNumFree((PyCodeObject *)v);
				Py_DECREF(v);
				if (x != NULL && nfree > 0) 
				{
					v = PyTuple_New(nfree);
					if (v == NULL) 
					{
						Py_DECREF(x);
						x = NULL;
						break;
					}
					while (--nfree >= 0) 
					{
						w = POP();
						PyTuple_SET_ITEM(v, nfree, w);
					}
					err = PyFunction_SetClosure(x, v);
					Py_DECREF(v);
				}
				if (x != NULL && oparg > 0) 
				{
					v = PyTuple_New(oparg);
					if (v == NULL) 
					{
						Py_DECREF(x);
						x = NULL;
						break;
					}
					while (--oparg >= 0) 
					{
						w = POP();
						PyTuple_SET_ITEM(v, oparg, w);
					}
					err = PyFunction_SetDefaults(x, v);
					Py_DECREF(v);
				}
				PUSH(x);
				break;
			}

		case BUILD_SLICE:
			if (oparg == 3)
			{
				w = POP();
			}
			else
			{
				w = NULL;
			}
			v = POP();
			u = POP();
			x = PySlice_New(u, v, w);
			Py_DECREF(u);
			Py_DECREF(v);
			Py_XDECREF(w);
			PUSH(x);
			if (x != NULL) 
			{
				continue;
			}
			break;

		case EXTENDED_ARG:
			opcode = NEXTOP();
			oparg = oparg<<16 | NEXTARG();
			goto dispatch_opcode;

		default:
			fprintf(stderr,
				"XXX lineno: %d, opcode: %d\n",
				f->f_lineno, opcode);
			PyErr_SetString(PyExc_SystemError, "unknown opcode");
			why = WHY_EXCEPTION;
			break;
		}

on_error:
		if (why == WHY_NOT) 
		{
			if (err == 0 && x != NULL) 
			{
#ifdef _DEBUG
				if (PyErr_Occurred())
				{
					fprintf(stderr,
						"XXX undetected error\n");
				}
				else
#endif
				{
					continue;
				}
			}
			why = WHY_EXCEPTION;
			x = Py_None;
			err = 0;
		}

		if (why == WHY_EXCEPTION || why == WHY_RERAISE) 
		{
			if (!PyErr_Occurred()) 
			{
				PyErr_SetString(PyExc_SystemError,
					"error return without exception set");
				why = WHY_EXCEPTION;
			}
		}
#ifdef _DEBUG
		else 
		{
			if (PyErr_Occurred()) 
			{
				fprintf(stderr,
					"XXX undetected error (why=%d)\n",
					why);
				why = WHY_EXCEPTION;
			}
		}
#endif
		if (why == WHY_EXCEPTION) 
		{
			f->f_lasti = INSTR_OFFSET() - 1;
			if (HAS_ARG(opcode))
			{
				f->f_lasti -= 2;
			}
			PyTraceBack_Here(f);

			if (tstate->c_tracefunc != NULL)
			{
				call_exc_trace(tstate->c_tracefunc,
					       tstate->c_traceobj, f);
			}
		}

		if (why == WHY_RERAISE)
		{
			why = WHY_EXCEPTION;
		}


		while (why != WHY_NOT && why != WHY_YIELD && f->f_iblock > 0) 
		{
			PyTryBlock *b = PyFrame_BlockPop(f);

			if (b->b_type == SETUP_LOOP && why == WHY_CONTINUE) 
			{
				PyFrame_BlockSetup(f, b->b_type, b->b_handler,
						   b->b_level);
				why = WHY_NOT;
				JUMPTO(PyInt_AS_LONG(retval));
				Py_DECREF(retval);
				break;
			}

			while (STACK_LEVEL() > b->b_level) 
			{
				v = POP();
				Py_XDECREF(v);
			}
			if (b->b_type == SETUP_LOOP && why == WHY_BREAK) 
			{
				why = WHY_NOT;
				JUMPTO(b->b_handler);
				break;
			}
			if (b->b_type == SETUP_FINALLY ||
			    (b->b_type == SETUP_EXCEPT &&
			     why == WHY_EXCEPTION)) 
			{
				if (why == WHY_EXCEPTION) 
				{
					PyObject *exc, *val, *tb;
					PyErr_Fetch(&exc, &val, &tb);
					if (val == NULL) 
					{
						val = Py_None;
						Py_INCREF(val);
					}
					if (b->b_type == SETUP_EXCEPT) 
					{
						PyErr_NormalizeException(
							&exc, &val, &tb);
						set_exc_info(tstate,
							     exc, val, tb);
					}
					if (tb == NULL) 
					{
						Py_INCREF(Py_None);
						PUSH(Py_None);
					} else
						PUSH(tb);
					PUSH(val);
					PUSH(exc);
				}
				else {
					if (why == WHY_RETURN ||
					    why == WHY_CONTINUE)
					{
						PUSH(retval);
					}
					v = PyInt_FromLong((long)why);
					PUSH(v);
				}
				why = WHY_NOT;
				JUMPTO(b->b_handler);
				break;
			}
		}

		if (why != WHY_NOT)
		{
			break;
		}
	}

	if (why != WHY_YIELD) 
	{
		while (!EMPTY()) 
		{
			v = POP();
			Py_XDECREF(v);
		}
	}

	if (why != WHY_RETURN && why != WHY_YIELD)
	{
		retval = NULL;
	}

	if (tstate->use_tracing) 
	{
		if (tstate->c_tracefunc
		    && (why == WHY_RETURN || why == WHY_YIELD)) 
		{
			if (call_trace(tstate->c_tracefunc,
				       tstate->c_traceobj, f,
				       PyTrace_RETURN, retval)) 
			{
				Py_XDECREF(retval);
				retval = NULL;
				why = WHY_EXCEPTION;
			}
		}
		if (tstate->c_profilefunc) 
		{
			if (why == WHY_EXCEPTION)
			{
				call_trace_protected(tstate->c_profilefunc,
						     tstate->c_profileobj, f,
						     PyTrace_RETURN);
			}
			else if (call_trace(tstate->c_profilefunc,
					    tstate->c_profileobj, f,
					    PyTrace_RETURN, retval)) 
			{
				Py_XDECREF(retval);
				retval = NULL;
				why = WHY_EXCEPTION;
			}
		}
	}

	reset_exc_info(tstate);

	--tstate->recursion_depth;
	tstate->frame = f->f_back;

	return retval;
}

PyObject *PyEval_EvalCodeEx(PyCodeObject *co, PyObject *globals, PyObject *locals,
	   PyObject **args, int argcount, PyObject **kws, int kwcount,
	   PyObject **defs, int defcount, PyObject *closure)
{
	PyFrameObject *f;
	PyObject *retval = NULL;
	PyObject **fastlocals, **freevars;
	PyThreadState *tstate = PyThreadState_GET();
	PyObject *x, *u;

	if (globals == NULL) 
	{
		PyErr_SetString(PyExc_SystemError, 
				"PyEval_EvalCodeEx: NULL globals");
		return NULL;
	}

	f = PyFrame_New(tstate,
			co,
			globals, locals);
	if (f == NULL)
	{
		return NULL;
	}

	fastlocals = f->f_localsplus;
	freevars = f->f_localsplus + f->f_nlocals;

	if (co->co_argcount > 0 ||
	    co->co_flags & (CO_VARARGS | CO_VARKEYWORDS)) 
	{
		int i;
		int n = argcount;
		PyObject *kwdict = NULL;
		if (co->co_flags & CO_VARKEYWORDS) 
		{
			kwdict = PyDict_New();
			if (kwdict == NULL)
			{
				goto fail;
			}
			i = co->co_argcount;
			if (co->co_flags & CO_VARARGS)
			{
				i++;
			}
			SETLOCAL(i, kwdict);
		}
		if (argcount > co->co_argcount) 
		{
			if (!(co->co_flags & CO_VARARGS)) 
			{
				PyErr_Format(PyExc_TypeError,
				    "%.200s() takes %s %d "
				    "%sargument%s (%d given)",
				    PyString_AsString(co->co_name),
				    defcount ? "at most" : "exactly",
				    co->co_argcount,
				    kwcount ? "non-keyword " : "",
				    co->co_argcount == 1 ? "" : "s",
				    argcount);
				goto fail;
			}
			n = co->co_argcount;
		}
		for (i = 0; i < n; i++) 
		{
			x = args[i];
			Py_INCREF(x);
			SETLOCAL(i, x);
		}
		if (co->co_flags & CO_VARARGS) 
		{
			u = PyTuple_New(argcount - n);
			if (u == NULL)
			{
				goto fail;
			}
			SETLOCAL(co->co_argcount, u);
			for (i = n; i < argcount; i++) 
			{
				x = args[i];
				Py_INCREF(x);
				PyTuple_SET_ITEM(u, i-n, x);
			}
		}
		for (i = 0; i < kwcount; i++) 
		{
			PyObject *keyword = kws[2*i];
			PyObject *value = kws[2*i + 1];
			int j;
			if (keyword == NULL || !PyString_Check(keyword)) 
			{
				PyErr_Format(PyExc_TypeError,
				    "%.200s() keywords must be strings",
				    PyString_AsString(co->co_name));
				goto fail;
			}
			for (j = 0; j < co->co_argcount; j++) 
			{
				PyObject *nm = PyTuple_GET_ITEM(
					co->co_varnames, j);
				int cmp = PyObject_RichCompareBool(
					keyword, nm, Py_EQ);
				if (cmp > 0)
				{
					break;
				}
				else if (cmp < 0)
				{
					goto fail;
				}
			}
			if (PyErr_Occurred())
			{
				goto fail;
			}
			if (j >= co->co_argcount) 
			{
				if (kwdict == NULL) 
				{
					PyErr_Format(PyExc_TypeError,
					    "%.200s() got an unexpected "
					    "keyword argument '%.400s'",
					    PyString_AsString(co->co_name),
					    PyString_AsString(keyword));
					goto fail;
				}
				PyDict_SetItem(kwdict, keyword, value);
			}
			else 
			{
				if (GETLOCAL(j) != NULL) 
				{
					PyErr_Format(PyExc_TypeError,
					     "%.200s() got multiple "
					     "values for keyword "
					     "argument '%.400s'",
					     PyString_AsString(co->co_name),
					     PyString_AsString(keyword));
					goto fail;
				}
				Py_INCREF(value);
				SETLOCAL(j, value);
			}
		}
		if (argcount < co->co_argcount) 
		{
			int m = co->co_argcount - defcount;
			for (i = argcount; i < m; i++) 
			{
				if (GETLOCAL(i) == NULL) 
				{
					PyErr_Format(PyExc_TypeError,
					    "%.200s() takes %s %d "
					    "%sargument%s (%d given)",
					    PyString_AsString(co->co_name),
					    ((co->co_flags & CO_VARARGS) ||
					     defcount) ? "at least"
						       : "exactly",
					    m, kwcount ? "non-keyword " : "",
					    m == 1 ? "" : "s", i);
					goto fail;
				}
			}
			if (n > m)
			{
				i = n - m;
			}
			else
			{
				i = 0;
			}
			for (; i < defcount; i++) 
			{
				if (GETLOCAL(m+i) == NULL) 
				{
					PyObject *def = defs[i];
					Py_INCREF(def);
					SETLOCAL(m+i, def);
				}
			}
		}
	}
	else 
	{
		if (argcount > 0 || kwcount > 0) 
		{
			PyErr_Format(PyExc_TypeError,
				     "%.200s() takes no arguments (%d given)",
				     PyString_AsString(co->co_name),
				     argcount + kwcount);
			goto fail;
		}
	}
	if (f->f_ncells) 
	{
		int i = 0, j = 0, nargs, found;
		char *cellname, *argname;
		PyObject *c;

		nargs = co->co_argcount;
		if (co->co_flags & CO_VARARGS)
		{
			nargs++;
		}
		if (co->co_flags & CO_VARKEYWORDS)
		{
			nargs++;
		}

		for (i = 0; i < f->f_ncells && j < nargs; ++i) 
		{
			cellname = PyString_AS_STRING(
				PyTuple_GET_ITEM(co->co_cellvars, i));
			found = 0;
			while (j < nargs) 
			{
				argname = PyString_AS_STRING(
					PyTuple_GET_ITEM(co->co_varnames, j));
				if (strcmp(cellname, argname) == 0) 
				{
					c = PyCell_New(GETLOCAL(j));
					if (c == NULL)
					{
						goto fail;
					}
					GETLOCAL(f->f_nlocals + i) = c;
					found = 1;
					break;
				}
				j++;
			}
			if (found == 0) 
			{
				c = PyCell_New(NULL);
				if (c == NULL)
				{
					goto fail;
				}
				SETLOCAL(f->f_nlocals + i, c);
			}
		}
		while (i < f->f_ncells) 
		{
			c = PyCell_New(NULL);
			if (c == NULL)
			{
				goto fail;
			}
			SETLOCAL(f->f_nlocals + i, c);
			i++;
		}
	}
	if (f->f_nfreevars) 
	{
		int i;
		for (i = 0; i < f->f_nfreevars; ++i) 
		{
			PyObject *o = PyTuple_GET_ITEM(closure, i);
			Py_INCREF(o);
			freevars[f->f_ncells + i] = o;
		}
	}

	if (co->co_flags & CO_GENERATOR) 
	{
		Py_XDECREF(f->f_back);
		f->f_back = NULL;

		return gen_new(f);
	}

    retval = eval_frame(f);

fail:
	assert(tstate != NULL);
	++tstate->recursion_depth;
    Py_DECREF(f);
	--tstate->recursion_depth;
	return retval;
}


static void set_exc_info(PyThreadState *tstate,
	     PyObject *type, PyObject *value, PyObject *tb)
{
	PyFrameObject *frame;
	PyObject *tmp_type, *tmp_value, *tmp_tb;

	frame = tstate->frame;
	if (frame->f_exc_type == NULL) 
	{
		if (tstate->exc_type == NULL) 
		{
			Py_INCREF(Py_None);
			tstate->exc_type = Py_None;
		}
		tmp_type = frame->f_exc_type;
		tmp_value = frame->f_exc_value;
		tmp_tb = frame->f_exc_traceback;
		Py_XINCREF(tstate->exc_type);
		Py_XINCREF(tstate->exc_value);
		Py_XINCREF(tstate->exc_traceback);
		frame->f_exc_type = tstate->exc_type;
		frame->f_exc_value = tstate->exc_value;
		frame->f_exc_traceback = tstate->exc_traceback;
		Py_XDECREF(tmp_type);
		Py_XDECREF(tmp_value);
		Py_XDECREF(tmp_tb);
	}
	tmp_type = tstate->exc_type;
	tmp_value = tstate->exc_value;
	tmp_tb = tstate->exc_traceback;
	Py_XINCREF(type);
	Py_XINCREF(value);
	Py_XINCREF(tb);
	tstate->exc_type = type;
	tstate->exc_value = value;
	tstate->exc_traceback = tb;
	Py_XDECREF(tmp_type);
	Py_XDECREF(tmp_value);
	Py_XDECREF(tmp_tb);
	PySys_SetObject("exc_type", type);
	PySys_SetObject("exc_value", value);
	PySys_SetObject("exc_traceback", tb);
}

static void reset_exc_info(PyThreadState *tstate)
{
	PyFrameObject *frame;
	PyObject *tmp_type, *tmp_value, *tmp_tb;
	frame = tstate->frame;
	if (frame->f_exc_type != NULL) 
	{
		tmp_type = tstate->exc_type;
		tmp_value = tstate->exc_value;
		tmp_tb = tstate->exc_traceback;
		Py_XINCREF(frame->f_exc_type);
		Py_XINCREF(frame->f_exc_value);
		Py_XINCREF(frame->f_exc_traceback);
		tstate->exc_type = frame->f_exc_type;
		tstate->exc_value = frame->f_exc_value;
		tstate->exc_traceback = frame->f_exc_traceback;
		Py_XDECREF(tmp_type);
		Py_XDECREF(tmp_value);
		Py_XDECREF(tmp_tb);
		PySys_SetObject("exc_type", frame->f_exc_type);
		PySys_SetObject("exc_value", frame->f_exc_value);
		PySys_SetObject("exc_traceback", frame->f_exc_traceback);
	}
	tmp_type = frame->f_exc_type;
	tmp_value = frame->f_exc_value;
	tmp_tb = frame->f_exc_traceback;
	frame->f_exc_type = NULL;
	frame->f_exc_value = NULL;
	frame->f_exc_traceback = NULL;
	Py_XDECREF(tmp_type);
	Py_XDECREF(tmp_value);
	Py_XDECREF(tmp_tb);
}

static enum why_code do_raise(PyObject *type, PyObject *value, PyObject *tb)
{
	if (type == NULL) 
	{
		PyThreadState *tstate = PyThreadState_Get();
		type = tstate->exc_type == NULL ? Py_None : tstate->exc_type;
		value = tstate->exc_value;
		tb = tstate->exc_traceback;
		Py_XINCREF(type);
		Py_XINCREF(value);
		Py_XINCREF(tb);
	}

	if (tb == Py_None) 
	{
		Py_DECREF(tb);
		tb = NULL;
	}
	else if (tb != NULL && !PyTraceBack_Check(tb)) 
	{
		PyErr_SetString(PyExc_TypeError,
			   "raise: arg 3 must be a traceback or None");
		goto raise_error;
	}

	if (value == NULL) 
	{
		value = Py_None;
		Py_INCREF(value);
	}

	while (PyTuple_Check(type) && PyTuple_Size(type) > 0) 
	{
		PyObject *tmp = type;
		type = PyTuple_GET_ITEM(type, 0);
		Py_INCREF(type);
		Py_DECREF(tmp);
	}

	if (PyString_Check(type))
	{
		;
	}
	else if (PyClass_Check(type))
	{
		PyErr_NormalizeException(&type, &value, &tb);
	}
	else if (PyInstance_Check(type)) 
	{
		if (value != Py_None) 
		{
			PyErr_SetString(PyExc_TypeError,
				"instance exception may not have a separate value");
			goto raise_error;
		}
		else 
		{
			Py_DECREF(value);
			value = type;
			type = (PyObject*) ((PyInstanceObject*)type)->in_class;
			Py_INCREF(type);
		}
	}
	else 
	{
		PyErr_Format(PyExc_TypeError,
			     "exceptions must be strings, classes, or "
			     "instances, not %s", type->ob_type->tp_name);
		goto raise_error;
	}
	PyErr_Restore(type, value, tb);
	if (tb == NULL)
	{
		return WHY_EXCEPTION;
	}
	else
	{
		return WHY_RERAISE;
	}
raise_error:
	Py_XDECREF(value);
	Py_XDECREF(type);
	Py_XDECREF(tb);
	return WHY_EXCEPTION;
}

static int unpack_iterable(PyObject *v, int argcnt, PyObject **sp)
{
	int i = 0;
	PyObject *it;
	PyObject *w;

	assert(v != NULL);

	it = PyObject_GetIter(v);
	if (it == NULL)
	{
		goto Error;
	}

	for (; i < argcnt; i++) 
	{
		w = PyIter_Next(it);
		if (w == NULL) 
		{
			if (!PyErr_Occurred()) 
			{
				PyErr_Format(PyExc_ValueError,
					"need more than %d value%s to unpack",
					i, i == 1 ? "" : "s");
			}
			goto Error;
		}
		*--sp = w;
	}

	w = PyIter_Next(it);
	if (w == NULL) 
	{
		if (PyErr_Occurred())
		{
			goto Error;
		}
		Py_DECREF(it);
		return 1;
	}
	Py_DECREF(w);
	PyErr_SetString(PyExc_ValueError, "too many values to unpack");
Error:
	for (; i > 0; i--, sp++)
	{
		Py_DECREF(*sp);
	}
	Py_XDECREF(it);
	return 0;
}


#ifdef _DEBUG
static int prtrace(PyObject *v, char *str)
{
	printf("%s ", str);
	if (PyObject_Print(v, stdout, 0) != 0)
	{
		PyErr_Clear();
	}
	printf("\n");
	return 1;
}
#endif

static void call_exc_trace(Py_tracefunc func, PyObject *self, PyFrameObject *f)
{
	PyObject *type, *value, *traceback, *arg;
	int err;
	PyErr_Fetch(&type, &value, &traceback);
	if (value == NULL) 
	{
		value = Py_None;
		Py_INCREF(value);
	}
	arg = Py_BuildValue("(OOO)", type, value, traceback);
	if (arg == NULL) 
	{
		PyErr_Restore(type, value, traceback);
		return;
	}
	err = call_trace(func, self, f, PyTrace_EXCEPTION, arg);
	Py_DECREF(arg);
	if (err == 0)
	{
		PyErr_Restore(type, value, traceback);
	}
	else 
	{
		Py_XDECREF(type);
		Py_XDECREF(value);
		Py_XDECREF(traceback);
	}
}

static void call_trace_protected(Py_tracefunc func, PyObject *obj, PyFrameObject *frame,
		     int what)
{
	PyObject *type, *value, *traceback;
	int err;
	PyErr_Fetch(&type, &value, &traceback);
	err = call_trace(func, obj, frame, what, NULL);
	if (err == 0)
	{
		PyErr_Restore(type, value, traceback);
	}
	else 
	{
		Py_XDECREF(type);
		Py_XDECREF(value);
		Py_XDECREF(traceback);
	}
}

static int call_trace(Py_tracefunc func, PyObject *obj, PyFrameObject *frame,
	   int what, PyObject *arg)
{
	PyThreadState *tstate = frame->f_tstate;
	int result;
	if (tstate->tracing)
	{
		return 0;
	}
	tstate->tracing++;
	tstate->use_tracing = 0;
	result = func(obj, frame, what, arg);
	tstate->use_tracing = ((tstate->c_tracefunc != NULL)
			       || (tstate->c_profilefunc != NULL));
	tstate->tracing--;
	return result;
}

void PyEval_SetProfile(Py_tracefunc func, PyObject *arg)
{
	PyThreadState *tstate = PyThreadState_Get();
	PyObject *temp = tstate->c_profileobj;
	Py_XINCREF(arg);
	tstate->c_profilefunc = NULL;
	tstate->c_profileobj = NULL;
	tstate->use_tracing = tstate->c_tracefunc != NULL;
	Py_XDECREF(temp);
	tstate->c_profilefunc = func;
	tstate->c_profileobj = arg;
	tstate->use_tracing = (func != NULL) || (tstate->c_tracefunc != NULL);
}

void PyEval_SetTrace(Py_tracefunc func, PyObject *arg)
{
	PyThreadState *tstate = PyThreadState_Get();
	PyObject *temp = tstate->c_traceobj;
	Py_XINCREF(arg);
	tstate->c_tracefunc = NULL;
	tstate->c_traceobj = NULL;
	tstate->use_tracing = tstate->c_profilefunc != NULL;
	Py_XDECREF(temp);
	tstate->c_tracefunc = func;
	tstate->c_traceobj = arg;
	tstate->use_tracing = ((func != NULL)
			       || (tstate->c_profilefunc != NULL));
}

PyObject *PyEval_GetBuiltins()
{
	PyFrameObject *current_frame = (PyFrameObject *)PyEval_GetFrame();
	if (current_frame == NULL)
	{
		return PyThreadState_Get()->interp->builtins;
	}
	else
	{
		return current_frame->f_builtins;
	}
}

PyObject *PyEval_GetLocals()
{
	PyFrameObject *current_frame = (PyFrameObject *)PyEval_GetFrame();
	if (current_frame == NULL)
	{
		return NULL;
	}
	PyFrame_FastToLocals(current_frame);
	return current_frame->f_locals;
}

PyObject *PyEval_GetGlobals()
{
	PyFrameObject *current_frame = (PyFrameObject *)PyEval_GetFrame();
	if (current_frame == NULL)
	{
		return NULL;
	}
	else
	{
		return current_frame->f_globals;
	}
}

PyObject *PyEval_GetFrame()
{
	PyThreadState *tstate = PyThreadState_Get();
	return _PyThreadState_GetFrame((PyObject *)tstate);
}

int PyEval_GetRestricted()
{
	PyFrameObject *current_frame = (PyFrameObject *)PyEval_GetFrame();
	return current_frame == NULL ? 0 : current_frame->f_restricted;
}

int PyEval_MergeCompilerFlags(PyCompilerFlags *cf)
{
	PyFrameObject *current_frame = (PyFrameObject *)PyEval_GetFrame();
	int result = 0;

	if (current_frame != NULL) 
	{
		const int codeflags = current_frame->f_code->co_flags;
		const int compilerflags = codeflags & PyCF_MASK;
		if (compilerflags) 
		{
			result = 1;
			cf->cf_flags |= compilerflags;
		}
	}
	return result;
}

int Py_FlushLine()
{
	PyObject *f = PySys_GetObject("stdout");
	if (f == NULL)
	{
		return 0;
	}
	if (!PyFile_SoftSpace(f, 0))
	{
		return 0;
	}
	return PyFile_WriteString("\n", f);
}

#undef PyEval_CallObject

PyObject *PyEval_CallObject(PyObject *func, PyObject *arg)
{
	return PyEval_CallObjectWithKeywords(func, arg, (PyObject *)NULL);
}

#define PyEval_CallObject(func,arg) \
        PyEval_CallObjectWithKeywords(func, arg, (PyObject *)NULL)

PyObject *PyEval_CallObjectWithKeywords(PyObject *func, PyObject *arg, PyObject *kw)
{
	PyObject *result;

	if (arg == NULL)
	{
		arg = PyTuple_New(0);
	}
	else if (!PyTuple_Check(arg)) 
	{
		PyErr_SetString(PyExc_TypeError,
				"argument list must be a tuple");
		return NULL;
	}
	else
	{
		Py_INCREF(arg);
	}

	if (kw != NULL && !PyDict_Check(kw)) 
	{
		PyErr_SetString(PyExc_TypeError,
				"keyword list must be a dictionary");
		Py_DECREF(arg);
		return NULL;
	}

	result = PyObject_Call(func, arg, kw);
	Py_DECREF(arg);
	return result;
}

char *PyEval_GetFuncName(PyObject *func)
{
	if (PyMethod_Check(func))
	{
		return PyEval_GetFuncName(PyMethod_GET_FUNCTION(func));
	}
	else if (PyFunction_Check(func))
	{
		return PyString_AsString(((PyFunctionObject*)func)->func_name);
	}
	else if (PyCFunction_Check(func))
	{
		return ((PyCFunctionObject*)func)->m_ml->ml_name;
	}
	else if (PyClass_Check(func))
	{
		return PyString_AsString(((PyClassObject*)func)->cl_name);
	}
	else if (PyInstance_Check(func)) 
	{
		return PyString_AsString(
			((PyInstanceObject*)func)->in_class->cl_name);
	} 
	else 
	{
		return func->ob_type->tp_name;
	}
}

char *PyEval_GetFuncDesc(PyObject *func)
{
	if (PyMethod_Check(func))
	{
		return "()";
	}
	else if (PyFunction_Check(func))
	{
		return "()";
	}
	else if (PyCFunction_Check(func))
	{
		return "()";
	}
	else if (PyClass_Check(func))
	{
		return " constructor";
	}
	else if (PyInstance_Check(func)) 
	{
		return " instance";
	} 
	else 
	{
		return " object";
	}
}

#define EXT_POP(STACK_POINTER) (*--(STACK_POINTER))

static PyObject *fast_cfunction(PyObject *func, PyObject ***pp_stack, int na)
{
	PyCFunction meth = PyCFunction_GET_FUNCTION(func);
	PyObject *self = PyCFunction_GET_SELF(func);
	int flags = PyCFunction_GET_FLAGS(func);

	switch (flags) 
	{
	case METH_OLDARGS:
		if (na == 0)
		{
			return (*meth)(self, NULL);
		}
		else if (na == 1) 
		{
			PyObject *arg = EXT_POP(*pp_stack);
			PyObject *result =  (*meth)(self, arg);
			Py_DECREF(arg);
			return result;
		} 
		else 
		{
			PyObject *args = load_args(pp_stack, na);
			PyObject *result = (*meth)(self, args);
			Py_DECREF(args);
			return result;
		}

	case METH_NOARGS:
		if (na == 0)
		{
			return (*meth)(self, NULL);
		}
		PyErr_Format(PyExc_TypeError,
			     "%.200s() takes no arguments (%d given)",
			     ((PyCFunctionObject*)func)->m_ml->ml_name, na);
		return NULL;

	case METH_O:
		if (na == 1) 
		{
			PyObject *arg = EXT_POP(*pp_stack);
			PyObject *result = (*meth)(self, arg);
			Py_DECREF(arg);
			return result;
		}
		PyErr_Format(PyExc_TypeError,
			     "%.200s() takes exactly one argument (%d given)",
			     ((PyCFunctionObject*)func)->m_ml->ml_name, na);
		return NULL;
	
	default:
		fprintf(stderr, "%.200s() flags = %d\n", 
			((PyCFunctionObject*)func)->m_ml->ml_name, flags);
		PyErr_BadInternalCall();
		return NULL;
	}		
}

static PyObject *fast_function(PyObject *func, PyObject ***pp_stack, int n, int na, int nk)
{
	PyObject *co = PyFunction_GET_CODE(func);
	PyObject *globals = PyFunction_GET_GLOBALS(func);
	PyObject *argdefs = PyFunction_GET_DEFAULTS(func);
	PyObject *closure = PyFunction_GET_CLOSURE(func);
	PyObject **d = NULL;
	int nd = 0;

	if (argdefs != NULL) 
	{
		d = &PyTuple_GET_ITEM(argdefs, 0);
		nd = ((PyTupleObject *)argdefs)->ob_size;
	}
	return PyEval_EvalCodeEx((PyCodeObject *)co, globals,
			  (PyObject *)NULL, (*pp_stack)-n, na,
			  (*pp_stack)-2*nk, nk, d, nd,
			  closure);
}

static PyObject *update_keyword_args(PyObject *orig_kwdict, int nk, PyObject ***pp_stack,
                    PyObject *func)
{
	PyObject *kwdict = NULL;
	if (orig_kwdict == NULL)
	{
		kwdict = PyDict_New();
	}
	else 
	{
		kwdict = PyDict_Copy(orig_kwdict);
		Py_DECREF(orig_kwdict);
	}
	if (kwdict == NULL)
	{
		return NULL;
	}
	while (--nk >= 0) 
	{
		int err;
		PyObject *value = EXT_POP(*pp_stack);
		PyObject *key = EXT_POP(*pp_stack);
		if (PyDict_GetItem(kwdict, key) != NULL) 
		{
            PyErr_Format(PyExc_TypeError,
                         "%.200s%s got multiple values "
                         "for keyword argument '%.200s'",
			PyEval_GetFuncName(func),
			PyEval_GetFuncDesc(func),
			PyString_AsString(key));
			Py_DECREF(key);
			Py_DECREF(value);
			Py_DECREF(kwdict);
			return NULL;
		}
		err = PyDict_SetItem(kwdict, key, value);
		Py_DECREF(key);
		Py_DECREF(value);
		if (err) 
		{
			Py_DECREF(kwdict);
			return NULL;
		}
	}
	return kwdict;
}

static PyObject *update_star_args(int nstack, int nstar, PyObject *stararg,
		 PyObject ***pp_stack)
{
	PyObject *callargs, *w;

	callargs = PyTuple_New(nstack + nstar);
	if (callargs == NULL) 
	{
		return NULL;
	}
	if (nstar) 
	{
		int i;
		for (i = 0; i < nstar; i++) 
		{
			PyObject *a = PyTuple_GET_ITEM(stararg, i);
			Py_INCREF(a);
			PyTuple_SET_ITEM(callargs, nstack + i, a);
		}
	}
	while (--nstack >= 0) 
	{
		w = EXT_POP(*pp_stack);
		PyTuple_SET_ITEM(callargs, nstack, w);
	}
	return callargs;
}

static PyObject *load_args(PyObject ***pp_stack, int na)
{
	PyObject *args = PyTuple_New(na);
	PyObject *w;

	if (args == NULL)
	{
		return NULL;
	}
	while (--na >= 0) 
	{
		w = EXT_POP(*pp_stack);
		PyTuple_SET_ITEM(args, na, w);
	}
	return args;
}

static PyObject *do_call(PyObject *func, PyObject ***pp_stack, int na, int nk)
{
	PyObject *callargs = NULL;
	PyObject *kwdict = NULL;
	PyObject *result = NULL;

	if (nk > 0) 
	{
		kwdict = update_keyword_args(NULL, nk, pp_stack, func);
		if (kwdict == NULL)
		{
			goto call_fail;
		}
	}
	callargs = load_args(pp_stack, na);
	if (callargs == NULL)
	{
		goto call_fail;
	}
	result = PyObject_Call(func, callargs, kwdict);
call_fail:
	Py_XDECREF(callargs);
	Py_XDECREF(kwdict);
	return result;
}

static PyObject *ext_do_call(PyObject *func, PyObject ***pp_stack, int flags, int na, int nk)
{
	int nstar = 0;
	PyObject *callargs = NULL;
	PyObject *stararg = NULL;
	PyObject *kwdict = NULL;
	PyObject *result = NULL;

	if (flags & CALL_FLAG_KW) 
	{
		kwdict = EXT_POP(*pp_stack);
		if (!(kwdict && PyDict_Check(kwdict))) 
		{
			PyErr_Format(PyExc_TypeError,
				     "%s%s argument after ** "
				     "must be a dictionary",
				     PyEval_GetFuncName(func),
				     PyEval_GetFuncDesc(func));
			goto ext_call_fail;
		}
	}
	if (flags & CALL_FLAG_VAR) 
	{
		stararg = EXT_POP(*pp_stack);
		if (!PyTuple_Check(stararg)) 
		{
			PyObject *t = NULL;
			t = PySequence_Tuple(stararg);
			if (t == NULL) 
			{
				if (PyErr_ExceptionMatches(PyExc_TypeError)) 
				{
					PyErr_Format(PyExc_TypeError,
						     "%s%s argument after * "
						     "must be a sequence",
						     PyEval_GetFuncName(func),
						     PyEval_GetFuncDesc(func));
				}
				goto ext_call_fail;
			}
			Py_DECREF(stararg);
			stararg = t;
		}
		nstar = PyTuple_GET_SIZE(stararg);
	}
	if (nk > 0) 
	{
		kwdict = update_keyword_args(kwdict, nk, pp_stack, func);
		if (kwdict == NULL)
		{
			goto ext_call_fail;
		}
	}
	callargs = update_star_args(na, nstar, stararg, pp_stack);
	if (callargs == NULL)
	{
		goto ext_call_fail;
	}
	result = PyObject_Call(func, callargs, kwdict);
ext_call_fail:
	Py_XDECREF(callargs);
	Py_XDECREF(kwdict);
	Py_XDECREF(stararg);
	return result;
}

#define SLICE_ERROR_MSG \
	"standard sequence type does not support step size other than one"

static PyObject *loop_subscript(PyObject *v, PyObject *w)
{
	PySequenceMethods *sq = v->ob_type->tp_as_sequence;
	int i;
	if (sq == NULL || sq->sq_item == NULL) 
	{
		PyErr_SetString(PyExc_TypeError, "loop over non-sequence");
		return NULL;
	}
	i = PyInt_AsLong(w);
	v = (*sq->sq_item)(v, i);
	if (v)
	{
		return v;
	}
	if (PyErr_ExceptionMatches(PyExc_IndexError))
	{
		PyErr_Clear();
	}
	return NULL;
}

int _PyEval_SliceIndex(PyObject *v, int *pi)
{
	if (v != NULL) 
	{
		long x;
		if (PyInt_Check(v)) 
		{
			x = PyInt_AsLong(v);
		} 
		else if (PyLong_Check(v)) 
		{
			x = PyLong_AsLong(v);
			if (x==-1 && PyErr_Occurred()) 
			{
				PyObject *long_zero;
				int cmp;

				if (!PyErr_ExceptionMatches(
					PyExc_OverflowError)) 
				{
					return 0;
				}

				PyErr_Clear();

				long_zero = PyLong_FromLong(0L);
				if (long_zero == NULL)
				{
					return 0;
				}

				cmp = PyObject_RichCompareBool(v, long_zero,
							       Py_GT);
				Py_DECREF(long_zero);
				if (cmp < 0)
				{
					return 0;
				}
				else if (cmp > 0)
				{
					x = INT_MAX;
				}
				else
				{
					x = 0;
				}
			}
		} 
		else 
		{
			PyErr_SetString(PyExc_TypeError,
					"slice indices must be integers");
			return 0;
		}
		if (x > INT_MAX)
		{
			x = INT_MAX;
		}
		else if (x < -INT_MAX)
		{
			x = 0;
		}
		*pi = x;
	}
	return 1;
}

#undef ISINT
#define ISINT(x) ((x) == NULL || PyInt_Check(x) || PyLong_Check(x))

static PyObject *apply_slice(PyObject *u, PyObject *v, PyObject *w)
{
	PyTypeObject *tp = u->ob_type;
	PySequenceMethods *sq = tp->tp_as_sequence;

	if (sq && sq->sq_slice && ISINT(v) && ISINT(w)) 
	{
		int ilow = 0, ihigh = INT_MAX;
		if (!_PyEval_SliceIndex(v, &ilow))
		{
			return NULL;
		}
		if (!_PyEval_SliceIndex(w, &ihigh))
		{
			return NULL;
		}
		return PySequence_GetSlice(u, ilow, ihigh);
	}
	else 
	{
		PyObject *slice = PySlice_New(v, w, NULL);
		if (slice != NULL) 
		{
			PyObject *res = PyObject_GetItem(u, slice);
			Py_DECREF(slice);
			return res;
		}
		else
		{
			return NULL;
		}
	}
}

static int assign_slice(PyObject *u, PyObject *v, PyObject *w, PyObject *x)
{
	PyTypeObject *tp = u->ob_type;
	PySequenceMethods *sq = tp->tp_as_sequence;

	if (sq && sq->sq_slice && ISINT(v) && ISINT(w)) 
	{
		int ilow = 0, ihigh = INT_MAX;
		if (!_PyEval_SliceIndex(v, &ilow))
		{
			return -1;
		}
		if (!_PyEval_SliceIndex(w, &ihigh))
		{
			return -1;
		}
		if (x == NULL)
		{
			return PySequence_DelSlice(u, ilow, ihigh);
		}
		else
		{
			return PySequence_SetSlice(u, ilow, ihigh, x);
		}
	}
	else 
	{
		PyObject *slice = PySlice_New(v, w, NULL);
		if (slice != NULL) 
		{
			int res;
			if (x != NULL)
			{
				res = PyObject_SetItem(u, slice, x);
			}
			else
			{
				res = PyObject_DelItem(u, slice);
			}
			Py_DECREF(slice);
			return res;
		}
		else
		{
			return -1;
		}
	}
}

static PyObject *cmp_outcome(int op, PyObject *v, PyObject *w)
{
	int res = 0;
	switch (op) 
	{
	case IS:
	case IS_NOT:
		res = (v == w);
		if (op == (int) IS_NOT)
		{
			res = !res;
		}
		break;
	
	case IN:
	case NOT_IN:
		res = PySequence_Contains(w, v);
		if (res < 0)
		{
			return NULL;
		}
		if (op == (int) NOT_IN)
		{
			res = !res;
		}
		break;
	
	case EXC_MATCH:
		res = PyErr_GivenExceptionMatches(v, w);
		break;

	default:
		return PyObject_RichCompare(v, w, op);
	}
	v = res ? Py_True : Py_False;
	Py_INCREF(v);
	return v;
}

static PyObject *import_from(PyObject *v, PyObject *name)
{
	PyObject *x;

	x = PyObject_GetAttr(v, name);
	if (x == NULL && PyErr_ExceptionMatches(PyExc_AttributeError)) 
	{
		PyErr_Format(PyExc_ImportError,
			     "cannot import name %.230s",
			     PyString_AsString(name));
	}
	return x;
}

static int import_all_from(PyObject *locals, PyObject *v)
{
	PyObject *all = PyObject_GetAttrString(v, "__all__");
	PyObject *dict, *name, *value;
	int skip_leading_underscores = 0;
	int pos, err;

	if (all == NULL) 
	{
		if (!PyErr_ExceptionMatches(PyExc_AttributeError))
		{
			return -1;
		}
		PyErr_Clear();
		dict = PyObject_GetAttrString(v, "__dict__");
		if (dict == NULL) 
		{
			if (!PyErr_ExceptionMatches(PyExc_AttributeError))
			{
				return -1;
			}
			PyErr_SetString(PyExc_ImportError,
				"from-import-* object has no __dict__ and no __all__");
			return -1;
		}
		all = PyMapping_Keys(dict);
		Py_DECREF(dict);
		if (all == NULL)
		{
			return -1;
		}
		skip_leading_underscores = 1;
	}

	for (pos = 0, err = 0; ; pos++) 
	{
		name = PySequence_GetItem(all, pos);
		if (name == NULL) 
		{
			if (!PyErr_ExceptionMatches(PyExc_IndexError))
			{
				err = -1;
			}
			else
			{
				PyErr_Clear();
			}
			break;
		}
		if (skip_leading_underscores &&
		    PyString_Check(name) &&
		    PyString_AS_STRING(name)[0] == '_')
		{
			Py_DECREF(name);
			continue;
		}
		value = PyObject_GetAttr(v, name);
		if (value == NULL)
		{
			err = -1;
		}
		else
		{
			err = PyDict_SetItem(locals, name, value);
		}
		Py_DECREF(name);
		Py_XDECREF(value);
		if (err != 0)
		{
			break;
		}
	}
	Py_DECREF(all);
	return err;
}

static PyObject *build_class(PyObject *methods, PyObject *bases, PyObject *name)
{
	PyObject *metaclass = NULL, *result, *base;

	if (PyDict_Check(methods))
	{
		metaclass = PyDict_GetItemString(methods, "__metaclass__");
	}
	if (metaclass != NULL)
	{
		Py_INCREF(metaclass);
	}
	else if (PyTuple_Check(bases) && PyTuple_GET_SIZE(bases) > 0) 
	{
		base = PyTuple_GET_ITEM(bases, 0);
		metaclass = PyObject_GetAttrString(base, "__class__");
		if (metaclass == NULL) 
		{
			PyErr_Clear();
			metaclass = (PyObject *)base->ob_type;
			Py_INCREF(metaclass);
		}
	}
	else 
	{
		PyObject *g = PyEval_GetGlobals();
		if (g != NULL && PyDict_Check(g))
		{
			metaclass = PyDict_GetItemString(g, "__metaclass__");
		}
		if (metaclass == NULL)
		{
			metaclass = (PyObject *) &PyClass_Type;
		}
		Py_INCREF(metaclass);
	}
	result = PyObject_CallFunction(metaclass, "OOO", name, bases, methods);
	Py_DECREF(metaclass);
	return result;
}

static int exec_statement(PyFrameObject *f, PyObject *prog, PyObject *globals,
	       PyObject *locals)
{
	int n;
	PyObject *v;
	int plain = 0;

	if (PyTuple_Check(prog) && globals == Py_None && locals == Py_None &&
	    ((n = PyTuple_Size(prog)) == 2 || n == 3)) 
	{
		globals = PyTuple_GetItem(prog, 1);
		if (n == 3)
		{
			locals = PyTuple_GetItem(prog, 2);
		}
		prog = PyTuple_GetItem(prog, 0);
	}
	if (globals == Py_None) 
	{
		globals = PyEval_GetGlobals();
		if (locals == Py_None) 
		{
			locals = PyEval_GetLocals();
			plain = 1;
		}
	}
	else if (locals == Py_None)
	{
		locals = globals;
	}
	if (!PyString_Check(prog) &&
	    !PyUnicode_Check(prog) &&
	    !PyCode_Check(prog) &&
	    !PyFile_Check(prog)) 
	{
		PyErr_SetString(PyExc_TypeError,
			"exec: arg 1 must be a string, file, or code object");
		return -1;
	}
	if (!PyDict_Check(globals)) 
	{
		PyErr_SetString(PyExc_TypeError,
		    "exec: arg 2 must be a dictionary or None");
		return -1;
	}
	if (!PyDict_Check(locals)) 
	{
		PyErr_SetString(PyExc_TypeError,
		    "exec: arg 3 must be a dictionary or None");
		return -1;
	}
	if (PyDict_GetItemString(globals, "__builtins__") == NULL)
	{
		PyDict_SetItemString(globals, "__builtins__", f->f_builtins);
	}
	if (PyCode_Check(prog)) 
	{
		if (PyCode_GetNumFree((PyCodeObject *)prog) > 0) 
		{
			PyErr_SetString(PyExc_TypeError,
				"code object passed to exec may not contain free variables");
			return -1;
		}
		v = PyEval_EvalCode((PyCodeObject *) prog, globals, locals);
	}
	else if (PyFile_Check(prog)) 
	{
		FILE *fp = PyFile_AsFile(prog);
		char *name = PyString_AsString(PyFile_Name(prog));
		PyCompilerFlags cf;
		cf.cf_flags = 0;
		if (PyEval_MergeCompilerFlags(&cf))
		{
			v = PyRun_FileFlags(fp, name, Py_file_input, globals,
					    locals, &cf); 
		}
		else
		{
			v = PyRun_File(fp, name, Py_file_input, globals,
				       locals); 
		}
	}
	else 
	{
		char *str;
		PyCompilerFlags cf;
		if (PyString_AsStringAndSize(prog, &str, NULL))
		{
			return -1;
		}
		cf.cf_flags = 0;
		if (PyEval_MergeCompilerFlags(&cf))
		{
			v = PyRun_StringFlags(str, Py_file_input, globals, 
					      locals, &cf);
		}
		else
		{
			v = PyRun_String(str, Py_file_input, globals, locals);
		}
	}
	if (plain)
	{
		PyFrame_LocalsToFast(f, 0);
	}
	if (v == NULL)
	{
		return -1;
	}
	Py_DECREF(v);
	return 0;
}

static void format_exc_check_arg(PyObject *exc, char *format_str, PyObject *obj)
{
	char *obj_str;

	if (!obj)
	{
		return;
	}

	obj_str = PyString_AsString(obj);
	if (!obj_str)
	{
		return;
	}

	PyErr_Format(exc, format_str, obj_str);
}

