//20180324
#pragma once


struct _frame;

DL_IMPORT(int) PyTraceBack_Here(struct _frame *);
DL_IMPORT(int) PyTraceBack_Print(PyObject *, PyObject *);

extern DL_IMPORT(PyTypeObject) PyTraceBack_Type;
#define PyTraceBack_Check(v) ((v)->ob_type == &PyTraceBack_Type)
