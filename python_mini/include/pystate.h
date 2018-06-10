//20180324
#pragma once

struct _ts; 
struct _is; 

typedef struct _is {

    struct _is *next;
    struct _ts *tstate_head;

    PyObject *modules;
    PyObject *sysdict;
    PyObject *builtins;

    int checkinterval;
} PyInterpreterState;


struct _frame; 

typedef int (*Py_tracefunc)(PyObject *, struct _frame *, int, PyObject *);

#define PyTrace_CALL 0
#define PyTrace_EXCEPTION 1
#define PyTrace_LINE 2
#define PyTrace_RETURN 3

typedef struct _ts {

    struct _ts *next;
    PyInterpreterState *interp;

    struct _frame *frame;
    int recursion_depth;
    int ticker;
    int tracing;
    int use_tracing;

    Py_tracefunc c_profilefunc;
    Py_tracefunc c_tracefunc;
    PyObject *c_profileobj;
    PyObject *c_traceobj;

    PyObject *curexc_type;
    PyObject *curexc_value;
    PyObject *curexc_traceback;

    PyObject *exc_type;
    PyObject *exc_value;
    PyObject *exc_traceback;

    PyObject *dict;

    int tick_counter;

} PyThreadState;


DL_IMPORT(PyInterpreterState *) PyInterpreterState_New();
DL_IMPORT(void) PyInterpreterState_Clear(PyInterpreterState *);
DL_IMPORT(void) PyInterpreterState_Delete(PyInterpreterState *);

DL_IMPORT(PyThreadState *) PyThreadState_New(PyInterpreterState *);
DL_IMPORT(void) PyThreadState_Clear(PyThreadState *);
DL_IMPORT(void) PyThreadState_Delete(PyThreadState *);
DL_IMPORT(void) PyThreadState_DeleteCurrent();

DL_IMPORT(PyThreadState *) PyThreadState_Get();
DL_IMPORT(PyThreadState *) PyThreadState_Swap(PyThreadState *);
DL_IMPORT(PyObject *) PyThreadState_GetDict();



extern DL_IMPORT(PyThreadState *) _PyThreadState_Current;

#ifdef _DEBUG
#define PyThreadState_GET() PyThreadState_Get()
#else
#define PyThreadState_GET() (_PyThreadState_Current)
#endif

DL_IMPORT(PyInterpreterState *) PyInterpreterState_Head();
DL_IMPORT(PyInterpreterState *) PyInterpreterState_Next(PyInterpreterState *);
DL_IMPORT(PyThreadState *) PyInterpreterState_ThreadHead(PyInterpreterState *);
DL_IMPORT(PyThreadState *) PyThreadState_Next(PyThreadState *);

extern DL_IMPORT(unaryfunc) _PyThreadState_GetFrame;
