#pragma once

extern DL_IMPORT(PyTypeObject) PyBuffer_Type;

#define PyBuffer_Check(op) ((op)->ob_type == &PyBuffer_Type)

#define Py_END_OF_BUFFER	(-1)

extern DL_IMPORT(PyObject *) PyBuffer_FromObject(PyObject *base,
                                                 int offset, int size);
extern DL_IMPORT(PyObject *) PyBuffer_FromReadWriteObject(PyObject *base,
                                                          int offset,
                                                          int size);

extern DL_IMPORT(PyObject *) PyBuffer_FromMemory(void *ptr, int size);
extern DL_IMPORT(PyObject *) PyBuffer_FromReadWriteMemory(void *ptr, int size);

extern DL_IMPORT(PyObject *) PyBuffer_New(int size);
