#pragma once

#ifndef _GNU_SOURCE
# define _GNU_SOURCE	1
#endif

/* Forcing SUSv2 compatibility still produces problems on some
   platforms, True64 and SGI IRIX begin two of them, so for now the
   define is switched off. */
#if 0
#ifndef _XOPEN_SOURCE
# define _XOPEN_SOURCE	500
#endif
#endif

/* Include nearly all Python header files */

#include "patchlevel.h"
#include "pyconfig.h"

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

/* pyconfig.h may or may not define DL_IMPORT */
#ifndef DL_IMPORT	/* declarations for DLL import/export */
#define DL_IMPORT(RTYPE) RTYPE
#endif
#ifndef DL_EXPORT	/* declarations for DLL import/export */
#define DL_EXPORT(RTYPE) RTYPE
#endif

#if defined(_MSC_VER) && defined(_DEBUG) && USE_VC6_MEMORY_LEAK

/*
see 
http://www.nokuno.jp/secret/program2.html
*/
#include <crtdbg.h>
#include <malloc.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
/*
#define new  ::new(_NORMAL_BLOCK, __file__, __line__)
*/

#ifdef malloc
#undef malloc
#endif
#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef free
#undef free
#endif
#define free(s) (_free_dbg(s, _NORMAL_BLOCK))

#ifdef calloc
#undef calloc
#endif
#define calloc(m, s) (_calloc_dbg(m, s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef realloc
#undef realloc
#endif
#define realloc(m, s) (_realloc_dbg(m, s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef new
#undef new
#endif
#define new DEBUG_NEW

/*
On Windows, when application start: 
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
*/
#endif

#if defined(__sgi) && defined(WITH_THREAD) && !defined(_SGI_MP_SOURCE)
#define _SGI_MP_SOURCE
#endif

#include <stdio.h>
#ifndef NULL
#   error "python.h requires that stdio.h define NULL."
#endif

#include <string.h>
#include <errno.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* CAUTION:  Build setups should ensure that NDEBUG is defined on the
 * compiler command line when building Python in release mode; else
 * assert() calls won't be removed.
 */
#include <assert.h>

#include "pyport.h"

#include "pymem.h"

#include "object.h"
#include "objimpl.h"

#include "pydebug.h"

#include "unicodeobject.h"
#include "intobject.h"
#include "longobject.h"
#include "floatobject.h"
#ifndef WITHOUT_COMPLEX
#include "complexobject.h"
#endif
#include "rangeobject.h"
#include "stringobject.h"
#include "bufferobject.h"
#include "tupleobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "methodobject.h"
#include "moduleobject.h"
#include "funcobject.h"
#include "classobject.h"
#include "fileobject.h"
#include "cobject.h"
#include "traceback.h"
#include "sliceobject.h"
#include "cellobject.h"
#include "iterobject.h"
#include "descrobject.h"
#include "weakrefobject.h"

#include "codecs.h"
#include "pyerrors.h"

#include "pystate.h"

#include "modsupport.h"
#include "pythonrun.h"
#include "ceval.h"
#include "sysmodule.h"
#include "intrcheck.h"
#include "import.h"

#include "abstract.h"

#define PyArg_GetInt(v, a)	PyArg_Parse((v), "i", (a))
#define PyArg_NoArgs(v)		PyArg_Parse(v, "")

/* Convert a possibly signed character to a nonnegative int */
/* XXX This assumes characters are 8 bits wide */
#ifdef __CHAR_UNSIGNED__
#define Py_CHARMASK(c)		(c)
#else
#define Py_CHARMASK(c)		((c) & 0xff)
#endif

#include "pyfpe.h"

/* These definitions must match corresponding definitions in graminit.h.
   There's code in compile.c that checks that they are the same. */
#define Py_single_input 256
#define Py_file_input 257
#define Py_eval_input 258

#ifdef HAVE_PTH
/* GNU pth user-space thread support */
#include <pth.h>
#endif
