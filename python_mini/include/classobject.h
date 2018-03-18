//20180318
#pragma once

typedef struct {
    PyObject_HEAD
    PyObject *cl_bases;
    PyObject *cl_dict;
    PyObject *cl_name;
    PyObject *cl_getattr;
    PyObject *cl_setattr;
    PyObject *cl_delattr;
} PyClassObject;

typedef struct {
    PyObject_HEAD
    PyClassObject *in_class;
    PyObject *in_dict;
    PyObject *in_weakreflist;
} PyInstanceObject;

typedef struct {
    PyObject_HEAD
    PyObject *im_func;
    PyObject *im_self; 
    PyObject *im_class;
    PyObject *im_weakreflist;
} PyMethodObject;

extern DL_IMPORT(PyTypeObject) PyClass_Type, PyInstance_Type, PyMethod_Type;

#define PyClass_Check(op) ((op)->ob_type == &PyClass_Type)
#define PyInstance_Check(op) ((op)->ob_type == &PyInstance_Type)
#define PyMethod_Check(op) ((op)->ob_type == &PyMethod_Type)

extern DL_IMPORT(PyObject *) PyClass_New(PyObject *, PyObject *, PyObject *);
extern DL_IMPORT(PyObject *) PyInstance_New(PyObject *, PyObject *,
                                            PyObject *);
extern DL_IMPORT(PyObject *) PyInstance_NewRaw(PyObject *, PyObject *);
extern DL_IMPORT(PyObject *) PyMethod_New(PyObject *, PyObject *, PyObject *);

extern DL_IMPORT(PyObject *) PyMethod_Function(PyObject *);
extern DL_IMPORT(PyObject *) PyMethod_Self(PyObject *);
extern DL_IMPORT(PyObject *) PyMethod_Class(PyObject *);

#define PyMethod_GET_FUNCTION(meth) \
        (((PyMethodObject *)meth) -> im_func)
#define PyMethod_GET_SELF(meth) \
	(((PyMethodObject *)meth) -> im_self)
#define PyMethod_GET_CLASS(meth) \
	(((PyMethodObject *)meth) -> im_class)

extern DL_IMPORT(int) PyClass_IsSubclass(PyObject *, PyObject *);
