#pragma once

extern DL_IMPORT(PyTypeObject) PyRange_Type;

#define PyRange_Check(op) ((op)->ob_type == &PyRange_Type)

extern DL_IMPORT(PyObject *) PyRange_New(long, long, long, int);
