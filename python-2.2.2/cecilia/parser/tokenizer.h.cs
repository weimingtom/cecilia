#define _DEBUG

using System;

namespace Cecilia
{
	using size_t = System.UInt32;
	
	public partial class Python
	{
		//20170403
		//#pragma once
		
		//#include "token.h"
		
		public const int MAXINDENT = 100;
		
		public class tok_state {
			public CharPtr buf;
			public CharPtr cur;
			public CharPtr inp;
			public CharPtr end;
			public CharPtr start;
			public int done;
			public FILEPtr fp;
			public int tabsize;
			public int indent;
			public int[] indstack = new int[MAXINDENT];
			public int atbol;
			public int pendin;
			public CharPtr prompt, nextprompt;
			public int lineno;
			public int level;
			public CharPtr filename;
			public int altwarning;
			public int alterror;
			public int alttabsize;
			public int[] altindstack = new int[MAXINDENT];
		};
		
//		extern struct tok_state *PyTokenizer_FromString(char *);
//		extern struct tok_state *PyTokenizer_FromFile(FILE *, char *, char *);
//		extern void PyTokenizer_Free(struct tok_state *);
//		extern int PyTokenizer_Get(struct tok_state *, char **, char **);
	}
}
