#pragma once
   
typedef struct PyStructSequence_Field {
	char *name;
	char *doc;
} PyStructSequence_Field;

typedef struct PyStructSequence_Desc {
	char *name;
	char *doc;
	struct PyStructSequence_Field *fields;
	int n_in_sequence;
} PyStructSequence_Desc;

extern DL_IMPORT(void) PyStructSequence_InitType(PyTypeObject *type, 
						 PyStructSequence_Desc *desc);
       
extern DL_IMPORT(PyObject *) PyStructSequence_New(PyTypeObject* type);

typedef struct {
	PyObject_VAR_HEAD
	PyObject *ob_item[1];
} PyStructSequence;

/* Macro, *only* to be used to fill in brand new objects */
#define PyStructSequence_SET_ITEM(op, i, v) \
	(((PyStructSequence *)(op))->ob_item[i] = v)
