//20180118
#pragma once

#ifdef WANT_SIGFPE_HANDLER
#include <signal.h>
#include <setjmp.h>
#include <math.h>
extern jmp_buf PyFPE_jbuf;
extern int PyFPE_counter;
extern double PyFPE_dummy(void *);

#define PyFPE_START_PROTECT(err_string, leave_stmt) \
if (!PyFPE_counter++ && setjmp(PyFPE_jbuf)) { \
	PyErr_SetString(PyExc_FloatingPointError, err_string); \
	PyFPE_counter = 0; \
	leave_stmt; \
}

#define PyFPE_END_PROTECT(v) PyFPE_counter -= (int)PyFPE_dummy(&(v));

#else

#define PyFPE_START_PROTECT(err_string, leave_stmt)
#define PyFPE_END_PROTECT(v)

#endif
