//20180324
#pragma once

#include "pyconfig.h" 

#define ANY void
#ifdef HAVE_PROTOTYPES
#define Py_PROTO(x) x
#else
#define Py_PROTO(x) ()
#endif
#ifndef Py_FPROTO
#define Py_FPROTO(x) Py_PROTO(x)
#endif

#ifndef LONG_LONG
#define LONG_LONG long long
#endif

#ifdef HAVE_UINTPTR_T
typedef uintptr_t	Py_uintptr_t;
typedef intptr_t	Py_intptr_t;

#elif SIZEOF_VOID_P <= SIZEOF_INT
typedef unsigned int	Py_uintptr_t;
typedef int		Py_intptr_t;

#elif SIZEOF_VOID_P <= SIZEOF_LONG
typedef unsigned long	Py_uintptr_t;
typedef long		Py_intptr_t;

#else
#   error "Python needs a typedef for Py_uintptr_t in pyport.h."
#endif 

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <math.h> 

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else 
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else 
#include <time.h>
#endif
#endif

#ifdef HAVE_SYS_SELECT_H

#include <sys/select.h>

#else 


#endif 

#define HAVE_STAT

#ifndef DONT_HAVE_FSTAT
#define HAVE_FSTAT
#endif

#include <sys/stat.h>

#ifndef S_ISREG
#define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif

#ifdef SIGNED_RIGHT_SHIFT_ZERO_FILLS
#define Py_ARITHMETIC_RIGHT_SHIFT(TYPE, I, J) \
	((I) < 0 ? ~((~(unsigned TYPE)(I)) >> (J)) : (I) >> (J))
#else
#define Py_ARITHMETIC_RIGHT_SHIFT(TYPE, I, J) ((I) >> (J))
#endif

#define Py_FORCE_EXPANSION(X) X

#ifdef Py_DEBUG
#define Py_SAFE_DOWNCAST(VALUE, WIDE, NARROW) \
	(assert((WIDE)(NARROW)(VALUE) == (VALUE)), (NARROW)(VALUE))
#else
#define Py_SAFE_DOWNCAST(VALUE, WIDE, NARROW) (NARROW)(VALUE)
#endif

#define Py_IS_INFINITY(X) ((X) && (X)*0.5 == (X))

#ifdef INFINITY
#define Py_HUGE_VAL ((double)INFINITY)
#else
#define Py_HUGE_VAL HUGE_VAL
#endif

#define Py_OVERFLOWED(X) ((X) != 0.0 && (errno == ERANGE ||    \
					 (X) == Py_HUGE_VAL || \
					 (X) == -Py_HUGE_VAL))

#define Py_SET_ERANGE_IF_OVERFLOW(X) \
	do { \
		if (errno == 0 && ((X) == Py_HUGE_VAL ||  \
				   (X) == -Py_HUGE_VAL))  \
			errno = ERANGE; \
	} while(0)

#define Py_ADJUST_ERANGE1(X)						\
	do {								\
		if (errno == 0) {					\
			if ((X) == Py_HUGE_VAL || (X) == -Py_HUGE_VAL)	\
				errno = ERANGE;				\
		}							\
		else if (errno == ERANGE && (X) == 0.0)			\
			errno = 0;					\
	} while(0)

#define Py_ADJUST_ERANGE2(X, Y)						\
	do {								\
		if ((X) == Py_HUGE_VAL || (X) == -Py_HUGE_VAL ||	\
		    (Y) == Py_HUGE_VAL || (Y) == -Py_HUGE_VAL) {	\
				if (errno == 0)				\
					errno = ERANGE;			\
		}							\
		else if (errno == ERANGE)				\
			errno = 0;					\
	} while(0)

#ifdef SOLARIS
extern int gethostname(char *, int);
#endif

#ifdef __BEOS__
int shutdown( int, int );
#endif

#ifdef HAVE__GETPTY
#include <sys/types.h>
extern char * _getpty(int *, int, mode_t, int);
#endif

#if defined(HAVE_OPENPTY) || defined(HAVE_FORKPTY)
#if !defined(HAVE_PTY_H) && !defined(HAVE_LIBUTIL_H)
#include <termios.h>
extern int openpty(int *, int *, char *, struct termios *, struct winsize *);
extern int forkpty(int *, char *, struct termios *, struct winsize *);
#endif
#endif


#if 0

extern int getrusage();
extern int getpagesize();

extern int fclose(FILE *);

extern int fdatasync(int);
extern int rename(const char *, const char *);
extern int pclose(FILE *);
extern int lstat(const char *, struct stat *);
extern int symlink(const char *, const char *);
extern int fsync(int fd);

#endif 

#ifndef HAVE_HYPOT
extern double hypot(double, double);
#endif

#ifndef DL_IMPORT    
#define DL_IMPORT(RTYPE) RTYPE
#endif

#ifdef MALLOC_ZERO_RETURNS_NULL
#define _PyMem_EXTRA 1
#else
#define _PyMem_EXTRA 0
#endif


#if 0 

#ifndef	FD_SETSIZE
#define	FD_SETSIZE	256
#endif

#ifndef FD_SET

typedef long fd_mask;

#define NFDBITS	(sizeof(fd_mask) * NBBY)
#ifndef howmany
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif

typedef	struct fd_set {
	fd_mask	fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)	memset((char *)(p), '\0', sizeof(*(p)))

#endif

#endif





#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef LONG_MAX
#if SIZEOF_LONG == 4
#define LONG_MAX 0X7FFFFFFFL
#elif SIZEOF_LONG == 8
#define LONG_MAX 0X7FFFFFFFFFFFFFFFL
#else
#error "could not set LONG_MAX in pyport.h"
#endif
#endif

#ifndef LONG_MIN
#define LONG_MIN (-LONG_MAX-1)
#endif

#ifndef LONG_BIT
#define LONG_BIT (8 * SIZEOF_LONG)
#endif

#if LONG_BIT != 8 * SIZEOF_LONG
#error "LONG_BIT definition appears wrong for platform (bad gcc/glibc config?)."
#endif

#ifdef __BORLANDC__
#  include <io.h>
#  define _chsize chsize
#  define _setmode setmode
#endif

#if (!defined(__GNUC__) || __GNUC__ < 2 || \
     (__GNUC__ == 2 && __GNUC_MINOR__ < 7) || \
     defined(NEXT) )
#define __attribute__(__x)
#endif
