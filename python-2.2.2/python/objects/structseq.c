//20170425
#include "python.h"
#include "structmember.h"
#include "structseq.h"

static char visible_length_key[] = "n_sequence_fields";
static char real_length_key[] = "n_fields";

#define VISIBLE_SIZE(op) ((op)->ob_size)
#define VISIBLE_SIZE_TP(tp) PyInt_AsLong( \
                      PyDict_GetItemString((tp)->tp_dict, visible_length_key))

#define REAL_SIZE_TP(tp) PyInt_AsLong( \
                      PyDict_GetItemString((tp)->tp_dict, real_length_key))
#define REAL_SIZE(op) REAL_SIZE_TP((op)->ob_type)


PyObject *PyStructSequence_New(PyTypeObject *type)
{
	PyStructSequence *obj;
       
	obj = PyObject_New(PyStructSequence, type);
	obj->ob_size = VISIBLE_SIZE_TP(type);

	return (PyObject*) obj;
}

static void structseq_dealloc(PyStructSequence *obj)
{
	int i, size;

	size = REAL_SIZE(obj);
	for (i = 0; i < size; ++i) 
	{
		Py_XDECREF(obj->ob_item[i]);
	}
	PyObject_FREE(obj);
}

static int structseq_length(PyStructSequence *obj)
{
	return VISIBLE_SIZE(obj);
}

static PyObject *structseq_item(PyStructSequence *obj, int i)
{
	if (i < 0 || i >= VISIBLE_SIZE(obj)) 
	{
		PyErr_SetString(PyExc_IndexError, "tuple index out of range");
		return NULL;
	}
	Py_INCREF(obj->ob_item[i]);
	return obj->ob_item[i];
}

static PyObject *structseq_slice(PyStructSequence *obj, int low, int high)
{
	PyTupleObject *np;
	int i;

	if (low < 0)
	{
		low = 0;
	}
	if (high > VISIBLE_SIZE(obj))
	{
		high = VISIBLE_SIZE(obj);
	}
	if (high < low)
	{
		high = low;
	}
	np = (PyTupleObject *)PyTuple_New(high-low);
	if (np == NULL)
	{
		return NULL;
	}
	for (i = low; i < high; ++i)
	{
		PyObject *v = obj->ob_item[i];
		Py_INCREF(v);
		PyTuple_SET_ITEM(np, i-low, v);
	}
	return (PyObject *) np;
}

static PyObject *structseq_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyObject *arg = NULL;
	PyObject *dict = NULL;
	PyObject *ob;
	PyStructSequence *res = NULL;
	int len, min_len, max_len, i;
	static char *kwlist[] = {"sequence", "dict", 0};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|O:structseq", 
					 kwlist, &arg, &dict))
	{
		return NULL;
	}

	arg = PySequence_Fast(arg, "constructor requires a sequence");

	if (!arg) 
	{				
		return NULL;
	}

	if (dict && !PyDict_Check(dict)) 
	{
		PyErr_Format(PyExc_TypeError, 
			     "%.500s() takes a dict as second arg, if any",
			     type->tp_name);
		Py_DECREF(arg);
		return NULL;
	}

	len = PySequence_Fast_GET_SIZE(arg);
	min_len = VISIBLE_SIZE_TP(type);
	max_len = REAL_SIZE_TP(type);

	if (min_len != max_len) 
	{
		if (len < min_len) 
		{
			PyErr_Format(PyExc_TypeError, 
				"%.500s() takes an at least %d-sequence (%d-sequence given)",
				     type->tp_name, min_len, len);
			Py_DECREF(arg);
			return NULL;
		}

		if (len > max_len) 
		{
			PyErr_Format(PyExc_TypeError, 
				"%.500s() takes an at most %d-sequence (%d-sequence given)",
				     type->tp_name, max_len, len);
			Py_DECREF(arg);
			return NULL;
		}
	} 
	else 
	{
		if (len != min_len) 
		{
			PyErr_Format(PyExc_TypeError, 
				"%.500s() takes a %d-sequence (%d-sequence given)",
				     type->tp_name, min_len, len);
			Py_DECREF(arg);
			return NULL;
		}
	}

	res = (PyStructSequence*) PyStructSequence_New(type);
	for (i = 0; i < len; ++i) 
	{
		PyObject *v = PySequence_Fast_GET_ITEM(arg, i);
		Py_INCREF(v);
		res->ob_item[i] = v;
	}
	for (; i < max_len; ++i) 
	{
		if (dict && (ob = PyDict_GetItemString(
			dict, type->tp_members[i].name))) 
		{

		}
		else 
		{
			ob = Py_None;
		}
		Py_INCREF(ob);
		res->ob_item[i] = ob;
	}
	
	Py_DECREF(arg);
	return (PyObject*) res;
}

static PyObject *make_tuple(PyStructSequence *obj)
{
	return structseq_slice(obj, 0, VISIBLE_SIZE(obj));
}

static PyObject *structseq_repr(PyStructSequence *obj)
{
	PyObject *tup, *str;
	tup = make_tuple(obj);
	str = PyObject_Repr(tup);
	Py_DECREF(tup);
	return str;
}

