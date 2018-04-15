//20180318
#pragma once

typedef struct {
    PyObject_HEAD
    long ob_ival;
} PyIntObject;

extern DL_IMPORT(PyTypeObject) PyInt_Type;

#define PyInt_Check(op) PyObject_TypeCheck(op, &PyInt_Type)
#define PyInt_CheckExact(op) ((op)->ob_type == &PyInt_Type)

extern DL_IMPORT(PyObject *) PyInt_FromString(char*, char**, int);
extern DL_IMPORT(PyObject *) PyInt_FromUnicode(Py_UNICODE*, int, int);
extern DL_IMPORT(PyObject *) PyInt_FromLong(long);
extern DL_IMPORT(long) PyInt_AsLong(PyObject *);
extern DL_IMPORT(long) PyInt_GetMax();

extern DL_IMPORT(PyIntObject) _Py_ZeroStruct, _Py_TrueStruct;

#define Py_False ((PyObject *) &_Py_ZeroStruct)
#define Py_True ((PyObject *) &_Py_TrueStruct)

#define PyInt_AS_LONG(op) (((PyIntObject *)(op))->ob_ival)

extern DL_IMPORT(unsigned long) PyOS_strtoul(char *, char **, int);
extern DL_IMPORT(long) PyOS_strtol(char *, char **, int);
