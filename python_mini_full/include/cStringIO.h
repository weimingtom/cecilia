//20180318
#pragma once

static struct PycStringIO_CAPI {
  int(*cread)(PyObject *, char **, int);
  int(*creadline)(PyObject *, char **);
  int(*cwrite)(PyObject *, char *, int);
  PyObject *(*cgetvalue)(PyObject *);
  PyObject *(*NewOutput)(int);
  PyObject *(*NewInput)(PyObject *);
  PyTypeObject *InputType, *OutputType;
} * PycStringIO = NULL;

#define PycStringIO_InputCheck(O) \
  ((O)->ob_type==PycStringIO->InputType)
#define PycStringIO_OutputCheck(O) \
  ((O)->ob_type==PycStringIO->OutputType)

static void *xxxPyCObject_Import(char *module_name, char *name)
{
	PyObject *m, *c;
	void *r = NULL;
  
	if((m = PyImport_ImportModule(module_name)))
    {
		if((c = PyObject_GetAttrString(m,name)))
		{
			r = PyCObject_AsVoidPtr(c);
			Py_DECREF(c);
		}
		Py_DECREF(m);
    }

	return r;
}

#define PycString_IMPORT \
  PycStringIO = (struct PycStringIO_CAPI*)xxxPyCObject_Import("cStringIO", "cStringIO_CAPI")
