//20180324
#pragma once

#ifdef Py_DEBUG

#define Py_TRACE_REFS

#define Py_REF_DEBUG

#endif

#ifdef Py_TRACE_REFS
#define PyObject_HEAD \
	struct _object *_ob_next, *_ob_prev; \
	int ob_refcnt; \
	struct _typeobject *ob_type;
#define PyObject_HEAD_INIT(type) 0, 0, 1, type,
#else
#define PyObject_HEAD \
	int ob_refcnt; \
	struct _typeobject *ob_type;
#define PyObject_HEAD_INIT(type) 1, type,
#endif

#define PyObject_VAR_HEAD \
	PyObject_HEAD \
	int ob_size;
 
typedef struct _object {
	PyObject_HEAD
} PyObject;

typedef struct {
	PyObject_VAR_HEAD
} PyVarObject;

typedef PyObject * (*unaryfunc)(PyObject *);
typedef PyObject * (*binaryfunc)(PyObject *, PyObject *);
typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
typedef int (*inquiry)(PyObject *);
typedef int (*coercion)(PyObject **, PyObject **);
typedef PyObject *(*intargfunc)(PyObject *, int);
typedef PyObject *(*intintargfunc)(PyObject *, int, int);
typedef int(*intobjargproc)(PyObject *, int, PyObject *);
typedef int(*intintobjargproc)(PyObject *, int, int, PyObject *);
typedef int(*objobjargproc)(PyObject *, PyObject *, PyObject *);
typedef int (*getreadbufferproc)(PyObject *, int, void **);
typedef int (*getwritebufferproc)(PyObject *, int, void **);
typedef int (*getsegcountproc)(PyObject *, int *);
typedef int (*getcharbufferproc)(PyObject *, int, const char **);
typedef int (*objobjproc)(PyObject *, PyObject *);
typedef int (*visitproc)(PyObject *, void *);
typedef int (*traverseproc)(PyObject *, visitproc, void *);

typedef struct {
	binaryfunc nb_add;
	binaryfunc nb_subtract;
	binaryfunc nb_multiply;
	binaryfunc nb_divide;
	binaryfunc nb_remainder;
	binaryfunc nb_divmod;
	ternaryfunc nb_power;
	unaryfunc nb_negative;
	unaryfunc nb_positive;
	unaryfunc nb_absolute;
	inquiry nb_nonzero;
	unaryfunc nb_invert;
	binaryfunc nb_lshift;
	binaryfunc nb_rshift;
	binaryfunc nb_and;
	binaryfunc nb_xor;
	binaryfunc nb_or;
	coercion nb_coerce;
	unaryfunc nb_int;
	unaryfunc nb_long;
	unaryfunc nb_float;
	unaryfunc nb_oct;
	unaryfunc nb_hex;

	binaryfunc nb_inplace_add;
	binaryfunc nb_inplace_subtract;
	binaryfunc nb_inplace_multiply;
	binaryfunc nb_inplace_divide;
	binaryfunc nb_inplace_remainder;
	ternaryfunc nb_inplace_power;
	binaryfunc nb_inplace_lshift;
	binaryfunc nb_inplace_rshift;
	binaryfunc nb_inplace_and;
	binaryfunc nb_inplace_xor;
	binaryfunc nb_inplace_or;

	binaryfunc nb_floor_divide;
	binaryfunc nb_true_divide;
	binaryfunc nb_inplace_floor_divide;
	binaryfunc nb_inplace_true_divide;
} PyNumberMethods;

typedef struct {
	inquiry sq_length;
	binaryfunc sq_concat;
	intargfunc sq_repeat;
	intargfunc sq_item;
	intintargfunc sq_slice;
	intobjargproc sq_ass_item;
	intintobjargproc sq_ass_slice;
	objobjproc sq_contains;
	binaryfunc sq_inplace_concat;
	intargfunc sq_inplace_repeat;
} PySequenceMethods;

typedef struct {
	inquiry mp_length;
	binaryfunc mp_subscript;
	objobjargproc mp_ass_subscript;
} PyMappingMethods;

typedef struct {
	getreadbufferproc bf_getreadbuffer;
	getwritebufferproc bf_getwritebuffer;
	getsegcountproc bf_getsegcount;
	getcharbufferproc bf_getcharbuffer;
} PyBufferProcs;
	

