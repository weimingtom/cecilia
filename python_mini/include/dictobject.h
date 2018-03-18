//20180318
#pragma once

#define PyDict_MINSIZE 8

typedef struct {
	long me_hash;
	PyObject *me_key;
	PyObject *me_value;
#ifdef USE_CACHE_ALIGNED
	long aligner;
#endif
} PyDictEntry;

typedef struct _dictobject PyDictObject;
struct _dictobject {
	PyObject_HEAD
	int ma_fill;
	int ma_used;
	int ma_mask;
	PyDictEntry *ma_table;
	PyDictEntry *(*ma_lookup)(PyDictObject *mp, PyObject *key, long hash);
	PyDictEntry ma_smalltable[PyDict_MINSIZE];
};

extern DL_IMPORT(PyTypeObject) PyDict_Type;

#define PyDict_Check(op) PyObject_TypeCheck(op, &PyDict_Type)

extern DL_IMPORT(PyObject *) PyDict_New();
extern DL_IMPORT(PyObject *) PyDict_GetItem(PyObject *mp, PyObject *key);
extern DL_IMPORT(int) PyDict_SetItem(PyObject *mp, PyObject *key, PyObject *item);
extern DL_IMPORT(int) PyDict_DelItem(PyObject *mp, PyObject *key);
extern DL_IMPORT(void) PyDict_Clear(PyObject *mp);
extern DL_IMPORT(int) PyDict_Next
	(PyObject *mp, int *pos, PyObject **key, PyObject **value);
extern DL_IMPORT(PyObject *) PyDict_Keys(PyObject *mp);
extern DL_IMPORT(PyObject *) PyDict_Values(PyObject *mp);
extern DL_IMPORT(PyObject *) PyDict_Items(PyObject *mp);
extern DL_IMPORT(int) PyDict_Size(PyObject *mp);
extern DL_IMPORT(PyObject *) PyDict_Copy(PyObject *mp);

extern DL_IMPORT(int) PyDict_Update(PyObject *mp, PyObject *other);

extern DL_IMPORT(int) PyDict_Merge(PyObject *mp,
				   PyObject *other,
				   int override);

extern DL_IMPORT(int) PyDict_MergeFromSeq2(PyObject *d,
					   PyObject *seq2,
					   int override);

extern DL_IMPORT(PyObject *) PyDict_GetItemString(PyObject *dp, char *key);
extern DL_IMPORT(int) PyDict_SetItemString(PyObject *dp, char *key, PyObject *item);
extern DL_IMPORT(int) PyDict_DelItemString(PyObject *dp, char *key);
