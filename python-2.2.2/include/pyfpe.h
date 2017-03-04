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

/*
 * This (following) is a heck of a way to decrement a counter. However,
 * unless the macro argument is provided, code optimizers will sometimes move
 * this statement so that it gets executed *before* the unsafe expression
 * which we're trying to protect.  That pretty well messes things up,
 * of course.
 * 
 * If the expression(s) you're trying to protect don't happen to return a
 * value, you will need to manufacture a dummy result just to preserve the
 * correct ordering of statements.  Note that the macro passes the address
 * of its argument (so you need to give it something which is addressable).
 * If your expression returns multiple results, pass the last such result
 * to PyFPE_END_PROTECT.
 * 
 * Note that PyFPE_dummy returns a double, which is cast to int.
 * This seeming insanity is to tickle the Floating Point Unit (FPU).
 * If an exception has occurred in a preceding floating point operation,
 * some architectures (notably Intel 80x86) will not deliver the interrupt
 * until the *next* floating point operation.  This is painful if you've
 * already decremented PyFPE_counter.
 */
#define PyFPE_END_PROTECT(v) PyFPE_counter -= (int)PyFPE_dummy(&(v));

#else

#define PyFPE_START_PROTECT(err_string, leave_stmt)
#define PyFPE_END_PROTECT(v)

#endif
