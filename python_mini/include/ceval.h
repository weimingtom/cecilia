//20180318
#pragma once

DL_IMPORT(PyObject *) PyEval_CallObjectWithKeywords
	(PyObject *, PyObject *, PyObject *);

#undef PyEval_CallObject
DL_IMPORT(PyObject *) PyEval_CallObject(PyObject *, PyObject *);

#define PyEval_CallObject(func,arg) \
        PyEval_CallObjectWithKeywords(func, arg, (PyObject *)NULL)

DL_IMPORT(PyObject *) PyEval_CallFunction(PyObject *obj, char *format, ...);
DL_IMPORT(PyObject *) PyEval_CallMethod(PyObject *obj,
                                        char *methodname, char *format, ...);

DL_IMPORT(void) PyEval_SetProfile(Py_tracefunc, PyObject *);
DL_IMPORT(void) PyEval_SetTrace(Py_tracefunc, PyObject *);

DL_IMPORT(PyObject *) PyEval_GetBuiltins();
DL_IMPORT(PyObject *) PyEval_GetGlobals();
DL_IMPORT(PyObject *) PyEval_GetLocals();
DL_IMPORT(PyObject *) PyEval_GetOwner();
DL_IMPORT(PyObject *) PyEval_GetFrame();
DL_IMPORT(int) PyEval_GetRestricted();

DL_IMPORT(int) PyEval_MergeCompilerFlags(PyCompilerFlags *cf);

DL_IMPORT(int) Py_FlushLine();

DL_IMPORT(int) Py_AddPendingCall(int (*func)(void *), void *arg);
DL_IMPORT(int) Py_MakePendingCalls();

DL_IMPORT(void) Py_SetRecursionLimit(int);
DL_IMPORT(int) Py_GetRecursionLimit();

DL_IMPORT(char *) PyEval_GetFuncName(PyObject *);
DL_IMPORT(char *) PyEval_GetFuncDesc(PyObject *);

extern DL_IMPORT(PyThreadState *) PyEval_SaveThread();
extern DL_IMPORT(void) PyEval_RestoreThread(PyThreadState *);

extern DL_IMPORT(void) PyEval_InitThreads();
extern DL_IMPORT(void) PyEval_AcquireLock();
extern DL_IMPORT(void) PyEval_ReleaseLock();
extern DL_IMPORT(void) PyEval_AcquireThread(PyThreadState *tstate);
extern DL_IMPORT(void) PyEval_ReleaseThread(PyThreadState *tstate);
extern DL_IMPORT(void) PyEval_ReInitThreads();

#define Py_BEGIN_ALLOW_THREADS { \
			PyThreadState *_save; \
			_save = PyEval_SaveThread();
#define Py_BLOCK_THREADS	PyEval_RestoreThread(_save);
#define Py_UNBLOCK_THREADS	_save = PyEval_SaveThread();
#define Py_END_ALLOW_THREADS	PyEval_RestoreThread(_save); \
		 }

extern DL_IMPORT(int) _PyEval_SliceIndex(PyObject *, int *);