typedef void (*destructor)(PyObject *);
typedef int (*printfunc)(PyObject *, FILE *, int);
typedef PyObject *(*getattrfunc)(PyObject *, char *);
typedef PyObject *(*getattrofunc)(PyObject *, PyObject *);
typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
typedef int (*cmpfunc)(PyObject *, PyObject *);
typedef PyObject *(*reprfunc)(PyObject *);
typedef long (*hashfunc)(PyObject *);
typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
typedef PyObject *(*getiterfunc) (PyObject *);
typedef PyObject *(*iternextfunc) (PyObject *);
typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*newfunc)(struct _typeobject *, PyObject *, PyObject *);
typedef PyObject *(*allocfunc)(struct _typeobject *, int);

typedef struct _typeobject {
	PyObject_VAR_HEAD
	char *tp_name; 
	int tp_basicsize, tp_itemsize; 
	
	destructor tp_dealloc;
	printfunc tp_print;
	getattrfunc tp_getattr;
	setattrfunc tp_setattr;
	cmpfunc tp_compare;
	reprfunc tp_repr;
	
	PyNumberMethods *tp_as_number;
	PySequenceMethods *tp_as_sequence;
	PyMappingMethods *tp_as_mapping;

	hashfunc tp_hash;
	ternaryfunc tp_call;
	reprfunc tp_str;
	getattrofunc tp_getattro;
	setattrofunc tp_setattro;

	PyBufferProcs *tp_as_buffer;
	
	long tp_flags;

	char *tp_doc;

	traverseproc tp_traverse;
	
	inquiry tp_clear;

	richcmpfunc tp_richcompare;

	long tp_weaklistoffset;

	getiterfunc tp_iter;
	iternextfunc tp_iternext;

	struct PyMethodDef *tp_methods;
	struct PyMemberDef *tp_members;
	struct PyGetSetDef *tp_getset;
	struct _typeobject *tp_base;
	PyObject *tp_dict;
	descrgetfunc tp_descr_get;
	descrsetfunc tp_descr_set;
	long tp_dictoffset;
	initproc tp_init;
	allocfunc tp_alloc;
	newfunc tp_new;
	destructor tp_free; 
	inquiry tp_is_gc; 
	PyObject *tp_bases;
	PyObject *tp_mro; 
	PyObject *tp_cache;
	PyObject *tp_subclasses;
	PyObject *tp_weaklist;

#ifdef COUNT_ALLOCS
	int tp_allocs;
	int tp_frees;
	int tp_maxalloc;
	struct _typeobject *tp_next;
#endif
} PyTypeObject;


extern DL_IMPORT(int) PyType_IsSubtype(PyTypeObject *, PyTypeObject *);
#define PyObject_TypeCheck(ob, tp) \
	((ob)->ob_type == (tp) || PyType_IsSubtype((ob)->ob_type, (tp)))

extern DL_IMPORT(PyTypeObject) PyType_Type; 
extern DL_IMPORT(PyTypeObject) PyBaseObject_Type; 
extern DL_IMPORT(PyTypeObject) PySuper_Type; 

#define PyType_Check(op) PyObject_TypeCheck(op, &PyType_Type)
#define PyType_CheckExact(op) ((op)->ob_type == &PyType_Type)

extern DL_IMPORT(int) PyType_Ready(PyTypeObject *);
extern DL_IMPORT(PyObject *) PyType_GenericAlloc(PyTypeObject *, int);
extern DL_IMPORT(PyObject *) PyType_GenericNew(PyTypeObject *,
					       PyObject *, PyObject *);
extern DL_IMPORT(PyObject *) _PyType_Lookup(PyTypeObject *, PyObject *);

