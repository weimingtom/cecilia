//20180406
#pragma once

#define USE_VC6_MEMORY_LEAK 0

#define PYTHON_MEMORY_FILE_PATH "../../assets"

#define MS_NO_COREDLL



#include <io.h>
#define HAVE_LIMITS_H
#define HAVE_SYS_UTIME_H
#define HAVE_HYPOT
#define HAVE_TEMPNAM
#define HAVE_TMPFILE
#define HAVE_TMPNAM
#define DONT_HAVE_SIG_ALARM
#define DONT_HAVE_SIG_PAUSE
#define LONG_BIT	32
#define PREFIX ""
#define EXEC_PREFIX ""

#ifdef _MSC_VER
#ifdef _WIN64
#define MS_WIN64
#endif
#ifdef _WIN32
#define NT	
#define MS_WIN32
#endif
#ifdef _WIN16
#define MS_WIN16
#endif
#define MS_WINDOWS


#ifdef MS_WIN64
#ifdef _M_IX86
#define COMPILER "[MSC 64 bit (Intel)]"
#elif defined(_M_ALPHA)
#define COMPILER "[MSC 64 bit (Alpha)]"
#else
#define COMPILER "[MSC 64 bit (Unknown)]"
#endif
#endif

#if defined(MS_WIN32) && !defined(MS_WIN64)
#ifdef _M_IX86
#define COMPILER "[MSC 32 bit (Intel)]"
#elif defined(_M_ALPHA)
#define COMPILER "[MSC 32 bit (Alpha)]"
#else
#define COMPILER "[MSC (Unknown)]"
#endif
#endif

#endif



#if defined(_MSC_VER) && _MSC_VER > 850
#ifndef MS_NO_COREDLL
#define MS_COREDLL
#ifndef USE_DL_EXPORT
#define USE_DL_IMPORT
#endif
#endif

#define PYTHONPATH ".\\DLLs;.\\lib;.\\lib\\plat-win;.\\lib\\lib-tk"
typedef int pid_t;
#define WORD_BIT 32
#pragma warning(disable:4113)
#define hypot _hypot
#include <stdio.h>
#define HAVE_CLOCK
#define HAVE_STRFTIME
#define HAVE_STRERROR
#define NT_THREADS
#define WITH_THREAD
#ifndef NETSCAPE_PI
#define USE_SOCKET
#endif
#ifdef USE_DL_IMPORT
#define DL_IMPORT(RTYPE) __declspec(dllimport) RTYPE
#endif
#ifdef USE_DL_EXPORT
#define DL_IMPORT(RTYPE) __declspec(dllexport) RTYPE
#define DL_EXPORT(RTYPE) __declspec(dllexport) RTYPE
#endif

#define HAVE_LONG_LONG 1
#define LONG_LONG __int64
#endif

#ifdef __BORLANDC__
#define COMPILER "[Borland]"
#define HAVE_CLOCK
#define HAVE_STRFTIME

#ifdef _WIN32

#define NT
#define MS_WIN32
#define MS_WINDOWS

#ifndef MS_NO_COREDLL
#define MS_COREDLL
#ifndef USE_DL_EXPORT
#define USE_DL_IMPORT
#endif 
#endif

#define PYTHONPATH ".\\DLLs;.\\lib;.\\lib\\plat-win;.\\lib\\lib-tk"
typedef int pid_t;
#define WORD_BIT 32
#include <stdio.h>
#define HAVE_STRERROR
#define NT_THREADS
#define WITH_THREAD
#ifndef NETSCAPE_PI
#define USE_SOCKET
#endif
#ifdef USE_DL_IMPORT
#define DL_IMPORT(RTYPE) __declspec(dllimport) RTYPE
#endif
#ifdef USE_DL_EXPORT
#define DL_IMPORT(RTYPE) __declspec(dllexport) RTYPE
#define DL_EXPORT(RTYPE) __declspec(dllexport) RTYPE
#endif

#define HAVE_LONG_LONG 1
#define LONG_LONG __int64

