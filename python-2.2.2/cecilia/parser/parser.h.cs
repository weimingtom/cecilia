#define _DEBUG

using System;

namespace Cecilia
{
	public partial class Python
	{
		//20170403
		//#pragma once
		
		public const int MAXSTACK = 500;
		
		public class stackentry {
			public int s_state;
			public dfa	s_dfa;
			public node s_parent;
		};
		public class stackentryPtr
		{
			public stackentry this[int offset]
			{
				get { return null; }
				set { }
			}		
			public void inc() { }
			public void dec() { }
			public stackentryPtr(stackentryPtr ptr) { }
			public stackentryPtr(stackentry[] arr, int index) { }
			public static bool equals(stackentryPtr a, stackentry[] b) { return false; }
			public static bool equals(stackentryPtr a, stackentryPtr b) { return false; }
		}
		
		public class stack {
			public stackentryPtr s_top;
			public stackentry[] s_base = new stackentry[MAXSTACK];
			
			public stack()
			{
				for (int i = 0; i < MAXSTACK; ++i)
				{
					s_base[i] = new stackentry();
				}
			}
		};
		
		public class parser_state {
			public stack p_stack = new stack();
			public grammar p_grammar;	
			public node p_tree;	
			public int	p_generators;
		};
		
		//parser_state *PyParser_New(grammar *g, int start);
		//void PyParser_Delete(parser_state *ps);
		//int PyParser_AddToken(parser_state *ps, int type, char *str, int lineno, int *expected_ret);
		//void PyGrammar_AddAccelerators(grammar *g);
	}
}
