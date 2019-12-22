#define _DEBUG

using System;

namespace Cecilia
{
	public partial class Python
	{
		//20170403
		//#include "python.h"
		//#include "pgenheaders.h"
		//#include "token.h"
		//#include "grammar.h"
		//#include "node.h"
		//#include "parser.h"
		//#include "errcode.h"
		
		#if _DEBUG
		//extern int Py_DebugFlag;
		public static bool D() { return Py_DebugFlag != 0; }
		#else
		public static bool D() { return false; }
		#endif
		
		//static void s_reset(stack *);
		
		private static void s_reset(stack s)
		{
			s.s_top = new stackentryPtr(s.s_base, MAXSTACK);
		}
		
		private static bool s_empty(stack s) { return stackentryPtr.equals(s.s_top, new stackentryPtr(s.s_base, MAXSTACK)); }
		
		private static int s_push(stack s, dfa d, node parent)
		{
			stackentry top;
			if (stackentryPtr.equals(s.s_top, s.s_base))
			{
				fprintf(stderr, "s_push: parser stack overflow\n");
				return E_NOMEM;
			}
			s.s_top.dec(); top = s.s_top[0];
			top.s_dfa = d;
			top.s_parent = parent;
			top.s_state = 0;
			return 0;
		}
		
		#if _DEBUG
		private static void s_pop(stack s)
		{
			if (s_empty(s))
			{
				Py_FatalError("s_pop: parser stack underflow -- FATAL");
			}
			s.s_top.inc();
		}
		#else
		private static void s_pop(stack s) { s.s_top.inc(); }
		#endif
		
		public static parser_state PyParser_New(grammar g, int start)
		{
			parser_state ps;
			
			if (0==g.g_accel)
			{
				PyGrammar_AddAccelerators(g);
			}
			ps = PyMem_NEW_parser_state(1);
			if (ps == null)
			{
				return null;
			}
			ps.p_grammar = g;
			ps.p_generators = 0;
			ps.p_tree = PyNode_New(start);
			if (ps.p_tree == null) 
			{
				PyMem_DEL(ref ps);
				return null;
			}
			s_reset(ps.p_stack);
			s_push(ps.p_stack, PyGrammar_FindDFA(g, start), ps.p_tree);
			return ps;
		}
		
		public static void PyParser_Delete(parser_state ps)
		{
			PyNode_Free(ps.p_tree);
			PyMem_DEL(ref ps);
		}
		
		private static int shift(stack s, int type, CharPtr str, int newstate, int lineno)
		{
			int err;
			assert(!s_empty(s));
			err = PyNode_AddChild(s.s_top[0].s_parent, type, str, lineno);
			if (0!=err)
			{
				return err;
			}
			s.s_top[0].s_state = newstate;
			return 0;
		}
		
		private static int push(stack s, int type, dfa d, int newstate, int lineno)
		{
			int err;
			node n;
			n = s.s_top[0].s_parent;
			assert(!s_empty(s));
			err = PyNode_AddChild(n, type, (CharPtr)null, lineno);
			if (0!=err)
			{
				return err;
			}
			s.s_top[0].s_state = newstate;
			return s_push(s, d, CHILD(n, NCH(n)-1));
		}
		
		private static int classify(parser_state ps, int type, CharPtr str)
		{
			grammar g = ps.p_grammar;
			int n = g.g_ll.ll_nlabels;
			
			if (type == NAME) 
			{
				CharPtr s = new CharPtr(str);
				labelPtr l = new labelPtr(g.g_ll.ll_label);
				int i;
				for (i = n; i > 0; i--, l.inc())
				{
					if (l[0].lb_type == NAME && l[0].lb_str != null &&
					    l[0].lb_str[0] == s[0] &&
					    strcmp(l[0].lb_str, s) == 0)
					{
						if (0==ps.p_generators &&
						    s[0] == 'y' &&
						    strcmp(s, "yield") == 0)
						{
							break;
						}
						if (D()) printf("It's a keyword\n");
						return n - i;
					}
				}
			}
			
			{
				labelPtr l = new labelPtr(g.g_ll.ll_label);
				int i;
				for (i = n; i > 0; i--, l.inc())
				{
					if (l[0].lb_type == type && l[0].lb_str == null)
					{
						if (D()) printf("It's a token we know\n");
						return n - i;
					}
				}
			}
			
			if (D()) printf("Illegal token\n");
			return -1;
		}
		
		private static void future_hack(parser_state ps)
		{
			node n = ps.p_stack.s_top[0].s_parent;
			node ch;
			int i;
		
			if (strcmp(STR(CHILD(n, 0)), "from") != 0)
			{
				return;
			}
			ch = CHILD(n, 1);
			if (strcmp(STR(CHILD(ch, 0)), "__future__") != 0)
			{
				return;
			}
			for (i = 3; i < NCH(n); i += 2) 
			{
				ch = CHILD(n, i);
				if (NCH(ch) >= 1 && TYPE(CHILD(ch, 0)) == NAME &&
				    strcmp(STR(CHILD(ch, 0)), "generators") == 0) 
				{
					ps.p_generators = 1;
					break;
				}
			}
		}
		