#undef HAVE_SYS_UTIME_H
#define HAVE_UTIME_H
#define HAVE_DIRENT_H
#define HAVE_CLOCK

#else
#error "Only Win32 and later are supported"
#endif

#endif 

#if defined(__GNUC__) && defined(_WIN32)
#if (__GNUC__==2) && (__GNUC_MINOR__<=91)
#warning "Please use an up-to-date version of gcc! (>2.91 recommended)"
#endif

#define NT
#define MS_WIN32
#define MS_WINDOWS

#ifndef MS_NO_COREDLL
#define MS_COREDLL	
#ifndef USE_DL_EXPORT
#define USE_DL_IMPORT
#endif 
#endif 

#define COMPILER "[gcc]"
#define PYTHONPATH ".\\DLLs;.\\lib;.\\lib\\plat-win;.\\lib\\lib-tk"
#define WORD_BIT 32
#define hypot _hypot
#include <stdio.h>
#define HAVE_CLOCK
#define HAVE_STRFTIME
#define HAVE_STRERROR
#define NT_THREADS
#define WITH_THREAD
#ifndef NETSCAPE_PI
#define USE_SOCKET
#endif
#ifdef USE_DL_IMPORT
#define DL_IMPORT(RTYPE) __declspec(dllimport) RTYPE
#endif
#ifdef USE_DL_EXPORT
#define DL_IMPORT(RTYPE) __declspec(dllexport) RTYPE
#define DL_EXPORT(RTYPE) __declspec(dllexport) RTYPE
#endif

#define HAVE_LONG_LONG 1
#define LONG_LONG long long 
#endif

#if defined(__LCC__)

#define NT
#define MS_WIN32
#define MS_WINDOWS

#ifndef MS_NO_COREDLL
#define MS_COREDLL
#ifndef USE_DL_EXPORT
#define USE_DL_IMPORT
#endif
#endif

#define COMPILER "[lcc-win32]"
#define PYTHONPATH ".\\DLLs;.\\lib;.\\lib\\plat-win;.\\lib\\lib-tk"
typedef int pid_t;
#define WORD_BIT 32
#include <stdio.h>
#define HAVE_CLOCK
#define HAVE_STRFTIME
#define HAVE_STRERROR
#define NT_THREADS
#define WITH_THREAD
#ifndef NETSCAPE_PI
#define USE_SOCKET
#endif
#ifdef USE_DL_IMPORT
#define DL_IMPORT(RTYPE) __declspec(dllimport) RTYPE
#endif
#ifdef USE_DL_EXPORT
#define DL_IMPORT(RTYPE) __declspec(dllexport) RTYPE
#define DL_EXPORT(RTYPE) __declspec(dllexport) RTYPE
#endif

#define HAVE_LONG_LONG 1
#define LONG_LONG __int64
#endif

#if _MSC_VER >= 1200
#include <basetsd.h>
#endif

#if defined(MS_WIN64)
#	define PLATFORM "win32"
#	define SIZEOF_VOID_P 8
#	define SIZEOF_TIME_T 8
#	define SIZEOF_OFF_T 4
#	define SIZEOF_FPOS_T 8
#	define SIZEOF_HKEY 8
#	define HAVE_LARGEFILE_SUPPORT
#elif defined(MS_WIN32)
#	define PLATFORM "win32"
#	define HAVE_LARGEFILE_SUPPORT
#	ifdef _M_ALPHA
#		define SIZEOF_VOID_P 8
#		define SIZEOF_TIME_T 8
#	else
#		define SIZEOF_VOID_P 4
#		define SIZEOF_TIME_T 4
#		define SIZEOF_OFF_T 4
#		define SIZEOF_FPOS_T 8
#		define SIZEOF_HKEY 4
#	endif
#endif


#ifdef MS_WIN32

#if !defined(USE_DL_EXPORT) && defined(_MSC_VER)
#ifdef _DEBUG
//#pragma comment(lib,"python22_d.lib")
#else
//#pragma comment(lib,"python22.lib")
#endif
#endif

