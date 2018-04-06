//20180318
#pragma once

typedef struct _longobject PyLongObject;

extern DL_IMPORT(PyTypeObject) PyLong_Type;

#define PyLong_Check(op) PyObject_TypeCheck(op, &PyLong_Type)
#define PyLong_CheckExact(op) ((op)->ob_type == &PyLong_Type)

extern DL_IMPORT(PyObject *) PyLong_FromLong(long);
extern DL_IMPORT(PyObject *) PyLong_FromUnsignedLong(unsigned long);
extern DL_IMPORT(PyObject *) PyLong_FromDouble(double);
extern DL_IMPORT(long) PyLong_AsLong(PyObject *);
extern DL_IMPORT(unsigned long) PyLong_AsUnsignedLong(PyObject *);

extern DL_IMPORT(double) _PyLong_AsScaledDouble(PyObject *vv, int *e);

extern DL_IMPORT(double) PyLong_AsDouble(PyObject *);
extern DL_IMPORT(PyObject *) PyLong_FromVoidPtr(void *);
extern DL_IMPORT(void *) PyLong_AsVoidPtr(PyObject *);

#ifdef HAVE_LONG_LONG

#ifndef ULONG_MAX
#define ULONG_MAX 4294967295U
#endif
#ifndef LONGLONG_MAX
#define LONGLONG_MAX 9223372036854775807LL
#endif
#ifndef ULONGLONG_MAX
#define ULONGLONG_MAX 0xffffffffffffffffULL
#endif

extern DL_IMPORT(PyObject *) PyLong_FromLongLong(LONG_LONG);
extern DL_IMPORT(PyObject *) PyLong_FromUnsignedLongLong(unsigned LONG_LONG);
extern DL_IMPORT(LONG_LONG) PyLong_AsLongLong(PyObject *);
extern DL_IMPORT(unsigned LONG_LONG) PyLong_AsUnsignedLongLong(PyObject *);
#endif

DL_IMPORT(PyObject *) PyLong_FromString(char *, char **, int);
#ifdef Py_USING_UNICODE
DL_IMPORT(PyObject *) PyLong_FromUnicode(Py_UNICODE*, int, int);
#endif

extern DL_IMPORT(PyObject *) _PyLong_FromByteArray(
	const unsigned char* bytes, size_t n,
	int little_endian, int is_signed);

extern DL_IMPORT(int) _PyLong_AsByteArray(PyLongObject* v,
	unsigned char* bytes, size_t n,
	int little_endian, int is_signed);
