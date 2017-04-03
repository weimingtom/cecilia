#pragma once

static struct PycStringIO_CAPI {
  
  /* Read a string.  If the last argument is -1, the remainder will be read. */
  int(*cread)(PyObject *, char **, int);

  /* Read a line */
  int(*creadline)(PyObject *, char **);

  /* Write a string */
  int(*cwrite)(PyObject *, char *, int);

  /* Get the cStringIO object as a Python string */
  PyObject *(*cgetvalue)(PyObject *);

  /* Create a new output object */
  PyObject *(*NewOutput)(int);

  /* Create an input object from a Python string */
  PyObject *(*NewInput)(PyObject *);

  /* The Python types for cStringIO input and output objects.
     Note that you can do input on an output object.
     */
  PyTypeObject *InputType, *OutputType;

} * PycStringIO = NULL;

/* These can be used to test if you have one */
#define PycStringIO_InputCheck(O) \
  ((O)->ob_type==PycStringIO->InputType)
#define PycStringIO_OutputCheck(O) \
  ((O)->ob_type==PycStringIO->OutputType)

static void *
xxxPyCObject_Import(char *module_name, char *name)
{
  PyObject *m, *c;
  void *r=NULL;
  
  if((m=PyImport_ImportModule(module_name)))
    {
      if((c=PyObject_GetAttrString(m,name)))
	{
	  r=PyCObject_AsVoidPtr(c);
	  Py_DECREF(c);
	}
      Py_DECREF(m);
    }

  return r;
}

#define PycString_IMPORT \
  PycStringIO=(struct PycStringIO_CAPI*)xxxPyCObject_Import("cStringIO", "cStringIO_CAPI")
