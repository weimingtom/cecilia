//20180406

#include "python.h"

#include <winsock.h>

static PyMethodDef errno_methods[] = {
	{NULL,	      	NULL}
};

static void _inscode(PyObject *d, PyObject *de, char *name, int code)
{
	PyObject *u = PyString_FromString(name);
	PyObject *v = PyInt_FromLong((long) code);

	if (u && v) 
	{
		PyDict_SetItem(d, u, v);
		PyDict_SetItem(de, v, u);
	}
	Py_XDECREF(u);
	Py_XDECREF(v);
}

static char errno__doc__ [] =
	"This module makes available standard errno system symbols.\n"
	"\n"
	"The value of each symbol is the corresponding integer value,\n"
	"e.g., on most systems, errno.ENOENT equals the integer 2.\n"
	"\n"
	"The dictionary errno.errorcode maps numeric codes to symbol names,\n"
	"e.g., errno.errorcode[2] could be the string 'ENOENT'.\n"
	"\n"
	"Symbols that are not relevant to the underlying system are not defined.\n"
	"\n"
	"To map error codes to error messages, use the function os.strerror(),\n"
	"e.g. os.strerror(2) could return 'No such file or directory'.";

DL_EXPORT(void) initerrno()
{
	PyObject *m, *d, *de;
	m = Py_InitModule3("errno", errno_methods, errno__doc__);
	d = PyModule_GetDict(m);
	de = PyDict_New();
	if (!d || !de || PyDict_SetItemString(d, "errorcode", de) < 0)
	{
		return;
	}

#define inscode(d, ds, de, name, code, comment) _inscode(d, de, name, code)

	inscode(d, ds, de, "ENODEV", ENODEV, "No such device");
	inscode(d, ds, de, "EHOSTUNREACH", WSAEHOSTUNREACH, "No route to host");
	inscode(d, ds, de, "ENOSYS", ENOSYS, "Function not implemented");
	inscode(d, ds, de, "EPIPE", EPIPE, "Broken pipe");
	inscode(d, ds, de, "EINVAL", EINVAL, "Invalid argument");
	inscode(d, ds, de, "EINTR", EINTR, "Interrupted system call");
	inscode(d, ds, de, "EUSERS", WSAEUSERS, "Too many users");
	inscode(d, ds, de, "ENOTEMPTY", ENOTEMPTY, "Directory not empty");
	inscode(d, ds, de, "ENOBUFS", WSAENOBUFS, "No buffer space available");
	inscode(d, ds, de, "EREMOTE", WSAEREMOTE, "Object is remote");
	inscode(d, ds, de, "ECHILD", ECHILD, "No child processes");
	inscode(d, ds, de, "ELOOP", WSAELOOP, "Too many symbolic links encountered");
	inscode(d, ds, de, "EXDEV", EXDEV, "Cross-device link");
	inscode(d, ds, de, "E2BIG", E2BIG, "Arg list too long");
	inscode(d, ds, de, "ESRCH", ESRCH, "No such process");
	inscode(d, ds, de, "EMSGSIZE", WSAEMSGSIZE, "Message too long");
	inscode(d, ds, de, "EAFNOSUPPORT", WSAEAFNOSUPPORT, "Address family not supported by protocol");
	inscode(d, ds, de, "EHOSTDOWN", WSAEHOSTDOWN, "Host is down");
	inscode(d, ds, de, "EPFNOSUPPORT", WSAEPFNOSUPPORT, "Protocol family not supported");
	inscode(d, ds, de, "ENOPROTOOPT", WSAENOPROTOOPT, "Protocol not available");
	inscode(d, ds, de, "EBUSY", EBUSY, "Device or resource busy");
	inscode(d, ds, de, "EWOULDBLOCK", WSAEWOULDBLOCK, "Operation would block");
	inscode(d, ds, de, "EISCONN", WSAEISCONN, "Transport endpoint is already connected");
	inscode(d, ds, de, "ESHUTDOWN", WSAESHUTDOWN, "Cannot send after transport endpoint shutdown");
	inscode(d, ds, de, "EBADF", EBADF, "Bad file number");
	inscode(d, ds, de, "EIO", EIO, "I/O error");
	inscode(d, ds, de, "EPROTOTYPE", WSAEPROTOTYPE, "Protocol wrong type for socket");
	inscode(d, ds, de, "ENOSPC", ENOSPC, "No space left on device");
	inscode(d, ds, de, "ENOEXEC", ENOEXEC, "Exec format error");
	inscode(d, ds, de, "EALREADY", WSAEALREADY, "Operation already in progress");
	inscode(d, ds, de, "ENETDOWN", WSAENETDOWN, "Network is down");
	inscode(d, ds, de, "EACCES", EACCES, "Permission denied");
	inscode(d, ds, de, "EILSEQ", EILSEQ, "Illegal byte sequence");
	inscode(d, ds, de, "ENOTDIR", ENOTDIR, "Not a directory");
	inscode(d, ds, de, "EPERM", EPERM, "Operation not permitted");
	inscode(d, ds, de, "EDOM", EDOM, "Math argument out of domain of func");
	inscode(d, ds, de, "ECONNREFUSED", WSAECONNREFUSED, "Connection refused");
	inscode(d, ds, de, "EISDIR", EISDIR, "Is a directory");
	inscode(d, ds, de, "EPROTONOSUPPORT", WSAEPROTONOSUPPORT, "Protocol not supported");
	inscode(d, ds, de, "EROFS", EROFS, "Read-only file system");
	inscode(d, ds, de, "EADDRNOTAVAIL", WSAEADDRNOTAVAIL, "Cannot assign requested address");
	inscode(d, ds, de, "ENFILE", ENFILE, "File table overflow");
	inscode(d, ds, de, "ESPIPE", ESPIPE, "Illegal seek");
	inscode(d, ds, de, "ENETRESET", WSAENETRESET, "Network dropped connection because of reset");
	inscode(d, ds, de, "ETIMEDOUT", WSAETIMEDOUT, "Connection timed out");
	inscode(d, ds, de, "ENOENT", ENOENT, "No such file or directory");
	inscode(d, ds, de, "EEXIST", EEXIST, "File exists");
	inscode(d, ds, de, "EDQUOT", WSAEDQUOT, "Quota exceeded");
	inscode(d, ds, de, "EFAULT", EFAULT, "Bad address");
	inscode(d, ds, de, "EFBIG", EFBIG, "File too large");
	inscode(d, ds, de, "EDEADLK", EDEADLK, "Resource deadlock would occur");
	inscode(d, ds, de, "ENOTCONN", WSAENOTCONN, "Transport endpoint is not connected");
	inscode(d, ds, de, "EDESTADDRREQ", WSAEDESTADDRREQ, "Destination address required");
	inscode(d, ds, de, "ENOLCK", ENOLCK, "No record locks available");
	inscode(d, ds, de, "ECONNABORTED", WSAECONNABORTED, "Software caused connection abort");
	inscode(d, ds, de, "ENETUNREACH", WSAENETUNREACH, "Network is unreachable");
	inscode(d, ds, de, "ESTALE", WSAESTALE, "Stale NFS file handle");
	inscode(d, ds, de, "ENOMEM", ENOMEM, "Out of memory");
	inscode(d, ds, de, "ENOTSOCK", WSAENOTSOCK, "Socket operation on non-socket");
	inscode(d, ds, de, "EMLINK", EMLINK, "Too many links");
	inscode(d, ds, de, "ERANGE", ERANGE, "Math result not representable");
	inscode(d, ds, de, "ECONNRESET", WSAECONNRESET, "Connection reset by peer");
	inscode(d, ds, de, "EADDRINUSE", WSAEADDRINUSE, "Address already in use");
	inscode(d, ds, de, "EOPNOTSUPP", WSAEOPNOTSUPP, "Operation not supported on transport endpoint");
	inscode(d, ds, de, "EAGAIN", EAGAIN, "Try again");
	inscode(d, ds, de, "ENAMETOOLONG", ENAMETOOLONG, "File name too long");
	inscode(d, ds, de, "ENOTTY", ENOTTY, "Not a typewriter");
	inscode(d, ds, de, "ESOCKTNOSUPPORT", WSAESOCKTNOSUPPORT, "Socket type not supported");
	inscode(d, ds, de, "EDEADLOCK", EDEADLOCK, "Error EDEADLOCK");
	inscode(d, ds, de, "ETOOMANYREFS", WSAETOOMANYREFS, "Too many references: cannot splice");
	inscode(d, ds, de, "EMFILE", EMFILE, "Too many open files");
	inscode(d, ds, de, "EINPROGRESS", WSAEINPROGRESS, "Operation now in progress");
	inscode(d, ds, de, "ENXIO", ENXIO, "No such device or address");
	inscode(d, ds, de, "WSAEHOSTDOWN", WSAEHOSTDOWN, "Host is down");
	inscode(d, ds, de, "WSAENETDOWN", WSAENETDOWN, "Network is down");
	inscode(d, ds, de, "WSAENOTSOCK", WSAENOTSOCK, "Socket operation on non-socket");
	inscode(d, ds, de, "WSAEHOSTUNREACH", WSAEHOSTUNREACH, "No route to host");
	inscode(d, ds, de, "WSAELOOP", WSAELOOP, "Too many symbolic links encountered");
	inscode(d, ds, de, "WSAEMFILE", WSAEMFILE, "Too many open files");
	inscode(d, ds, de, "WSAESTALE", WSAESTALE, "Stale NFS file handle");
	inscode(d, ds, de, "WSAVERNOTSUPPORTED", WSAVERNOTSUPPORTED, "Error WSAVERNOTSUPPORTED");
	inscode(d, ds, de, "WSAENETUNREACH", WSAENETUNREACH, "Network is unreachable");
	inscode(d, ds, de, "WSAEPROCLIM", WSAEPROCLIM, "Error WSAEPROCLIM");
	inscode(d, ds, de, "WSAEFAULT", WSAEFAULT, "Bad address");
	inscode(d, ds, de, "WSANOTINITIALISED", WSANOTINITIALISED, "Error WSANOTINITIALISED");
	inscode(d, ds, de, "WSAEUSERS", WSAEUSERS, "Too many users");
	inscode(d, ds, de, "WSAENOPROTOOPT", WSAENOPROTOOPT, "Protocol not available");
	inscode(d, ds, de, "WSAECONNABORTED", WSAECONNABORTED, "Software caused connection abort");
	inscode(d, ds, de, "WSAENAMETOOLONG", WSAENAMETOOLONG, "File name too long");
	inscode(d, ds, de, "WSAENOTEMPTY", WSAENOTEMPTY, "Directory not empty");
	inscode(d, ds, de, "WSAESHUTDOWN", WSAESHUTDOWN, "Cannot send after transport endpoint shutdown");
	inscode(d, ds, de, "WSAEAFNOSUPPORT", WSAEAFNOSUPPORT, "Address family not supported by protocol");
	inscode(d, ds, de, "WSAETOOMANYREFS", WSAETOOMANYREFS, "Too many references: cannot splice");
	inscode(d, ds, de, "WSAEACCES", WSAEACCES, "Permission denied");
	inscode(d, ds, de, "WSABASEERR", WSABASEERR, "Error WSABASEERR");
	inscode(d, ds, de, "WSAEMSGSIZE", WSAEMSGSIZE, "Message too long");
	inscode(d, ds, de, "WSAEBADF", WSAEBADF, "Bad file number");
	inscode(d, ds, de, "WSAECONNRESET", WSAECONNRESET, "Connection reset by peer");
	inscode(d, ds, de, "WSAETIMEDOUT", WSAETIMEDOUT, "Connection timed out");
	inscode(d, ds, de, "WSAENOBUFS", WSAENOBUFS, "No buffer space available");
	inscode(d, ds, de, "WSAEDISCON", WSAEDISCON, "Error WSAEDISCON");
	inscode(d, ds, de, "WSAEINTR", WSAEINTR, "Interrupted system call");
	inscode(d, ds, de, "WSAEPROTOTYPE", WSAEPROTOTYPE, "Protocol wrong type for socket");
	inscode(d, ds, de, "WSAEADDRINUSE", WSAEADDRINUSE, "Address already in use");
	inscode(d, ds, de, "WSAEADDRNOTAVAIL", WSAEADDRNOTAVAIL, "Cannot assign requested address");
	inscode(d, ds, de, "WSAEALREADY", WSAEALREADY, "Operation already in progress");
	inscode(d, ds, de, "WSAEPROTONOSUPPORT", WSAEPROTONOSUPPORT, "Protocol not supported");
	inscode(d, ds, de, "WSASYSNOTREADY", WSASYSNOTREADY, "Error WSASYSNOTREADY");
	inscode(d, ds, de, "WSAEWOULDBLOCK", WSAEWOULDBLOCK, "Operation would block");
	inscode(d, ds, de, "WSAEPFNOSUPPORT", WSAEPFNOSUPPORT, "Protocol family not supported");
	inscode(d, ds, de, "WSAEOPNOTSUPP", WSAEOPNOTSUPP, "Operation not supported on transport endpoint");
	inscode(d, ds, de, "WSAEISCONN", WSAEISCONN, "Transport endpoint is already connected");
	inscode(d, ds, de, "WSAEDQUOT", WSAEDQUOT, "Quota exceeded");
	inscode(d, ds, de, "WSAENOTCONN", WSAENOTCONN, "Transport endpoint is not connected");
	inscode(d, ds, de, "WSAEREMOTE", WSAEREMOTE, "Object is remote");
	inscode(d, ds, de, "WSAEINVAL", WSAEINVAL, "Invalid argument");
	inscode(d, ds, de, "WSAEINPROGRESS", WSAEINPROGRESS, "Operation now in progress");
	inscode(d, ds, de, "WSAESOCKTNOSUPPORT", WSAESOCKTNOSUPPORT, "Socket type not supported");
	inscode(d, ds, de, "WSAEDESTADDRREQ", WSAEDESTADDRREQ, "Destination address required");
	inscode(d, ds, de, "WSAECONNREFUSED", WSAECONNREFUSED, "Connection refused");
	inscode(d, ds, de, "WSAENETRESET", WSAENETRESET, "Network dropped connection because of reset");

	Py_DECREF(de);
}
