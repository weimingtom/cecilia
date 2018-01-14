//20180114
#include "pyconfig.h"
#include "pyfpe.h"

#ifdef WANT_SIGFPE_HANDLER
jmp_buf PyFPE_jbuf;
int PyFPE_counter = 0;
#endif

double PyFPE_dummy(void *dummy)
{
	return 1.0;
}
