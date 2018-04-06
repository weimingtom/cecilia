//20180318
#pragma once

typedef struct {
    int b_type;	
    int b_handler;
    int b_level;
} PyTryBlock;

typedef struct _frame {
    PyObject_VAR_HEAD
    struct _frame *f_back;
    PyCodeObject *f_code;
    PyObject *f_builtins;
    PyObject *f_globals;
    PyObject *f_locals;
    PyObject **f_valuestack;
    PyObject **f_stacktop;
    PyObject *f_trace;
    PyObject *f_exc_type, *f_exc_value, *f_exc_traceback;
    PyThreadState *f_tstate;
    int f_lasti;
    int f_lineno;
    int f_restricted;
    int f_iblock;
    PyTryBlock f_blockstack[CO_MAXBLOCKS];
    int f_nlocals;
    int f_ncells;
    int f_nfreevars;
    int f_stacksize;
    PyObject *f_localsplus[1];
} PyFrameObject;

extern DL_IMPORT(PyTypeObject) PyFrame_Type;

#define PyFrame_Check(op) ((op)->ob_type == &PyFrame_Type)

DL_IMPORT(PyFrameObject *) PyFrame_New(PyThreadState *, PyCodeObject *,
                                       PyObject *, PyObject *);

#ifndef Py_DEBUG
#define GETITEM(v, i) PyTuple_GET_ITEM((PyTupleObject *)(v), (i))
#define GETITEMNAME(v, i) PyString_AS_STRING((PyStringObject *)GETITEM((v), (i)))
#else
#define GETITEM(v, i) PyTuple_GetItem((v), (i))
#define GETITEMNAME(v, i) PyString_AsString(GETITEM(v, i))
#endif

#define GETUSTRINGVALUE(s) ((unsigned char *)PyString_AS_STRING(s))

#define Getconst(f, i)	(GETITEM((f)->f_code->co_consts, (i)))
#define Getname(f, i)	(GETITEMNAME((f)->f_code->co_names, (i)))
#define Getnamev(f, i)	(GETITEM((f)->f_code->co_names, (i)))

DL_IMPORT(void) PyFrame_BlockSetup(PyFrameObject *, int, int, int);
DL_IMPORT(PyTryBlock *) PyFrame_BlockPop(PyFrameObject *);

DL_IMPORT(PyObject **) PyFrame_ExtendStack(PyFrameObject *, int, int);

DL_IMPORT(void) PyFrame_LocalsToFast(PyFrameObject *, int);
DL_IMPORT(void) PyFrame_FastToLocals(PyFrameObject *);
