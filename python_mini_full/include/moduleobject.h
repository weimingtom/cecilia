//20180318
#pragma once

extern DL_IMPORT(PyTypeObject) PyModule_Type;

#define PyModule_Check(op) PyObject_TypeCheck(op, &PyModule_Type)
#define PyModule_CheckExact(op) ((op)->ob_type == &PyModule_Type)

extern DL_IMPORT(PyObject *) PyModule_New(char *);
extern DL_IMPORT(PyObject *) PyModule_GetDict(PyObject *);
extern DL_IMPORT(char *) PyModule_GetName(PyObject *);
extern DL_IMPORT(char *) PyModule_GetFilename(PyObject *);
extern DL_IMPORT(void) _PyModule_Clear(PyObject *);
