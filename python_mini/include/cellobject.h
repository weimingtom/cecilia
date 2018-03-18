//20180318
#pragma once

typedef struct {
	PyObject_HEAD
	PyObject *ob_ref;
} PyCellObject;

extern DL_IMPORT(PyTypeObject) PyCell_Type;

#define PyCell_Check(op) ((op)->ob_type == &PyCell_Type)

extern DL_IMPORT(PyObject *) PyCell_New(PyObject *);
extern DL_IMPORT(PyObject *) PyCell_Get(PyObject *);
extern DL_IMPORT(int) PyCell_Set(PyObject *, PyObject *);

#define PyCell_GET(op) (((PyCellObject *)(op))->ob_ref)
#define PyCell_SET(op, v) (((PyCellObject *)(op))->ob_ref = v)
