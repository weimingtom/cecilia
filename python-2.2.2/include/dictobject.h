#pragma once

#define PyDict_MINSIZE 8

typedef struct {
	long me_hash;      /* cached hash code of me_key */
	PyObject *me_key;
	PyObject *me_value;
#ifdef USE_CACHE_ALIGNED
	long	aligner;
#endif
} PyDictEntry;

/*
To ensure the lookup algorithm terminates, there must be at least one Unused
slot (NULL key) in the table.
The value ma_fill is the number of non-NULL keys (sum of Active and Dummy);
ma_used is the number of non-NULL, non-dummy keys (== the number of non-NULL
values == the number of Active items).
To avoid slowing down lookups on a near-full table, we resize the table when
it's two-thirds full.
*/
typedef struct _dictobject PyDictObject;
struct _dictobject {
	PyObject_HEAD
	int ma_fill;  /* # Active + # Dummy */
	int ma_used;  /* # Active */

	/* The table contains ma_mask + 1 slots, and that's a power of 2.
	 * We store the mask instead of the size because the mask is more
	 * frequently needed.
	 */
	int ma_mask;

	/* ma_table points to ma_smalltable for small tables, else to
	 * additional malloc'ed memory.  ma_table is never NULL!  This rule
	 * saves repeated runtime null-tests in the workhorse getitem and
	 * setitem calls.
	 */
	PyDictEntry *ma_table;
	PyDictEntry *(*ma_lookup)(PyDictObject *mp, PyObject *key, long hash);
	PyDictEntry ma_smalltable[PyDict_MINSIZE];
};

extern DL_IMPORT(PyTypeObject) PyDict_Type;

#define PyDict_Check(op) PyObject_TypeCheck(op, &PyDict_Type)

extern DL_IMPORT(PyObject *) PyDict_New(void);
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

/* PyDict_Update(mp, other) is equivalent to PyDict_Merge(mp, other, 1). */
extern DL_IMPORT(int) PyDict_Update(PyObject *mp, PyObject *other);

/* PyDict_Merge updates/merges from a mapping object (an object that
   supports PyMapping_Keys() and PyObject_GetItem()).  If override is true,
   the last occurrence of a key wins, else the first.  The Python
   dict.update(other) is equivalent to PyDict_Merge(dict, other, 1).
*/
extern DL_IMPORT(int) PyDict_Merge(PyObject *mp,
				   PyObject *other,
				   int override);

/* PyDict_MergeFromSeq2 updates/merges from an iterable object producing
   iterable objects of length 2.  If override is true, the last occurrence
   of a key wins, else the first.  The Python dict constructor dict(seq2)
   is equivalent to dict={}; PyDict_MergeFromSeq(dict, seq2, 1).
*/
extern DL_IMPORT(int) PyDict_MergeFromSeq2(PyObject *d,
					   PyObject *seq2,
					   int override);

extern DL_IMPORT(PyObject *) PyDict_GetItemString(PyObject *dp, char *key);
extern DL_IMPORT(int) PyDict_SetItemString(PyObject *dp, char *key, PyObject *item);
extern DL_IMPORT(int) PyDict_DelItemString(PyObject *dp, char *key);
