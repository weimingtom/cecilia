//20180318
#pragma once

#define  PyObject_DelAttrString(O,A) PyObject_SetAttrString((O),(A),NULL)
#define  PyObject_DelAttr(O,A) PyObject_SetAttr((O),(A),NULL)

     DL_IMPORT(int) PyObject_Cmp(PyObject *o1, PyObject *o2, int *result);
     
	 DL_IMPORT(int) PyCallable_Check(PyObject *o);
     
	 DL_IMPORT(PyObject *) PyObject_Call(PyObject *callable_object,
					 PyObject *args, PyObject *kw);
     
	 DL_IMPORT(PyObject *) PyObject_CallObject(PyObject *callable_object,
                                               PyObject *args);

     DL_IMPORT(PyObject *) PyObject_CallFunction(PyObject *callable_object,
                                                 char *format, ...);

     DL_IMPORT(PyObject *) PyObject_CallMethod(PyObject *o, char *m,
                                               char *format, ...);

     DL_IMPORT(PyObject *) PyObject_CallFunctionObjArgs(PyObject *callable,
                                                        ...);

     DL_IMPORT(PyObject *) PyObject_CallMethodObjArgs(PyObject *o,
                                                      PyObject *m, ...);

     DL_IMPORT(PyObject *) PyObject_Type(PyObject *o);

     DL_IMPORT(int) PyObject_Size(PyObject *o);

#undef PyObject_Length
     DL_IMPORT(int) PyObject_Length(PyObject *o);
#define PyObject_Length PyObject_Size


     DL_IMPORT(PyObject *) PyObject_GetItem(PyObject *o, PyObject *key);

     DL_IMPORT(int) PyObject_SetItem(PyObject *o, PyObject *key, PyObject *v);

     DL_IMPORT(int) PyObject_DelItemString(PyObject *o, char *key);

     DL_IMPORT(int) PyObject_DelItem(PyObject *o, PyObject *key);

     DL_IMPORT(int) PyObject_AsCharBuffer(PyObject *obj,
					  const char **buffer,
					  int *buffer_len);

     DL_IMPORT(int) PyObject_CheckReadBuffer(PyObject *obj);

     DL_IMPORT(int) PyObject_AsReadBuffer(PyObject *obj,
					  const void **buffer,
					  int *buffer_len);

     DL_IMPORT(int) PyObject_AsWriteBuffer(PyObject *obj,
					   void **buffer,
					   int *buffer_len);

     DL_IMPORT(PyObject *) PyObject_GetIter(PyObject *);