#ifdef _DEBUG
#define Py_DEBUG
#endif

#define SIZEOF_SHORT 2
#define SIZEOF_INT 4
#define SIZEOF_LONG 4
#define SIZEOF_LONG_LONG 8

#endif

#ifndef _ALL_SOURCE
#endif

// #define const  
// #define DIRENT 1 
// #undef GETGROUPS_T 
// #undef gid_t 
// #undef HAVE_TM_ZONE 
#define HAVE_TZNAME
// #undef _MINIX 
// #undef mode_t 
// #undef NDIR 
// #undef off_t 
// #undef pid_t 
// #undef _POSIX_1_SOURCE 
// #undef _POSIX_SOURCE 
#define RETSIGTYPE void
// #undef size_t 
#define socklen_t int
#define STDC_HEADERS 1
// #undef SYSDIR 
// #undef SYSNDIR 
// #undef TIME_WITH_SYS_TIME 
// #define TM_IN_SYS_TIME 1 
// #undef uid_t 
// #undef VOID_CLOSEDIR 
// #undef BAD_EXEC_PROTOTYPES 
#define BAD_STATIC_FORWARD 1
// #undef GETPGRP_HAVE_ARGS 
// #define HAVE_ALTZONE 
#ifdef MS_WIN32
#define HAVE_PUTENV
#endif
#define HAVE_PROTOTYPES
// #undef SYS_SELECT_WITH_SYS_TIME 
// #undef WITH_SGI_DL 
// #undef WITH_DL_DLD 
// #undef WITH_THREAD 
// #define WITH_READLINE 1 
#define Py_USING_UNICODE
#define PY_UNICODE_TYPE unsigned short
#define Py_UNICODE_SIZE SIZEOF_SHORT
#if Py_UNICODE_SIZE == 2
#define HAVE_USABLE_WCHAR_T
#endif
#define WITH_CYCLE_GC 1
// #define HAVE_CLOCK 
#define HAVE_DYNAMIC_LOADING
#define HAVE_FTIME
#define HAVE_GETPEERNAME
// #undef HAVE_GETPGRP 
#define HAVE_GETPID
// #undef HAVE_GETTIMEOFDAY 
// #undef HAVE_GETWD 
// #undef HAVE_LSTAT 
#define HAVE_MKTIME
// #undef HAVE_NICE 
// #undef HAVE_READLINK 
// #undef HAVE_SELECT 
// #undef HAVE_SETPGID 
// #undef HAVE_SETPGRP 
// #undef HAVE_SETSID 
#define HAVE_SETVBUF
// #undef HAVE_SIGINTERRUPT 
// #undef HAVE_SYMLINK 
// #undef HAVE_TCGETPGRP 
// #undef HAVE_TCSETPGRP 
// #undef HAVE_TIMES 
// #undef HAVE_UNAME 
// #undef HAVE_WAITPID 
// #undef HAVE_DLFCN_H 
#define HAVE_FCNTL_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_STDARG_H 1
#define HAVE_STDARG_PROTOTYPES
#define HAVE_STDDEF_H 1
#define HAVE_STDLIB_H 1
// #undef HAVE_SYS_AUDIOIO_H 
// #define HAVE_SYS_PARAM_H 1 
// #define HAVE_SYS_SELECT_H 1
// #define HAVE_SYS_TIME_H 1
// #define HAVE_SYS_TIMES_H 1
// #define HAVE_SYS_UN_H 1
// #define HAVE_SYS_UTIME_H 1
// #define HAVE_SYS_UTSNAME_H 1 
// #undef HAVE_THREAD_H
// #define HAVE_UNISTD_H 1 
// #define HAVE_UTIME_H 1
// #undef HAVE_LIBDL
// #undef HAVE_LIBMPC
#define HAVE_LIBNSL 1
// #undef HAVE_LIBSEQ
#define HAVE_LIBSOCKET 1
// #undef HAVE_LIBSUN
// #undef HAVE_LIBTERMCAP
// #undef HAVE_LIBTERMLIB 
// #undef HAVE_LIBTHREAD
