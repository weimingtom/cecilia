
/* Grammar subroutines needed by parser */

namespace cecilia 
{
	public partical class cecilia 
	{

		/* Return the DFA for the given type */

		public static dfa
		PyGrammar_FindDFA(grammar g, int type)
		{
			dfa d;
		#if 1
			/* Massive speed-up */
			d = &g->g_dfa[type - NT_OFFSET];
			assert(d->d_type == type);
			return d;
		#else
			/* Old, slow version */
			register int i;
			
			for (i = g->g_ndfas, d = g->g_dfa; --i >= 0; d++) {
				if (d->d_type == type)
					return d;
			}
			assert(0);
			/* NOTREACHED */
		#endif
		}

		public static CharPtr
		PyGrammar_LabelRepr(label lb)
		{
			static char buf[100];
			
			if (lb->lb_type == ENDMARKER)
				return "EMPTY";
			else if (ISNONTERMINAL(lb->lb_type)) {
				if (lb->lb_str == NULL) {
					PyOS_snprintf(buf, sizeof(buf), "NT%d", lb->lb_type);
					return buf;
				}
				else
					return lb->lb_str;
			}
			else {
				if (lb->lb_str == NULL)
					return _PyParser_TokenNames[lb->lb_type];
				else {
					PyOS_snprintf(buf, sizeof(buf), "%.32s(%.32s)",
						_PyParser_TokenNames[lb->lb_type], lb->lb_str);
					return buf;
				}
			}
		}
		
	}
}