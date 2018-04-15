//20180324
#pragma once

DL_IMPORT(void) PyErr_SetNone(PyObject *);
DL_IMPORT(void) PyErr_SetObject(PyObject *, PyObject *);
DL_IMPORT(void) PyErr_SetString(PyObject *, const char *);
DL_IMPORT(PyObject *) PyErr_Occurred();
DL_IMPORT(void) PyErr_Clear();
DL_IMPORT(void) PyErr_Fetch(PyObject **, PyObject **, PyObject **);
DL_IMPORT(void) PyErr_Restore(PyObject *, PyObject *, PyObject *);

DL_IMPORT(int) PyErr_GivenExceptionMatches(PyObject *, PyObject *);
DL_IMPORT(int) PyErr_ExceptionMatches(PyObject *);
DL_IMPORT(void) PyErr_NormalizeException(PyObject**, PyObject**, PyObject**);


extern DL_IMPORT(PyObject *) PyExc_Exception;
extern DL_IMPORT(PyObject *) PyExc_StopIteration;
extern DL_IMPORT(PyObject *) PyExc_StandardError;
extern DL_IMPORT(PyObject *) PyExc_ArithmeticError;
extern DL_IMPORT(PyObject *) PyExc_LookupError;

extern DL_IMPORT(PyObject *) PyExc_AssertionError;
extern DL_IMPORT(PyObject *) PyExc_AttributeError;
extern DL_IMPORT(PyObject *) PyExc_EOFError;
extern DL_IMPORT(PyObject *) PyExc_FloatingPointError;
extern DL_IMPORT(PyObject *) PyExc_EnvironmentError;
extern DL_IMPORT(PyObject *) PyExc_IOError;
extern DL_IMPORT(PyObject *) PyExc_OSError;
extern DL_IMPORT(PyObject *) PyExc_ImportError;
extern DL_IMPORT(PyObject *) PyExc_IndexError;
extern DL_IMPORT(PyObject *) PyExc_KeyError;
extern DL_IMPORT(PyObject *) PyExc_KeyboardInterrupt;
extern DL_IMPORT(PyObject *) PyExc_MemoryError;
extern DL_IMPORT(PyObject *) PyExc_NameError;
extern DL_IMPORT(PyObject *) PyExc_OverflowError;
extern DL_IMPORT(PyObject *) PyExc_RuntimeError;
extern DL_IMPORT(PyObject *) PyExc_NotImplementedError;
extern DL_IMPORT(PyObject *) PyExc_SyntaxError;
extern DL_IMPORT(PyObject *) PyExc_IndentationError;
extern DL_IMPORT(PyObject *) PyExc_TabError;
extern DL_IMPORT(PyObject *) PyExc_ReferenceError;
extern DL_IMPORT(PyObject *) PyExc_SystemError;
extern DL_IMPORT(PyObject *) PyExc_SystemExit;
extern DL_IMPORT(PyObject *) PyExc_TypeError;
extern DL_IMPORT(PyObject *) PyExc_UnboundLocalError;
extern DL_IMPORT(PyObject *) PyExc_UnicodeError;
extern DL_IMPORT(PyObject *) PyExc_ValueError;
extern DL_IMPORT(PyObject *) PyExc_ZeroDivisionError;
#ifdef MS_WINDOWS
extern DL_IMPORT(PyObject *) PyExc_WindowsError;
#endif

extern DL_IMPORT(PyObject *) PyExc_MemoryErrorInst;

extern DL_IMPORT(PyObject *) PyExc_Warning;
extern DL_IMPORT(PyObject *) PyExc_UserWarning;
extern DL_IMPORT(PyObject *) PyExc_DeprecationWarning;
extern DL_IMPORT(PyObject *) PyExc_SyntaxWarning;
extern DL_IMPORT(PyObject *) PyExc_OverflowWarning;
extern DL_IMPORT(PyObject *) PyExc_RuntimeWarning;


extern DL_IMPORT(int) PyErr_BadArgument();
extern DL_IMPORT(PyObject *) PyErr_NoMemory();
extern DL_IMPORT(PyObject *) PyErr_SetFromErrno(PyObject *);
extern DL_IMPORT(PyObject *) PyErr_SetFromErrnoWithFilename(PyObject *, char *);
extern DL_IMPORT(PyObject *) PyErr_Format(PyObject *, const char *, ...)
			__attribute__((format(printf, 2, 3)));
#ifdef MS_WINDOWS
extern DL_IMPORT(PyObject *) PyErr_SetFromWindowsErrWithFilename(int, const char *);
extern DL_IMPORT(PyObject *) PyErr_SetFromWindowsErr(int);
#endif

extern DL_IMPORT(void) PyErr_BadInternalCall();
extern DL_IMPORT(void) _PyErr_BadInternalCall(char *filename, int lineno);
#define PyErr_BadInternalCall() _PyErr_BadInternalCall(__FILE__, __LINE__)

DL_IMPORT(PyObject *) PyErr_NewException(char *name, PyObject *base,
                                         PyObject *dict);
extern DL_IMPORT(void) PyErr_WriteUnraisable(PyObject *);

extern DL_IMPORT(int) PyErr_Warn(PyObject *, char *);
extern DL_IMPORT(int) PyErr_WarnExplicit(PyObject *, char *,
					 char *, int, char *, PyObject *);

extern DL_IMPORT(int) PyErr_CheckSignals();
extern DL_IMPORT(void) PyErr_SetInterrupt();

extern DL_IMPORT(void) PyErr_SyntaxLocation(char *, int);
extern DL_IMPORT(PyObject *) PyErr_ProgramText(char *, int);

#if !defined(HAVE_SNPRINTF)
# define HAVE_SNPRINTF
# define snprintf _snprintf
# define vsnprintf _vsnprintf
#endif

#include <stdarg.h>
extern DL_IMPORT(int) PyOS_snprintf(char *str, size_t size, const char  *format, ...)
			__attribute__((format(printf, 3, 4)));
extern DL_IMPORT(int) PyOS_vsnprintf(char *str, size_t size, const char  *format, va_list va)
			__attribute__((format(printf, 3, 0)));

