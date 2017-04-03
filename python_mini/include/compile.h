#pragma once

/* Bytecode object */
typedef struct {
    PyObject_HEAD
    int co_argcount;		/* #arguments, except *args */
    int co_nlocals;		/* #local variables */
    int co_stacksize;		/* #entries needed for evaluation stack */
    int co_flags;		/* CO_..., see below */
    PyObject *co_code;		/* instruction opcodes */
    PyObject *co_consts;	/* list (constants used) */
    PyObject *co_names;		/* list of strings (names used) */
    PyObject *co_varnames;	/* tuple of strings (local variable names) */
    PyObject *co_freevars;	/* tuple of strings (free variable names) */
    PyObject *co_cellvars;      /* tuple of strings (cell variable names) */
    /* The rest doesn't count for hash/cmp */
    PyObject *co_filename;	/* string (where it was loaded from) */
    PyObject *co_name;		/* string (name, for reference) */
    int co_firstlineno;		/* first source line number */
    PyObject *co_lnotab;	/* string (encoding addr<->lineno mapping) */
} PyCodeObject;

/* Masks for co_flags above */
#define CO_OPTIMIZED	0x0001
#define CO_NEWLOCALS	0x0002
#define CO_VARARGS	0x0004
#define CO_VARKEYWORDS	0x0008
#define CO_NESTED       0x0010
#define CO_GENERATOR    0x0020
/* XXX Temporary hack.  Until generators are a permanent part of the
   language, we need a way for a code object to record that generators
   were *possible* when it was compiled.  This is so code dynamically
   compiled *by* a code object knows whether to allow yield stmts.  In
   effect, this passes on the "from __future__ import generators" state
   in effect when the code block was compiled. */
#define CO_GENERATOR_ALLOWED    0x1000
#define CO_FUTURE_DIVISION    	0x2000

extern DL_IMPORT(PyTypeObject) PyCode_Type;

#define PyCode_Check(op) ((op)->ob_type == &PyCode_Type)
#define PyCode_GetNumFree(op) (PyTuple_GET_SIZE((op)->co_freevars))

#define CO_MAXBLOCKS 20 /* Max static block nesting within a function */

/* Public interface */
struct _node; /* Declare the existence of this type */
DL_IMPORT(PyCodeObject *) PyNode_Compile(struct _node *, char *);
DL_IMPORT(PyCodeObject *) PyCode_New(
	int, int, int, int, PyObject *, PyObject *, PyObject *, PyObject *,
	PyObject *, PyObject *, PyObject *, PyObject *, int, PyObject *); 
        /* same as struct above */
DL_IMPORT(int) PyCode_Addr2Line(PyCodeObject *, int);

/* Future feature support */

typedef struct {
    int ff_found_docstring;
    int ff_last_lineno;
    int ff_features;
} PyFutureFeatures;

DL_IMPORT(PyFutureFeatures *) PyNode_Future(struct _node *, char *);
DL_IMPORT(PyCodeObject *) PyNode_CompileFlags(struct _node *, char *,
					      PyCompilerFlags *);

#define FUTURE_NESTED_SCOPES "nested_scopes"
#define FUTURE_GENERATORS "generators"
#define FUTURE_DIVISION "division"

/* for internal use only */
#define _PyCode_GETCODEPTR(co, pp) \
	((*(co)->co_code->ob_type->tp_as_buffer->bf_getreadbuffer) \
	 ((co)->co_code, 0, (void **)(pp)))