extern DL_IMPORT(int) PyObject_Print(PyObject *, FILE *, int);
extern DL_IMPORT(void) _PyObject_Dump(PyObject *);
extern DL_IMPORT(PyObject *) PyObject_Repr(PyObject *);
extern DL_IMPORT(PyObject *) PyObject_Str(PyObject *);
#ifdef Py_USING_UNICODE
extern DL_IMPORT(PyObject *) PyObject_Unicode(PyObject *);
#endif
extern DL_IMPORT(int) PyObject_Compare(PyObject *, PyObject *);
extern DL_IMPORT(PyObject *) PyObject_RichCompare(PyObject *, PyObject *, int);
extern DL_IMPORT(int) PyObject_RichCompareBool(PyObject *, PyObject *, int);
extern DL_IMPORT(PyObject *) PyObject_GetAttrString(PyObject *, char *);
extern DL_IMPORT(int) PyObject_SetAttrString(PyObject *, char *, PyObject *);
extern DL_IMPORT(int) PyObject_HasAttrString(PyObject *, char *);
extern DL_IMPORT(PyObject *) PyObject_GetAttr(PyObject *, PyObject *);
extern DL_IMPORT(int) PyObject_SetAttr(PyObject *, PyObject *, PyObject *);
extern DL_IMPORT(int) PyObject_HasAttr(PyObject *, PyObject *);
extern DL_IMPORT(PyObject **) _PyObject_GetDictPtr(PyObject *);
extern DL_IMPORT(PyObject *) PyObject_GenericGetAttr(PyObject *, PyObject *);
extern DL_IMPORT(int) PyObject_GenericSetAttr(PyObject *,
					      PyObject *, PyObject *);
extern DL_IMPORT(long) PyObject_Hash(PyObject *);
extern DL_IMPORT(int) PyObject_IsTrue(PyObject *);
extern DL_IMPORT(int) PyObject_Not(PyObject *);
extern DL_IMPORT(int) PyCallable_Check(PyObject *);
extern DL_IMPORT(int) PyNumber_Coerce(PyObject **, PyObject **);
extern DL_IMPORT(int) PyNumber_CoerceEx(PyObject **, PyObject **);

extern DL_IMPORT(void) PyObject_ClearWeakRefs(PyObject *);

extern int _PyObject_SlotCompare(PyObject *, PyObject *);


extern DL_IMPORT(PyObject *) PyObject_Dir(PyObject *);


extern DL_IMPORT(int) Py_ReprEnter(PyObject *);
extern DL_IMPORT(void) Py_ReprLeave(PyObject *);

extern DL_IMPORT(long) _Py_HashDouble(double);
extern DL_IMPORT(long) _Py_HashPointer(void*);

#define PyObject_REPR(obj) PyString_AS_STRING(PyObject_Repr(obj))

#define Py_PRINT_RAW	1	

#define Py_TPFLAGS_HAVE_GETCHARBUFFER  (1L<<0)

#define Py_TPFLAGS_HAVE_SEQUENCE_IN (1L<<1)

#define Py_TPFLAGS_GC 0 

#define Py_TPFLAGS_HAVE_INPLACEOPS (1L<<3)

#define Py_TPFLAGS_CHECKTYPES (1L<<4)

#define Py_TPFLAGS_HAVE_RICHCOMPARE (1L<<5)

#define Py_TPFLAGS_HAVE_WEAKREFS (1L<<6)

#define Py_TPFLAGS_HAVE_ITER (1L<<7)

#define Py_TPFLAGS_HAVE_CLASS (1L<<8)

#define Py_TPFLAGS_HEAPTYPE (1L<<9)

#define Py_TPFLAGS_BASETYPE (1L<<10)

#define Py_TPFLAGS_READY (1L<<12)

#define Py_TPFLAGS_READYING (1L<<13)

#ifdef WITH_CYCLE_GC
#define Py_TPFLAGS_HAVE_GC (1L<<14)
#else
#define Py_TPFLAGS_HAVE_GC 0
#endif

#define Py_TPFLAGS_DEFAULT  ( \
                             Py_TPFLAGS_HAVE_GETCHARBUFFER | \
                             Py_TPFLAGS_HAVE_SEQUENCE_IN | \
                             Py_TPFLAGS_HAVE_INPLACEOPS | \
                             Py_TPFLAGS_HAVE_RICHCOMPARE | \
                             Py_TPFLAGS_HAVE_WEAKREFS | \
                             Py_TPFLAGS_HAVE_ITER | \
                             Py_TPFLAGS_HAVE_CLASS | \
                            0)

#define PyType_HasFeature(t,f)  (((t)->tp_flags & (f)) != 0)


#ifdef Py_TRACE_REFS
#ifndef Py_REF_DEBUG
#define Py_REF_DEBUG
#endif
#endif