		public static int PyParser_AddToken(parser_state ps, int type, CharPtr str,
			          int lineno, ref int expected_ret)
		{
			int ilabel;
			int err;
			
			if (D()) printf("Token %s/'%s' ... ", _PyParser_TokenNames[type], str);
			
			ilabel = classify(ps, type, str);
			if (ilabel < 0)
			{
				return E_SYNTAX;
			}
		
			for (;;) 
			{
				dfa d = ps.p_stack.s_top[0].s_dfa;
				state s = d.d_state[ps.p_stack.s_top[0].s_state];
				
				if (D()) printf(" DFA '%s', state %d:",
				                d.d_name, ps.p_stack.s_top[0].s_state);
				
				if (s.s_lower <= ilabel && ilabel < s.s_upper) 
				{
					int x = s.s_accel[ilabel - s.s_lower];
					if (x != -1) 
					{
						if (0!=(x & (1<<7)))
						{
							int nt = (x >> 8) + NT_OFFSET;
							int arrow = x & ((1<<7)-1);
							dfa d1 = PyGrammar_FindDFA(
								ps.p_grammar, nt);
							if ((err = push(ps.p_stack, nt, d1,
								arrow, lineno)) > 0) 
							{
								if (D()) printf(" MemError: push\n");
								return err;
							}
							if (D()) printf(" Push ...\n");
							continue;
						}
						if ((err = shift(ps.p_stack, type, str,
								x, lineno)) > 0) 
						{
							if (D()) printf(" MemError: shift.\n");
							return err;
						}
						if (D()) printf(" Shift.\n");
						while (true) 
						{
							s = d.d_state[ps.p_stack.s_top[0].s_state];
							if (!(0!=s.s_accept && s.s_narcs == 1))
							{
								break;
							}
								
							if (D()) printf("  DFA '%s', state %d: " +
								 "Direct pop.\n",
								 d.d_name,
								 ps.p_stack.s_top[0].s_state);
							if (d.d_name[0] == 'i' &&
							    strcmp(d.d_name,
								   "import_stmt") == 0)
							{
								future_hack(ps);
							}
							s_pop(ps.p_stack);
							if (s_empty(ps.p_stack)) 
							{
								if (D()) printf("  ACCEPT.\n");
								return E_DONE;
							}
							d = ps.p_stack.s_top[0].s_dfa;
						}
						return E_OK;
					}
				}
				
				if (0!=s.s_accept) 
				{
					if (d.d_name[0] == 'i' &&
					    strcmp(d.d_name, "import_stmt") == 0)
					{
						future_hack(ps);
					}
					s_pop(ps.p_stack);
					if (D()) printf(" Pop ...\n");
					if (s_empty(ps.p_stack)) 
					{
						if (D()) printf(" Error: bottom of stack.\n");
						return E_SYNTAX;
					}
					continue;
				}
				
				if (D()) printf(" Error.\n");
				if (true) //if (0!=expected_ret)
				{
					if (s.s_lower == s.s_upper - 1) 
					{
						expected_ret = ps.p_grammar.
						    g_ll.ll_label[s.s_lower].lb_type;
					}
					else 
					{
						expected_ret = -1;
					}
				}
				return E_SYNTAX;
			}
		}
		
		
		#if _DEBUG
		public static void dumptree(grammar g, node n)
		{
			int i;
			
			if (n == null)
			{
				printf("NIL");
			}
			else 
			{
				label l = new label();
				l.lb_type = TYPE(n);
				l.lb_str = STR(n);
				printf("%s", PyGrammar_LabelRepr(l));
				if (ISNONTERMINAL(TYPE(n))) 
				{
					printf("(");
					for (i = 0; i < NCH(n); i++) 
					{
						if (i > 0)
						{
							printf(",");
						}
						dumptree(g, CHILD(n, i));
					}
					printf(")");
				}
			}
		}
		
		public static void showtree(grammar g, node n)
		{
			int i;
			
			if (n == null)
			{
				return;
			}
			if (ISNONTERMINAL(TYPE(n))) 
			{
				for (i = 0; i < NCH(n); i++)
				{
					showtree(g, CHILD(n, i));
				}
			}
			else if (ISTERMINAL(TYPE(n))) 
			{
				printf("%s", _PyParser_TokenNames[TYPE(n)]);
				if (TYPE(n) == NUMBER || TYPE(n) == NAME)
				{
					printf("(%s)", STR(n));
				}
				printf(" ");
			}
			else
			{
				printf("? ");
			}
		}
		
		public static void printtree(parser_state ps)
		{
			if (0!=Py_DebugFlag) 
			{
				printf("Parse tree:\n");
				dumptree(ps.p_grammar, ps.p_tree);
				printf("\n");
				printf("Tokens:\n");
				showtree(ps.p_grammar, ps.p_tree);
				printf("\n");
			}
			printf("Listing:\n");
			PyNode_ListTree(ps.p_tree);
			printf("\n");
		}
		#endif 
	}
}
