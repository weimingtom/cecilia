//20180324
#pragma once

#define PyCF_MASK (CO_GENERATOR_ALLOWED | CO_FUTURE_DIVISION)
#define PyCF_MASK_OBSOLETE (CO_NESTED)

typedef struct {
	int cf_flags;
} PyCompilerFlags;

DL_IMPORT(void) Py_SetProgramName(char *);
DL_IMPORT(char *) Py_GetProgramName();

DL_IMPORT(void) Py_SetPythonHome(char *);
DL_IMPORT(char *) Py_GetPythonHome();

DL_IMPORT(void) Py_Initialize();
DL_IMPORT(void) Py_Finalize();
DL_IMPORT(int) Py_IsInitialized();
DL_IMPORT(PyThreadState *) Py_NewInterpreter();
DL_IMPORT(void) Py_EndInterpreter(PyThreadState *);

DL_IMPORT(int) PyRun_AnyFile(FILE *, char *);
DL_IMPORT(int) PyRun_AnyFileEx(FILE *, char *, int);

DL_IMPORT(int) PyRun_AnyFileFlags(FILE *, char *, PyCompilerFlags *);
DL_IMPORT(int) PyRun_AnyFileExFlags(FILE *, char *, int, PyCompilerFlags *);

DL_IMPORT(int) PyRun_SimpleString(char *);
DL_IMPORT(int) PyRun_SimpleStringFlags(char *, PyCompilerFlags *);
DL_IMPORT(int) PyRun_SimpleFile(FILE *, char *);
DL_IMPORT(int) PyRun_SimpleFileEx(FILE *, char *, int);
DL_IMPORT(int) PyRun_SimpleFileExFlags(FILE *, char *, int, PyCompilerFlags *);
DL_IMPORT(int) PyRun_InteractiveOne(FILE *, char *);
DL_IMPORT(int) PyRun_InteractiveOneFlags(FILE *, char *, PyCompilerFlags *);
DL_IMPORT(int) PyRun_InteractiveLoop(FILE *, char *);
DL_IMPORT(int) PyRun_InteractiveLoopFlags(FILE *, char *, PyCompilerFlags *);

DL_IMPORT(struct _node *) PyParser_SimpleParseString(char *, int);
DL_IMPORT(struct _node *) PyParser_SimpleParseFile(FILE *, char *, int);
DL_IMPORT(struct _node *) PyParser_SimpleParseStringFlags(char *, int, int);
DL_IMPORT(struct _node *) PyParser_SimpleParseFileFlags(FILE *, char *, int, int);

DL_IMPORT(PyObject *) PyRun_String(char *, int, PyObject *, PyObject *);
DL_IMPORT(PyObject *) PyRun_File(FILE *, char *, int, PyObject *, PyObject *);
DL_IMPORT(PyObject *) PyRun_FileEx(FILE *, char *, int,
				   PyObject *, PyObject *, int);
DL_IMPORT(PyObject *) PyRun_StringFlags(char *, int, PyObject *, PyObject *,
					PyCompilerFlags *);
DL_IMPORT(PyObject *) PyRun_FileFlags(FILE *, char *, int, PyObject *, 
				      PyObject *, PyCompilerFlags *);
DL_IMPORT(PyObject *) PyRun_FileExFlags(FILE *, char *, int, PyObject *, 
					PyObject *, int, PyCompilerFlags *);

DL_IMPORT(PyObject *) Py_CompileString(char *, char *, int);
DL_IMPORT(PyObject *) Py_CompileStringFlags(char *, char *, int,
					    PyCompilerFlags *);
DL_IMPORT(struct symtable *) Py_SymtableString(char *, char *, int);

DL_IMPORT(void) PyErr_Print();
DL_IMPORT(void) PyErr_PrintEx(int);
DL_IMPORT(void) PyErr_Display(PyObject *, PyObject *, PyObject *);

DL_IMPORT(int) Py_AtExit(void (*func)());

DL_IMPORT(void) Py_Exit(int);

DL_IMPORT(int) Py_FdIsInteractive(FILE *, char *);

DL_IMPORT(char *) Py_GetProgramFullPath();
DL_IMPORT(char *) Py_GetPrefix();
DL_IMPORT(char *) Py_GetExecPrefix();
DL_IMPORT(char *) Py_GetPath();

DL_IMPORT(const char *) Py_GetVersion();
DL_IMPORT(const char *) Py_GetPlatform();
DL_IMPORT(const char *) Py_GetCopyright();
DL_IMPORT(const char *) Py_GetCompiler();
DL_IMPORT(const char *) Py_GetBuildInfo();

DL_IMPORT(PyObject *) _PyBuiltin_Init();
DL_IMPORT(PyObject *) _PySys_Init();
DL_IMPORT(void) _PyImport_Init();
DL_IMPORT(void) _PyExc_Init();

DL_IMPORT(void) _PyExc_Fini();
DL_IMPORT(void) _PyImport_Fini();
DL_IMPORT(void) PyMethod_Fini();
DL_IMPORT(void) PyFrame_Fini();
DL_IMPORT(void) PyCFunction_Fini();
DL_IMPORT(void) PyTuple_Fini();
DL_IMPORT(void) PyString_Fini();
DL_IMPORT(void) PyInt_Fini();
DL_IMPORT(void) PyFloat_Fini();
DL_IMPORT(void) PyOS_FiniInterrupts();

DL_IMPORT(char *) PyOS_Readline(char *);
extern DL_IMPORT(int) (*PyOS_InputHook)();
extern DL_IMPORT(char) *(*PyOS_ReadlineFunctionPointer)(char *);

#define PYOS_STACK_MARGIN 2048

#if defined(WIN32) && !defined(MS_WIN64) && defined(_MSC_VER)
#define USE_STACKCHECK
#endif

#ifdef USE_STACKCHECK
DL_IMPORT(int) PyOS_CheckStack(void);
#endif

typedef void (*PyOS_sighandler_t)(int);
DL_IMPORT(PyOS_sighandler_t) PyOS_getsig(int);
DL_IMPORT(PyOS_sighandler_t) PyOS_setsig(int, PyOS_sighandler_t);