#ifdef Py_TRACE_REFS
extern DL_IMPORT(void) _Py_Dealloc(PyObject *);
extern DL_IMPORT(void) _Py_NewReference(PyObject *);
extern DL_IMPORT(void) _Py_ForgetReference(PyObject *);
extern DL_IMPORT(void) _Py_PrintReferences(FILE *);
extern DL_IMPORT(void) _Py_ResetReferences();
#endif

#ifndef Py_TRACE_REFS
#ifdef COUNT_ALLOCS
#define _Py_Dealloc(op) ((op)->ob_type->tp_frees++, (*(op)->ob_type->tp_dealloc)((PyObject *)(op)))
#define _Py_ForgetReference(op) ((op)->ob_type->tp_frees++)
#else
#define _Py_Dealloc(op) (*(op)->ob_type->tp_dealloc)((PyObject *)(op))
#define _Py_ForgetReference(op)
#endif
#endif

#ifdef COUNT_ALLOCS
extern DL_IMPORT(void) inc_count(PyTypeObject *);
#endif

#ifdef Py_REF_DEBUG

extern DL_IMPORT(long) _Py_RefTotal;

#ifndef Py_TRACE_REFS
#ifdef COUNT_ALLOCS
#define _Py_NewReference(op) (inc_count((op)->ob_type), _Py_RefTotal++, (op)->ob_refcnt = 1)
#else
#define _Py_NewReference(op) (_Py_RefTotal++, (op)->ob_refcnt = 1)
#endif
#endif

#define Py_INCREF(op) (_Py_RefTotal++, (op)->ob_refcnt++)

#define Py_DECREF(op)							\
       if (--_Py_RefTotal, 0 < (--((op)->ob_refcnt))) ;			\
       else if (0 == (op)->ob_refcnt) _Py_Dealloc( (PyObject*)(op));	\
       else fprintf( stderr, "%s:%i negative ref count %i\n",	\
		           __FILE__, __LINE__, (op)->ob_refcnt)
#else

#ifdef COUNT_ALLOCS
#define _Py_NewReference(op) (inc_count((op)->ob_type), (op)->ob_refcnt = 1)
#else
#define _Py_NewReference(op) ((op)->ob_refcnt = 1)
#endif

#define Py_INCREF(op) ((op)->ob_refcnt++)
#define Py_DECREF(op) \
	if (--(op)->ob_refcnt != 0) \
		; \
	else \
		_Py_Dealloc((PyObject *)(op))
#endif

#define Py_XINCREF(op) if ((op) == NULL) ; else Py_INCREF(op)
#define Py_XDECREF(op) if ((op) == NULL) ; else Py_DECREF(op)

extern DL_IMPORT(PyObject) _Py_NoneStruct;

#define Py_None (&_Py_NoneStruct)

extern DL_IMPORT(PyObject) _Py_NotImplementedStruct; 

#define Py_NotImplemented (&_Py_NotImplementedStruct)

#define Py_LT 0
#define Py_LE 1
#define Py_EQ 2
#define Py_NE 3
#define Py_GT 4
#define Py_GE 5

#ifdef BAD_STATIC_FORWARD
#define staticforward extern
#define statichere static
#else
#define staticforward static
#define statichere static
#endif

#define PyTrash_UNWIND_LEVEL 50

#define Py_TRASHCAN_SAFE_BEGIN(op) \
	{ \
		++_PyTrash_delete_nesting; \
		if (_PyTrash_delete_nesting < PyTrash_UNWIND_LEVEL) { \

#define Py_TRASHCAN_SAFE_END(op) \
		;} \
		else \
			_PyTrash_deposit_object((PyObject*)op);\
		--_PyTrash_delete_nesting; \
		if (_PyTrash_delete_later && _PyTrash_delete_nesting <= 0) \
			_PyTrash_destroy_chain(); \
	} \

extern DL_IMPORT(void) _PyTrash_deposit_object(PyObject*);
extern DL_IMPORT(void) _PyTrash_destroy_chain();

extern DL_IMPORT(int) _PyTrash_delete_nesting;
extern DL_IMPORT(PyObject *) _PyTrash_delete_later;

#define xxPy_TRASHCAN_SAFE_BEGIN(op) 
#define xxPy_TRASHCAN_SAFE_END(op) ;
