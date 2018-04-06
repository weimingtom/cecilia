//20180324
#pragma once

#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else
#include <curses.h>
#ifdef HAVE_TERM_H
#include <term.h>
#endif
#endif

#ifdef HAVE_NCURSES_H
#ifndef WINDOW_HAS_FLAGS
#define WINDOW_HAS_FLAGS 1
#endif
#ifndef MVWDELCH_IS_EXPRESSION
#define MVWDELCH_IS_EXPRESSION 1
#endif
#endif

#define PyCurses_API_pointers 4


typedef struct {
	PyObject_HEAD
	WINDOW *win;
} PyCursesWindowObject;

#define PyCursesWindow_Check(v)	 ((v)->ob_type == &PyCursesWindow_Type)

#ifdef CURSES_MODULE

#else

static void **PyCurses_API;

#define PyCursesWindow_Type (*(PyTypeObject *) PyCurses_API[0])
#define PyCursesSetupTermCalled  {if (! ((int (*)())PyCurses_API[1]) () ) return NULL;}
#define PyCursesInitialised      {if (! ((int (*)())PyCurses_API[2]) () ) return NULL;}
#define PyCursesInitialisedColor {if (! ((int (*)())PyCurses_API[3]) () ) return NULL;}

#define import_curses() \
{ \
  PyObject *module = PyImport_ImportModule("_curses"); \
  if (module != NULL) { \
    PyObject *module_dict = PyModule_GetDict(module); \
    PyObject *c_api_object = PyDict_GetItemString(module_dict, "_C_API"); \
    if (PyCObject_Check(c_api_object)) { \
      PyCurses_API = (void **)PyCObject_AsVoidPtr(c_api_object); \
    } \
  } \
}
#endif

static char *catchall_ERR  = "curses function returned ERR";
static char *catchall_NULL = "curses function returned NULL";

#define ARG_COUNT(X) \
	(((X) == NULL) ? 0 : (PyTuple_Check(X) ? PyTuple_Size(X) : 1))


#define NoArgNoReturnFunction(X) \
static PyObject *PyCurses_ ## X (PyObject *self, PyObject *args) \
{ \
  PyCursesInitialised \
  if (!PyArg_NoArgs(args)) return NULL; \
  return PyCursesCheckERR(X(), # X); }

#define NoArgOrFlagNoReturnFunction(X) \
static PyObject *PyCurses_ ## X (PyObject *self, PyObject *args) \
{ \
  int flag = 0; \
  PyCursesInitialised \
  switch(ARG_COUNT(args)) { \
  case 0: \
    return PyCursesCheckERR(X(), # X); \
  case 1: \
    if (!PyArg_Parse(args, "i;True(1) or False(0)", &flag)) return NULL; \
    if (flag) return PyCursesCheckERR(X(), # X); \
    else return PyCursesCheckERR(no ## X (), # X); \
  default: \
    PyErr_SetString(PyExc_TypeError, # X " requires 0 or 1 arguments"); \
    return NULL; } }

#define NoArgReturnIntFunction(X) \
static PyObject *PyCurses_ ## X (PyObject *self, PyObject *args) \
{ \
 PyCursesInitialised \
 if (!PyArg_NoArgs(args)) return NULL; \
 return PyInt_FromLong((long) X()); }


#define NoArgReturnStringFunction(X) \
static PyObject *PyCurses_ ## X (PyObject *self, PyObject *args) \
{ \
  PyCursesInitialised \
  if (!PyArg_NoArgs(args)) return NULL; \
  return PyString_FromString(X()); }

#define NoArgTrueFalseFunction(X) \
static PyObject *PyCurses_ ## X (PyObject *self, PyObject *args) \
{ \
  PyCursesInitialised \
  if (!PyArg_NoArgs(args)) return NULL; \
  if (X () == FALSE) { \
    Py_INCREF(Py_False); \
    return Py_False; \
  } \
  Py_INCREF(Py_True); \
  return Py_True; }

#define NoArgNoReturnVoidFunction(X) \
static PyObject *PyCurses_ ## X (PyObject *self, PyObject *args) \
{ \
  PyCursesInitialised \
  if (!PyArg_NoArgs(args)) return NULL; \
  X(); \
  Py_INCREF(Py_None); \
  return Py_None; }
