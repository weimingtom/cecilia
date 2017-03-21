namespace cecilia 
{
	public partical class cecilia 
	{
		/* Parser interface */

		public const int MAXSTACK = 500;

		public class stackentry {
			public int s_state;	/* State in current DFA */
			public dfa s_dfa; /* Current DFA */
			public class _node s_parent; /* Where to add next node */
		};

		public class stack {
			public stackentry s_top;		/* Top entry */
			public stackentry s_base[MAXSTACK];/* Array of stack entries */
			/* NB The stack grows down */
		};

		public class parser_state {
			public stack p_stack; /* Stack of parser states */
			public grammar p_grammar; /* Grammar to use */
			public node p_tree;	/* Top of parse tree */
			public int p_generators; /* 1 if yield is a keyword */
		};

//		parser_state *PyParser_New(grammar *g, int start);
//		void PyParser_Delete(parser_state *ps);
//		int PyParser_AddToken(parser_state *ps, int type, char *str, int lineno,
//							  int *expected_ret);
//		void PyGrammar_AddAccelerators(grammar *g);

	}
}
