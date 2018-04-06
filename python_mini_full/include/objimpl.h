//20180324
#pragma once

#include "pymem.h"

#ifdef WITH_PYMALLOC
#define PyCore_OBJECT_MALLOC_FUNC    _PyCore_ObjectMalloc
#define PyCore_OBJECT_REALLOC_FUNC   _PyCore_ObjectRealloc
#define PyCore_OBJECT_FREE_FUNC      _PyCore_ObjectFree
#define NEED_TO_DECLARE_OBJECT_MALLOC_AND_FRIEND
#endif 

#ifndef PyCore_OBJECT_MALLOC_FUNC
#undef PyCore_OBJECT_REALLOC_FUNC
#undef PyCore_OBJECT_FREE_FUNC
#define PyCore_OBJECT_MALLOC_FUNC    PyCore_MALLOC_FUNC
#define PyCore_OBJECT_REALLOC_FUNC   PyCore_REALLOC_FUNC
#define PyCore_OBJECT_FREE_FUNC      PyCore_FREE_FUNC
#endif

#ifndef PyCore_OBJECT_MALLOC_PROTO
#undef PyCore_OBJECT_REALLOC_PROTO
#undef PyCore_OBJECT_FREE_PROTO
#define PyCore_OBJECT_MALLOC_PROTO   PyCore_MALLOC_PROTO
#define PyCore_OBJECT_REALLOC_PROTO  PyCore_REALLOC_PROTO
#define PyCore_OBJECT_FREE_PROTO     PyCore_FREE_PROTO
#endif

#ifdef NEED_TO_DECLARE_OBJECT_MALLOC_AND_FRIEND
extern void *PyCore_OBJECT_MALLOC_FUNC PyCore_OBJECT_MALLOC_PROTO;
extern void *PyCore_OBJECT_REALLOC_FUNC PyCore_OBJECT_REALLOC_PROTO;
extern void PyCore_OBJECT_FREE_FUNC PyCore_OBJECT_FREE_PROTO;
#endif

#ifndef PyCore_OBJECT_MALLOC
#undef PyCore_OBJECT_REALLOC
#undef PyCore_OBJECT_FREE
#define PyCore_OBJECT_MALLOC(n)      PyCore_OBJECT_MALLOC_FUNC(n)
#define PyCore_OBJECT_REALLOC(p, n)  PyCore_OBJECT_REALLOC_FUNC((p), (n))
#define PyCore_OBJECT_FREE(p)        PyCore_OBJECT_FREE_FUNC(p)
#endif

extern DL_IMPORT(void *) PyObject_Malloc(size_t);
extern DL_IMPORT(void *) PyObject_Realloc(void *, size_t);
extern DL_IMPORT(void) PyObject_Free(void *);

#define PyObject_MALLOC(n)           PyCore_OBJECT_MALLOC(n)
#define PyObject_REALLOC(op, n)      PyCore_OBJECT_REALLOC((void *)(op), (n))
#define PyObject_FREE(op)            PyCore_OBJECT_FREE((void *)(op))

extern DL_IMPORT(PyObject *) PyObject_Init(PyObject *, PyTypeObject *);
extern DL_IMPORT(PyVarObject *) PyObject_InitVar(PyVarObject *,
                                                 PyTypeObject *, int);
extern DL_IMPORT(PyObject *) _PyObject_New(PyTypeObject *);
extern DL_IMPORT(PyVarObject *) _PyObject_NewVar(PyTypeObject *, int);
extern DL_IMPORT(void) _PyObject_Del(PyObject *);

#define PyObject_New(type, typeobj) \
		( (type *) _PyObject_New(typeobj) )
#define PyObject_NewVar(type, typeobj, n) \
		( (type *) _PyObject_NewVar((typeobj), (n)) )
#define PyObject_Del(op) _PyObject_Del((PyObject *)(op))

#define PyObject_INIT(op, typeobj) \
	( (op)->ob_type = (typeobj), _Py_NewReference((PyObject *)(op)), (op) )
#define PyObject_INIT_VAR(op, typeobj, size) \
	( (op)->ob_size = (size), PyObject_INIT((op), (typeobj)) )

#define _PyObject_SIZE(typeobj) ( (typeobj)->tp_basicsize )

#if ((SIZEOF_VOID_P - 1) & SIZEOF_VOID_P) != 0
#   error "_PyObject_VAR_SIZE requires SIZEOF_VOID_P be a power of 2"
#endif

