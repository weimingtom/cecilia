//20180318
#pragma once

extern DL_IMPORT(PyTypeObject) PyCObject_Type;

#define PyCObject_Check(op) ((op)->ob_type == &PyCObject_Type)

extern DL_IMPORT(PyObject *) PyCObject_FromVoidPtr(void *cobj, void (*destruct)(void*));

extern DL_IMPORT(PyObject *) PyCObject_FromVoidPtrAndDesc(void *cobj, void *desc,
                             void (*destruct)(void*,void*));

extern DL_IMPORT(void *) PyCObject_AsVoidPtr(PyObject *);

extern DL_IMPORT(void *) PyCObject_GetDesc(PyObject *);

extern DL_IMPORT(void *) PyCObject_Import(char *module_name, char *cobject_name);
