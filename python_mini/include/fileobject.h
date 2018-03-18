//20180318
#pragma once

extern DL_IMPORT(PyTypeObject) PyFile_Type;

#define PyFile_Check(op) PyObject_TypeCheck(op, &PyFile_Type)
#define PyFile_CheckExact(op) ((op)->ob_type == &PyFile_Type)

extern DL_IMPORT(PyObject *) PyFile_FromString(char *, char *);
extern DL_IMPORT(void) PyFile_SetBufSize(PyObject *, int);
extern DL_IMPORT(PyObject *) PyFile_FromFile(FILE *, char *, char *,
                                             int (*)(FILE *));
extern DL_IMPORT(FILE *) PyFile_AsFile(PyObject *);
extern DL_IMPORT(PyObject *) PyFile_Name(PyObject *);
extern DL_IMPORT(PyObject *) PyFile_GetLine(PyObject *, int);
extern DL_IMPORT(int) PyFile_WriteObject(PyObject *, PyObject *, int);
extern DL_IMPORT(int) PyFile_SoftSpace(PyObject *, int);
extern DL_IMPORT(int) PyFile_WriteString(const char *, PyObject *);
extern DL_IMPORT(int) PyObject_AsFileDescriptor(PyObject *);

extern DL_IMPORT(const char *) Py_FileSystemDefaultEncoding;
