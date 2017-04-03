#pragma once

typedef unsigned short digit;
typedef unsigned int wdigit; /* digit widened to parameter size */
#define BASE_TWODIGITS_TYPE long
typedef unsigned BASE_TWODIGITS_TYPE twodigits;
typedef BASE_TWODIGITS_TYPE stwodigits; /* signed variant of twodigits */

#define SHIFT	15
#define BASE	((digit)1 << SHIFT)
#define MASK	((int)(BASE - 1))

/* Long integer representation.
   The absolute value of a number is equal to
   	SUM(for i=0 through abs(ob_size)-1) ob_digit[i] * 2**(SHIFT*i)
   Negative numbers are represented with ob_size < 0;
   zero is represented by ob_size == 0.
   In a normalized number, ob_digit[abs(ob_size)-1] (the most significant
   digit) is never zero.  Also, in all cases, for all valid i,
   	0 <= ob_digit[i] <= MASK.
   The allocation function takes care of allocating extra memory
   so that ob_digit[0] ... ob_digit[abs(ob_size)-1] are actually available. */

struct _longobject {
	PyObject_HEAD
	int ob_size;
	digit ob_digit[1];
};

DL_IMPORT(PyLongObject *) _PyLong_New(int);

/* Return a copy of src. */
DL_IMPORT(PyObject *) _PyLong_Copy(PyLongObject *src);
