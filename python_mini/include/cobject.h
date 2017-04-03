#pragma once

extern DL_IMPORT(PyTypeObject) PyCObject_Type;

#define PyCObject_Check(op) ((op)->ob_type == &PyCObject_Type)

/* Create a PyCObject from a pointer to a C object and an optional
   destructor function.  If the second argument is non-null, then it
   will be called with the first argument if and when the PyCObject is
   destroyed.

*/
extern DL_IMPORT(PyObject *)
PyCObject_FromVoidPtr(void *cobj, void (*destruct)(void*));


/* Create a PyCObject from a pointer to a C object, a description object,
   and an optional destructor function.  If the third argument is non-null,
   then it will be called with the first and second arguments if and when 
   the PyCObject is destroyed.
*/
extern DL_IMPORT(PyObject *)
PyCObject_FromVoidPtrAndDesc(void *cobj, void *desc,
                             void (*destruct)(void*,void*));

/* Retrieve a pointer to a C object from a PyCObject. */
extern DL_IMPORT(void *)
PyCObject_AsVoidPtr(PyObject *);

/* Retrieve a pointer to a description object from a PyCObject. */
extern DL_IMPORT(void *)
PyCObject_GetDesc(PyObject *);

/* Import a pointer to a C object from a module using a PyCObject. */
extern DL_IMPORT(void *)
PyCObject_Import(char *module_name, char *cobject_name);
