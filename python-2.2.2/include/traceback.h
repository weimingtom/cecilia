#pragma once

/* Traceback interface */

struct _frame;

DL_IMPORT(int) PyTraceBack_Here(struct _frame *);
DL_IMPORT(int) PyTraceBack_Print(PyObject *, PyObject *);

/* Reveal traceback type so we can typecheck traceback objects */
extern DL_IMPORT(PyTypeObject) PyTraceBack_Type;
#define PyTraceBack_Check(v) ((v)->ob_type == &PyTraceBack_Type)
