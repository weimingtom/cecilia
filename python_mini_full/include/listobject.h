//20180318
#pragma once

typedef struct {
    PyObject_VAR_HEAD
    PyObject **ob_item;
} PyListObject;

extern DL_IMPORT(PyTypeObject) PyList_Type;

#define PyList_Check(op) PyObject_TypeCheck(op, &PyList_Type)
#define PyList_CheckExact(op) ((op)->ob_type == &PyList_Type)

extern DL_IMPORT(PyObject *) PyList_New(int size);
extern DL_IMPORT(int) PyList_Size(PyObject *);
extern DL_IMPORT(PyObject *) PyList_GetItem(PyObject *, int);
extern DL_IMPORT(int) PyList_SetItem(PyObject *, int, PyObject *);
extern DL_IMPORT(int) PyList_Insert(PyObject *, int, PyObject *);
extern DL_IMPORT(int) PyList_Append(PyObject *, PyObject *);
extern DL_IMPORT(PyObject *) PyList_GetSlice(PyObject *, int, int);
extern DL_IMPORT(int) PyList_SetSlice(PyObject *, int, int, PyObject *);
extern DL_IMPORT(int) PyList_Sort(PyObject *);
extern DL_IMPORT(int) PyList_Reverse(PyObject *);
extern DL_IMPORT(PyObject *) PyList_AsTuple(PyObject *);

#define PyList_GET_ITEM(op, i) (((PyListObject *)(op))->ob_item[i])
#define PyList_SET_ITEM(op, i, v) (((PyListObject *)(op))->ob_item[i] = (v))
#define PyList_GET_SIZE(op)    (((PyListObject *)(op))->ob_size)
