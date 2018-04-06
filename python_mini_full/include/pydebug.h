//20180324
#pragma once

extern DL_IMPORT(int) Py_DebugFlag;
extern DL_IMPORT(int) Py_VerboseFlag;
extern DL_IMPORT(int) Py_InteractiveFlag;
extern DL_IMPORT(int) Py_OptimizeFlag;
extern DL_IMPORT(int) Py_NoSiteFlag;
extern DL_IMPORT(int) Py_UseClassExceptionsFlag;
extern DL_IMPORT(int) Py_FrozenFlag;
extern DL_IMPORT(int) Py_TabcheckFlag;
extern DL_IMPORT(int) Py_UnicodeFlag;
extern DL_IMPORT(int) Py_IgnoreEnvironmentFlag;
extern DL_IMPORT(int) Py_DivisionWarningFlag;

extern DL_IMPORT(int) _Py_QnewFlag;

#define Py_GETENV(s) (Py_IgnoreEnvironmentFlag ? NULL : getenv(s))

DL_IMPORT(void) Py_FatalError(char *message);
