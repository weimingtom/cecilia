#define _DEBUG

//20170403
//#include "pgenheaders.h"
//#include "grammar.h"
//#include "node.h"
//#include "token.h"
//#include "parser.h"

using System;

namespace Cecilia
{
	public partial class Python
	{
//		static void fixdfa(grammar *, dfa *);
//		static void fixstate(grammar *, state *);
		
		public static void PyGrammar_AddAccelerators(grammar g)
		{
			dfaPtr d;
			int i;
		
		#if _DEBUG
			fprintf(stderr, "Adding parser accelerators ...\n");
		#endif
			d = new dfaPtr(g.g_dfa);
			for (i = g.g_ndfas; --i >= 0; d.inc())
			{
				fixdfa(g, d[0]);
			}
			g.g_accel = 1;
		#if _DEBUG
			fprintf(stderr, "Done.\n");
		#endif
		}
		
		public static void PyGrammar_RemoveAccelerators(grammar g)
		{
			dfaPtr d;
			int i;
			
			g.g_accel = 0;
			d = new dfaPtr(g.g_dfa);
			for (i = g.g_ndfas; --i >= 0; d.inc())
			{
				statePtr s;
				int j;
				s = new statePtr(d[0].d_state);
				for (j = 0; j < d[0].d_nstates; j++, s.inc())
				{
					if (null!=s[0].s_accel)
					{
						PyMem_DEL(ref s[0].s_accel);
					}
					s[0].s_accel = null;
				}
			}
		}
		
		private static void fixdfa(grammar g, dfa d)
		{
			statePtr s;
			int j;
		
			s = new statePtr(d.d_state);
			for (j = 0; j < d.d_nstates; j++, s.inc())
			{
				fixstate(g, s[0]);
			}
		}
		
		private static void fixstate(grammar g, state s)
		{
			arcPtr a;
			int k;
			intPtr accel;
			int nl = g.g_ll.ll_nlabels;
		
			s.s_accept = 0;
			accel = PyMem_NEW_int(nl);
			for (k = 0; k < nl; k++)
			{
				accel[k] = -1;
			}
			a = new arcPtr(s.s_arc);
			for (k = s.s_narcs; --k >= 0; a.inc())
			{
				int lbl = a[0].a_lbl;
				label l = g.g_ll.ll_label[lbl];
				int type = l.lb_type;
				if (a[0].a_arrow >= (1 << 7))
				{
					printf("XXX too many states!\n");
					continue;
				}
				if (ISNONTERMINAL(type)) 
				{
					dfa d1 = PyGrammar_FindDFA(g, type);
					int ibit;
					if (type - NT_OFFSET >= (1 << 7)) 
					{
						printf("XXX too high nonterminal number!\n");
						continue;
					}
					for (ibit = 0; ibit < g.g_ll.ll_nlabels; ibit++) 
					{
						if (testbit(d1.d_first, ibit)) 
						{
							if (accel[ibit] != -1)
							{
								printf("XXX ambiguity!\n");
							}
							accel[ibit] = a[0].a_arrow | (1 << 7) |
								((type - NT_OFFSET) << 8);
						}
					}
				}
				else if (lbl == EMPTY)
				{
					s.s_accept = 1;
				}
				else if (lbl >= 0 && lbl < nl)
				{
					accel[lbl] = a[0].a_arrow;
				}
			}
			while (nl > 0 && accel[nl-1] == -1)
			{
				nl--;
			}
			for (k = 0; k < nl && accel[k] == -1;)
			{
				k++;
			}
			if (k < nl) 
			{
				int i;
				s.s_accel = PyMem_NEW_int(nl-k);
				if (s.s_accel == null) 
				{
					fprintf(stderr, "no mem to add parser accelerators\n");
					exit(1);
				}
				s.s_lower = k;
				s.s_upper = nl;
				for (i = 0; k < nl; i++, k++)
				{
					s.s_accel[i] = accel[k];
				}
			}
			PyMem_DEL(ref accel);
		}
	}
}
