//20180324
#pragma once

#include <stdarg.h>

#define CACHE_HASH
#define INTERN_STRINGS

typedef struct {
    PyObject_VAR_HEAD
    long ob_shash;
    PyObject *ob_sinterned;
    char ob_sval[1];
} PyStringObject;

extern DL_IMPORT(PyTypeObject) PyString_Type;

#define PyString_Check(op) PyObject_TypeCheck(op, &PyString_Type)
#define PyString_CheckExact(op) ((op)->ob_type == &PyString_Type)

extern DL_IMPORT(PyObject *) PyString_FromStringAndSize(const char *, int);
extern DL_IMPORT(PyObject *) PyString_FromString(const char *);
extern DL_IMPORT(PyObject *) PyString_FromFormatV(const char*, va_list)
				__attribute__((format(printf, 1, 0)));
extern DL_IMPORT(PyObject *) PyString_FromFormat(const char*, ...)
				__attribute__((format(printf, 1, 2)));
extern DL_IMPORT(int) PyString_Size(PyObject *);
extern DL_IMPORT(char *) PyString_AsString(PyObject *);
extern DL_IMPORT(void) PyString_Concat(PyObject **, PyObject *);
extern DL_IMPORT(void) PyString_ConcatAndDel(PyObject **, PyObject *);
extern DL_IMPORT(int) _PyString_Resize(PyObject **, int);
extern DL_IMPORT(int) _PyString_Eq(PyObject *, PyObject*);
extern DL_IMPORT(PyObject *) PyString_Format(PyObject *, PyObject *);
extern DL_IMPORT(PyObject *) _PyString_FormatLong(PyObject*, int, int,
						  int, char**, int*);

extern DL_IMPORT(void) PyString_InternInPlace(PyObject **);
extern DL_IMPORT(PyObject *) PyString_InternFromString(const char *);
extern DL_IMPORT(void) _Py_ReleaseInternedStrings();

#define PyString_AS_STRING(op) (((PyStringObject *)(op))->ob_sval)
#define PyString_GET_SIZE(op)  (((PyStringObject *)(op))->ob_size)

extern DL_IMPORT(PyObject *) _PyString_Join(PyObject *sep, PyObject *x);

extern DL_IMPORT(PyObject*) PyString_Decode(
    const char *s,
    int size,
    const char *encoding,
    const char *errors  
    );

extern DL_IMPORT(PyObject*) PyString_Encode(
    const char *s,
    int size, 
    const char *encoding,
    const char *errors
    );

extern DL_IMPORT(PyObject*) PyString_AsEncodedObject(
    PyObject *str,
    const char *encoding,
    const char *errors
    );

extern DL_IMPORT(PyObject*) PyString_AsEncodedString(
    PyObject *str,
    const char *encoding,
    const char *errors
    );

extern DL_IMPORT(PyObject*) PyString_AsDecodedObject(
    PyObject *str,
    const char *encoding,
    const char *errors
    );

extern DL_IMPORT(PyObject*) PyString_AsDecodedString(
    PyObject *str,	 
    const char *encoding,
    const char *errors	
    );

extern DL_IMPORT(int) PyString_AsStringAndSize(
    PyObject *obj,
    char **s,
    int *len
    );
    