#define PyIter_Check(obj) \
    (PyType_HasFeature((obj)->ob_type, Py_TPFLAGS_HAVE_ITER) && \
     (obj)->ob_type->tp_iternext != NULL)

     DL_IMPORT(PyObject *) PyIter_Next(PyObject *);

     DL_IMPORT(int) PyNumber_Check(PyObject *o);

     DL_IMPORT(PyObject *) PyNumber_Add(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Subtract(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Multiply(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Divide(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_FloorDivide(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_TrueDivide(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Remainder(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Divmod(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Power(PyObject *o1, PyObject *o2,
                                          PyObject *o3);

     DL_IMPORT(PyObject *) PyNumber_Negative(PyObject *o);

     DL_IMPORT(PyObject *) PyNumber_Positive(PyObject *o);

     DL_IMPORT(PyObject *) PyNumber_Absolute(PyObject *o);

     DL_IMPORT(PyObject *) PyNumber_Invert(PyObject *o);

     DL_IMPORT(PyObject *) PyNumber_Lshift(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Rshift(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_And(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Xor(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Or(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_Int(PyObject *o);

     DL_IMPORT(PyObject *) PyNumber_Long(PyObject *o);

     DL_IMPORT(PyObject *) PyNumber_Float(PyObject *o);
	 
     DL_IMPORT(PyObject *) PyNumber_InPlaceAdd(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceSubtract(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceMultiply(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceDivide(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceFloorDivide(PyObject *o1,
						       PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceTrueDivide(PyObject *o1,
						      PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceRemainder(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlacePower(PyObject *o1, PyObject *o2,
     						 PyObject *o3);

     DL_IMPORT(PyObject *) PyNumber_InPlaceLshift(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceRshift(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceAnd(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceXor(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PyNumber_InPlaceOr(PyObject *o1, PyObject *o2);

     DL_IMPORT(int) PySequence_Check(PyObject *o);

     DL_IMPORT(int) PySequence_Size(PyObject *o);

#undef PySequence_Length
     DL_IMPORT(int) PySequence_Length(PyObject *o);
#define PySequence_Length PySequence_Size

     DL_IMPORT(PyObject *) PySequence_Concat(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PySequence_Repeat(PyObject *o, int count);

     DL_IMPORT(PyObject *) PySequence_GetItem(PyObject *o, int i);

     DL_IMPORT(PyObject *) PySequence_GetSlice(PyObject *o, int i1, int i2);

     DL_IMPORT(int) PySequence_SetItem(PyObject *o, int i, PyObject *v);

     DL_IMPORT(int) PySequence_DelItem(PyObject *o, int i);

     DL_IMPORT(int) PySequence_SetSlice(PyObject *o, int i1, int i2,
                                        PyObject *v);

     DL_IMPORT(int) PySequence_DelSlice(PyObject *o, int i1, int i2);

     DL_IMPORT(PyObject *) PySequence_Tuple(PyObject *o);

     DL_IMPORT(PyObject *) PySequence_List(PyObject *o);

     DL_IMPORT(PyObject *) PySequence_Fast(PyObject *o, const char* m);

#define PySequence_Fast_GET_SIZE(o) \
	(PyList_Check(o) ? PyList_GET_SIZE(o) : PyTuple_GET_SIZE(o))

#define PySequence_Fast_GET_ITEM(o, i)\
     (PyList_Check(o) ? PyList_GET_ITEM(o, i) : PyTuple_GET_ITEM(o, i))

     DL_IMPORT(int) PySequence_Count(PyObject *o, PyObject *value);

     DL_IMPORT(int) PySequence_Contains(PyObject *seq, PyObject *ob);

#define PY_ITERSEARCH_COUNT    1
#define PY_ITERSEARCH_INDEX    2
#define PY_ITERSEARCH_CONTAINS 3
     DL_IMPORT(int) _PySequence_IterSearch(PyObject *seq, PyObject *obj,
     		    int operation);

#undef PySequence_In
     DL_IMPORT(int) PySequence_In(PyObject *o, PyObject *value);

#define PySequence_In PySequence_Contains

     DL_IMPORT(int) PySequence_Index(PyObject *o, PyObject *value);

     DL_IMPORT(PyObject *) PySequence_InPlaceConcat(PyObject *o1, PyObject *o2);

     DL_IMPORT(PyObject *) PySequence_InPlaceRepeat(PyObject *o, int count);

     DL_IMPORT(int) PyMapping_Check(PyObject *o);

     DL_IMPORT(int) PyMapping_Size(PyObject *o);

#undef PyMapping_Length
     DL_IMPORT(int) PyMapping_Length(PyObject *o);

#define PyMapping_Length PyMapping_Size

#define PyMapping_DelItemString(O,K) PyObject_DelItemString((O),(K))

#define PyMapping_DelItem(O,K) PyObject_DelItem((O),(K))

     DL_IMPORT(int) PyMapping_HasKeyString(PyObject *o, char *key);

#define PyMapping_Keys(O) PyObject_CallMethod(O,"keys",NULL)

#define PyMapping_Values(O) PyObject_CallMethod(O,"values",NULL)

#define PyMapping_Items(O) PyObject_CallMethod(O,"items",NULL)

     DL_IMPORT(PyObject *) PyMapping_GetItemString(PyObject *o, char *key);

     DL_IMPORT(int) PyMapping_SetItemString(PyObject *o, char *key,
                                            PyObject *value);

DL_IMPORT(int) PyObject_IsInstance(PyObject *object, PyObject *typeorclass);
DL_IMPORT(int) PyObject_IsSubclass(PyObject *object, PyObject *typeorclass);

