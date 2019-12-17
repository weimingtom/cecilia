#define _DEBUG

using System;

namespace Cecilia
{
	public partial class Python
	{
		//20170403
		//#include "python.h"
		//#include "pgenheaders.h"
		//#include "grammar.h"
		//#include "token.h"
		
		public static dfa PyGrammar_FindDFA(grammar g, int type)
		{
			dfa d;
			d = g.g_dfa[type - NT_OFFSET];
			assert(d.d_type == type);
			return d;
		}
		
		private static CharPtr PyGrammar_LabelRepr_buf = new char[100];
		public static CharPtr PyGrammar_LabelRepr(label lb)
		{
			//static char buf[100];
			
			if (lb.lb_type == ENDMARKER)
			{
				return "EMPTY";
			}
			else if (ISNONTERMINAL(lb.lb_type)) 
			{
				if (lb.lb_str == null) 
				{
					PyOS_snprintf(PyGrammar_LabelRepr_buf, 100/*sizeof(buf)*/, "NT%d", lb.lb_type);
					return new CharPtr(PyGrammar_LabelRepr_buf);
				}
				else
				{
					return new CharPtr(lb.lb_str);
				}
			}
			else 
			{
				if (lb.lb_str == null)
				{
					return _PyParser_TokenNames[lb.lb_type];
				}
				else 
				{
					PyOS_snprintf(PyGrammar_LabelRepr_buf, 100/*sizeof(buf)*/, "%.32s(%.32s)",
						_PyParser_TokenNames[lb.lb_type], lb.lb_str);
					return new CharPtr(PyGrammar_LabelRepr_buf);
				}
			}
		}
	}
}
