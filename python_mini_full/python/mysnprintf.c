//20180123
#include "python.h"
#include <ctype.h>

int PyOS_snprintf(char *str, size_t size, const  char  *format, ...)
{
	int rc;
	va_list va;

	va_start(va, format);
	rc = PyOS_vsnprintf(str, size, format, va);
	va_end(va);
	return rc;
}

int PyOS_vsnprintf(char *str, size_t size, const char  *format, va_list va)
{
	int len;
#ifndef HAVE_SNPRINTF
	char *buffer;
#endif
	assert(str != NULL);
	assert(size > 0);
	assert(format != NULL);

#ifdef HAVE_SNPRINTF
	len = vsnprintf(str, size, format, va);
#else
	buffer = PyMem_MALLOC(size + 512);
	if (buffer == NULL) 
	{
		len = -666;
		goto Done;
	}

	len = vsprintf(buffer, format, va);
	if (len < 0)
	{
		;
	}
	else if ((size_t)len >= size + 512)
	{
		Py_FatalError("Buffer overflow in PyOS_snprintf/PyOS_vsnprintf");
	}
	else 
	{
		const size_t to_copy = (size_t)len < size ?	(size_t)len : size - 1;
		assert(to_copy < size);
		memcpy(str, buffer, to_copy);
		str[to_copy] = '\0';
	}
	PyMem_FREE(buffer);
Done:
#endif
	str[size-1] = '\0';
	return len;
}