static PyObject *structseq_concat(PyStructSequence *obj, PyObject *b)
{
	PyObject *tup, *result;
	tup = make_tuple(obj);
	result = PySequence_Concat(tup, b);
	Py_DECREF(tup);
	return result;
}

static PyObject *structseq_repeat(PyStructSequence *obj, int n)
{
	PyObject *tup, *result;
	tup = make_tuple(obj);
	result = PySequence_Repeat(tup, n);
	Py_DECREF(tup);
	return result;
}

static int structseq_contains(PyStructSequence *obj, PyObject *o)
{
	PyObject *tup;
	int result;
	tup = make_tuple(obj);
	result = PySequence_Contains(tup, o);
	Py_DECREF(tup);
	return result;
}

static long structseq_hash(PyObject *obj)
{
	PyObject *tup;
	long result;
	tup = make_tuple((PyStructSequence*) obj);
	result = PyObject_Hash(tup);
	Py_DECREF(tup);
	return result;
}

static PyObject *structseq_richcompare(PyObject *obj, PyObject *o2, int op)
{
	PyObject *tup, *result;
	tup = make_tuple((PyStructSequence*) obj);
	result = PyObject_RichCompare(tup, o2, op);
	Py_DECREF(tup);
	return result;
}

static PyObject *structseq_reduce(PyStructSequence* self)
{
	PyObject* tup;
	PyObject* dict;
	PyObject* result;
	long n_fields, n_visible_fields;
	int i;
	
	n_fields = REAL_SIZE(self);
	n_visible_fields = VISIBLE_SIZE(self);
	tup = PyTuple_New(n_visible_fields);
	if (!tup) 
	{
		return NULL;
	}

	dict = PyDict_New();
	if (!dict) 
	{
		Py_DECREF(tup);
		return NULL;
	}

	for (i = 0; i < n_visible_fields; i++) 
	{
		Py_INCREF(self->ob_item[i]);
		PyTuple_SET_ITEM(tup, i, self->ob_item[i]);
	}
	
	for (; i < n_fields; i++) 
	{
		PyDict_SetItemString(dict, self->ob_type->tp_members[i].name,
				     self->ob_item[i]);
	}

	result = Py_BuildValue("(O(OO))", self->ob_type, tup, dict);

	Py_DECREF(tup);
	Py_DECREF(dict);

	return result;
}

static PySequenceMethods structseq_as_sequence = {
	(inquiry)structseq_length,
	(binaryfunc)structseq_concat,   
	(intargfunc)structseq_repeat,   
	(intargfunc)structseq_item,		
	(intintargfunc)structseq_slice,	
	0,					
	0,					
	(objobjproc)structseq_contains,	 
};

static PyMethodDef structseq_methods[] = {
	{"__reduce__", (PyCFunction)structseq_reduce, 
	 METH_NOARGS, NULL},
	{NULL, NULL}
};

static PyTypeObject _struct_sequence_template = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,					
	NULL,	            
    0,		            
	0,	                
	(destructor)structseq_dealloc,	 
	0,                  
	0,			 		
	0,					
	0,               	
	(reprfunc)structseq_repr,       
	0,					
	&structseq_as_sequence,			
	0,					
	(hashfunc)structseq_hash,   
	0,              			
	0,					
	0,                       	
	0,	                        
	0,					
	Py_TPFLAGS_DEFAULT,         
	NULL,	 		    
	0,					
	0,					
	structseq_richcompare,		
	0,					
	0,					
	0,					
	structseq_methods,      	
    NULL,			         
	0,			        
	0,					
	0,					
	0,					
	0,					
	0,	                
	0,					
	0,					
	structseq_new,		
};

void PyStructSequence_InitType(PyTypeObject *type, PyStructSequence_Desc *desc)
{
	PyObject *dict;
	PyMemberDef* members;
	int n_members, i;

	for (i = 0; desc->fields[i].name != NULL; ++i)
	{
		;
	}
	n_members = i;

	memcpy(type, &_struct_sequence_template, sizeof(PyTypeObject));
	type->tp_name = desc->name;
	type->tp_doc = desc->doc;
	type->tp_basicsize = sizeof(PyStructSequence)+
		sizeof(PyObject*)*(n_members-1);
	type->tp_itemsize = 0;

	members = PyMem_NEW(PyMemberDef, n_members+1);
	
	for (i = 0; i < n_members; ++i) 
	{
		members[i].name = desc->fields[i].name;
		members[i].type = T_OBJECT;
		members[i].offset = offsetof(PyStructSequence, ob_item)
		  + i * sizeof(PyObject*);
		members[i].flags = READONLY;
		members[i].doc = desc->fields[i].doc;
	}
	members[n_members].name = NULL;

	type->tp_members = members;

	if (PyType_Ready(type) < 0)
	{
		return;
	}
	Py_INCREF(type);

	dict = type->tp_dict;
	PyDict_SetItemString(dict, visible_length_key, 
		       PyInt_FromLong((long) desc->n_in_sequence));
	PyDict_SetItemString(dict, real_length_key, 
		       PyInt_FromLong((long) n_members));
	PyDict_SetItemString(dict, "__safe_for_unpickling__", 
		       PyInt_FromLong(1));
}
