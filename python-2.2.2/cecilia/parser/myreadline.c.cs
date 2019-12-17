#define _DEBUG

using System;

namespace Cecilia
{
	using size_t = System.UInt32;
	
	public partial class Python
	{
		//20170403
		
		//#include "python.h"
		
		public delegate int PyOS_InputHook_delegate();
		public static PyOS_InputHook_delegate PyOS_InputHook = null;
		
		private static int my_fgets(CharPtr buf, int len, FILEPtr fp)
		{
			CharPtr p;
			for (;;) 
			{
				if (PyOS_InputHook != null)
				{
					PyOS_InputHook();
				}
				errno = 0;
				p = fgets(buf, len, fp);
				if (p != null)
				{
					return 0;
				}
				if (0!=feof(fp)) 
				{
					return -1;
				}
				if (errno == EINTR) 
				{
					if (0!=PyOS_InterruptOccurred()) 
					{
						return 1;
					}
					continue;
				}
				if (0!=PyOS_InterruptOccurred()) 
				{
					return 1;
				}
				return -2;
			}
		}
		
		public static CharPtr PyOS_StdioReadline(CharPtr prompt)
		{
			size_t n;
			CharPtr p;
		
			n = 100;
			if ((p = PyMem_MALLOC(n)) == null)
			{
				return null;
			}
			fflush(stdout);
			if (prompt!=null)
			{
				fprintf(stderr, "%s", prompt);
			}
			fflush(stderr);
			switch (my_fgets(p, (int)n, stdin)) 
			{
			case 0:
				break;
			
			case 1:
				PyMem_FREE(ref p);
				return null;
			
			case -1:
			case -2:
			default:
				p[0] = '\0';
				break;
			}
			n = strlen(p);
			while (n > 0 && p[n-1] != '\n') 
			{
				size_t incr = n+2;
				p = PyMem_REALLOC(p, n + incr);
				if (p == null)
				{
					return null;
				}
				if (incr > INT_MAX) 
				{
					PyErr_SetString(PyExc_OverflowError, "input line too long");
				}
				if (my_fgets(p+n, (int)incr, stdin) != 0)
				{
					break;
				}
				n += strlen(p+n);
			}
			return PyMem_REALLOC(p, n+1);
		}
		
		public delegate CharPtr PyOS_ReadlineFunctionPointer_delegate(CharPtr str);
		public static PyOS_ReadlineFunctionPointer_delegate PyOS_ReadlineFunctionPointer;
		
		public static CharPtr PyOS_Readline(CharPtr prompt)
		{
			CharPtr rv;
			if (PyOS_ReadlineFunctionPointer == null) 
			{
				PyOS_ReadlineFunctionPointer = PyOS_StdioReadline;
			}
			
			{ 
				PyThreadState _save; 
				_save = PyEval_SaveThread();
				
				rv = PyOS_ReadlineFunctionPointer(prompt);
					
				PyEval_RestoreThread(_save); 
			}
			
			return rv;
		}
	}
}
