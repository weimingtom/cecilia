namespace cecilia 
{
	public partical class cecilia 
	{

		/* Tokenizer interface */
		
		public const MAXINDENT = 100; /* Max indentation level */
		
		/* Tokenizer state */
		public class tok_state {
			/* Input state; buf <= cur <= inp <= end */
			/* NB an entire line is held in the buffer */
			public CharPtr buf;	/* Input buffer, or NULL; malloc'ed if fp != NULL */
			public CharPtr cur;	/* Next character in buffer */
			public CharPtr inp;	/* End of data in buffer */
			public CharPtr end;	/* End of input buffer if buf != NULL */
			public CharPtr start;	/* Start of current token if not NULL */
			public int done;	/* E_OK normally, E_EOF at EOF, otherwise error code */
			/* NB If done != E_OK, cur must be == inp!!! */
			public FILE fp;	/* Rest of input; NULL if tokenizing a string */
			public int tabsize;	/* Tab spacing */
			public int indent;	/* Current indentation index */
			public int indstack[MAXINDENT];	/* Stack of indents */
			public int atbol;	/* Nonzero if at begin of new line */
			public int pendin;	/* Pending indents (if > 0) or dedents (if < 0) */
			public CharPtr prompt, nextprompt;	/* For interactive prompting */
			public int lineno;	/* Current line number */
			public int level;	/* () [] {} Parentheses nesting level */
					/* Used to allow free continuations inside them */
			/* Stuff for checking on different tab sizes */
			public CharPtr filename;	/* For error messages */
			public int altwarning;	/* Issue warning if alternate tabs don't match */
			public int alterror;	/* Issue error if alternate tabs don't match */
			public int alttabsize;	/* Alternate tab spacing */
			public int altindstack[MAXINDENT];	/* Stack of alternate indents */
		};

//extern struct tok_state *PyTokenizer_FromString(char *);
//extern struct tok_state *PyTokenizer_FromFile(FILE *, char *, char *);
//extern void PyTokenizer_Free(struct tok_state *);
//extern int PyTokenizer_Get(struct tok_state *, char **, char **);

	}
}