#define _PyObject_VAR_SIZE(typeobj, nitems)	\
	(size_t)				\
	( ( (typeobj)->tp_basicsize +		\
	    (nitems)*(typeobj)->tp_itemsize +	\
	    (SIZEOF_VOID_P - 1)			\
	  ) & ~(SIZEOF_VOID_P - 1)		\
	)

#define PyObject_NEW(type, typeobj) \
( (type *) PyObject_Init( \
	(PyObject *) PyObject_MALLOC( _PyObject_SIZE(typeobj) ), (typeobj)) )

#define PyObject_NEW_VAR(type, typeobj, n) \
( (type *) PyObject_InitVar( \
      (PyVarObject *) PyObject_MALLOC(_PyObject_VAR_SIZE((typeobj),(n)) ),\
      (typeobj), (n)) )

#define PyObject_DEL(op) PyObject_FREE(op)

#define PyType_IS_GC(t) PyType_HasFeature((t), Py_TPFLAGS_HAVE_GC)

#define PyObject_IS_GC(o) (PyType_IS_GC((o)->ob_type) && \
	((o)->ob_type->tp_is_gc == NULL || (o)->ob_type->tp_is_gc(o)))

extern DL_IMPORT(PyObject *) _PyObject_GC_Malloc(PyTypeObject *, int);
extern DL_IMPORT(PyVarObject *) _PyObject_GC_Resize(PyVarObject *, int);

#define PyObject_GC_Resize(type, op, n) \
		( (type *) _PyObject_GC_Resize((PyVarObject *)(op), (n)) )

extern DL_IMPORT(PyObject *) _PyObject_GC_New(PyTypeObject *);
extern DL_IMPORT(PyVarObject *) _PyObject_GC_NewVar(PyTypeObject *, int);
extern DL_IMPORT(void) _PyObject_GC_Del(PyObject *);
extern DL_IMPORT(void) _PyObject_GC_Track(PyObject *);
extern DL_IMPORT(void) _PyObject_GC_UnTrack(PyObject *);

#ifdef WITH_CYCLE_GC

typedef union _gc_head {
	struct {
		union _gc_head *gc_next;
		union _gc_head *gc_prev;
		int gc_refs;
	} gc;
	double dummy;
} PyGC_Head;

extern PyGC_Head _PyGC_generation0;

#define _PyObject_GC_TRACK(o) do { \
	PyGC_Head *g = (PyGC_Head *)(o)-1; \
	if (g->gc.gc_next != NULL) \
		Py_FatalError("GC object already in linked list"); \
	g->gc.gc_next = &_PyGC_generation0; \
	g->gc.gc_prev = _PyGC_generation0.gc.gc_prev; \
	g->gc.gc_prev->gc.gc_next = g; \
	_PyGC_generation0.gc.gc_prev = g; \
    } while (0);

#define _PyObject_GC_UNTRACK(o) do { \
	PyGC_Head *g = (PyGC_Head *)(o)-1; \
	g->gc.gc_prev->gc.gc_next = g->gc.gc_next; \
	g->gc.gc_next->gc.gc_prev = g->gc.gc_prev; \
	g->gc.gc_next = NULL; \
    } while (0);

#define PyObject_GC_Track(op) _PyObject_GC_Track((PyObject *)op)
#define PyObject_GC_UnTrack(op) _PyObject_GC_UnTrack((PyObject *)op)


#define PyObject_GC_New(type, typeobj) \
		( (type *) _PyObject_GC_New(typeobj) )
#define PyObject_GC_NewVar(type, typeobj, n) \
		( (type *) _PyObject_GC_NewVar((typeobj), (n)) )
#define PyObject_GC_Del(op) _PyObject_GC_Del((PyObject *)(op))

#else

#define PyObject_GC_New PyObject_New
#define PyObject_GC_NewVar PyObject_NewVar
#define PyObject_GC_Del	 PyObject_Del
#define _PyObject_GC_TRACK(op)
#define _PyObject_GC_UNTRACK(op)
#define PyObject_GC_Track(op)
#define PyObject_GC_UnTrack(op)

#endif

#define PyGC_HEAD_SIZE 0
#define PyObject_GC_Init(op)
#define PyObject_GC_Fini(op)
#define PyObject_AS_GC(op) (op)
#define PyObject_FROM_GC(op) (op)


#define PyType_SUPPORTS_WEAKREFS(t) \
        (PyType_HasFeature((t), Py_TPFLAGS_HAVE_WEAKREFS) \
         && ((t)->tp_weaklistoffset > 0))

#define PyObject_GET_WEAKREFS_LISTPTR(o) \
	((PyObject **) (((char *) (o)) + (o)->ob_type->tp_weaklistoffset))
