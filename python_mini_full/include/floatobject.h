//20180318
#pragma once

typedef struct {
    PyObject_HEAD
    double ob_fval;
} PyFloatObject;

extern DL_IMPORT(PyTypeObject) PyFloat_Type;

#define PyFloat_Check(op) PyObject_TypeCheck(op, &PyFloat_Type)
#define PyFloat_CheckExact(op) ((op)->ob_type == &PyFloat_Type)

extern DL_IMPORT(PyObject *) PyFloat_FromString(PyObject*, char** junk);

extern DL_IMPORT(PyObject *) PyFloat_FromDouble(double);

extern DL_IMPORT(double) PyFloat_AsDouble(PyObject *);
#define PyFloat_AS_DOUBLE(op) (((PyFloatObject *)(op))->ob_fval)

extern DL_IMPORT(void) PyFloat_AsReprString(char*, PyFloatObject *v);

extern DL_IMPORT(void) PyFloat_AsString(char*, PyFloatObject *v);
