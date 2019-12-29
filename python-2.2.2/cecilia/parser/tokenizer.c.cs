#define _DEBUG

using System;

namespace Cecilia
{
	using size_t = System.UInt32;
	
	public partial class Python
	{
		//20170403
		//#include "pgenheaders.h"
		//#include <ctype.h>
		//#include "tokenizer.h"
		//#include "errcode.h"
		
		//extern char *PyOS_Readline(char *);
		
		private const int TABSIZE = 8;
		private static int Py_CHARMASK(char c) { return ((c) & 0xff); }
		
		//static struct tok_state *tok_new();
		//static int tok_nextc(struct tok_state *tok);
		//static void tok_backup(struct tok_state *tok, int c);
		
		public static string[] _PyParser_TokenNames = {
			"ENDMARKER",
			"NAME",
			"NUMBER",
			"STRING",
			"NEWLINE",
			"INDENT",
			"DEDENT",
			"LPAR",
			"RPAR",
			"LSQB",
			"RSQB",
			"COLON",
			"COMMA",
			"SEMI",
			"PLUS",
			"MINUS",
			"STAR",
			"SLASH",
			"VBAR",
			"AMPER",
			"LESS",
			"GREATER",
			"EQUAL",
			"DOT",
			"PERCENT",
			"BACKQUOTE",
			"LBRACE",
			"RBRACE",
			"EQEQUAL",
			"NOTEQUAL",
			"LESSEQUAL",
			"GREATEREQUAL",
			"TILDE",
			"CIRCUMFLEX",
			"LEFTSHIFT",
			"RIGHTSHIFT",
			"DOUBLESTAR",
			"PLUSEQUAL",
			"MINEQUAL",
			"STAREQUAL",
			"SLASHEQUAL",
			"PERCENTEQUAL",
			"AMPEREQUAL",
			"VBAREQUAL",
			"CIRCUMFLEXEQUAL",
			"LEFTSHIFTEQUAL",
			"RIGHTSHIFTEQUAL",
			"DOUBLESTAREQUAL",
			"DOUBLESLASH",
			"DOUBLESLASHEQUAL",
			"OP",
			"<ERRORTOKEN>",
			"<N_TOKENS>"
		};
		
		private static tok_state tok_new()
		{
			tok_state tok = PyMem_NEW_tok_state(1);
			if (tok == null)
			{
				return null;
			}
			tok.buf = tok.cur = tok.end = tok.inp = tok.start = null;
			tok.done = E_OK;
			tok.fp = null;
			tok.tabsize = TABSIZE;
			tok.indent = 0;
			tok.indstack[0] = 0;
			tok.atbol = 1;
			tok.pendin = 0;
			tok.prompt = tok.nextprompt = null;
			tok.lineno = 0;
			tok.level = 0;
			tok.filename = null;
			tok.altwarning = 0;
			tok.alterror = 0;
			tok.alttabsize = 1;
			tok.altindstack[0] = 0;
			return tok;
		}
		
		public static tok_state PyTokenizer_FromString(CharPtr str)
		{
			tok_state tok = tok_new();
			if (tok == null)
			{
				return null;
			}
			tok.buf = new CharPtr(str);
			tok.cur = new CharPtr(str);
			tok.end = new CharPtr(str);
			tok.inp = new CharPtr(str);
			return tok;
		}
		
		public static tok_state PyTokenizer_FromFile(FILEPtr fp, CharPtr ps1, CharPtr ps2)
		{
			tok_state tok = tok_new();
			if (tok == null)
			{
				return null;
			}
			if ((tok.buf = PyMem_NEW_char2(BUFSIZ)) == null) 
			{
				PyMem_DEL(ref tok);
				return null;
			}
			tok.cur = new CharPtr(tok.buf);
			tok.inp = new CharPtr(tok.buf);
			tok.end = new CharPtr(tok.buf, BUFSIZ);
			tok.fp = new FILEPtr(fp);
			tok.prompt = new CharPtr(ps1);
			tok.nextprompt = new CharPtr(ps2);
			return tok;
		}
		
		public static void PyTokenizer_Free(tok_state tok)
		{
			if (tok.fp != null && tok.buf != null)
			{
				PyMem_DEL(tok.buf);
			}
			PyMem_DEL(ref tok);
		}
		
		private static int tok_nextc(tok_state tok)
		{
			for (;;) 
			{
				if (tok.cur != tok.inp) 
				{
					return Py_CHARMASK(tok.cur[0]); tok.cur.inc();
				}
				if (tok.done != E_OK)
				{
					return EOF;
				}
				if (tok.fp == null) 
				{
					CharPtr end = strchr(tok.inp, '\n');
					if (end != null)
					{
						end.inc();
					}
					else 
					{
						end = strchr(tok.inp, '\0');
						if (end == tok.inp) 
						{
							tok.done = E_EOF;
							return EOF;
						}
					}
					if (tok.start == null)
					{
						tok.buf = tok.cur;
					}
					tok.lineno++;
					tok.inp = end;
					return Py_CHARMASK(tok.cur[0]); tok.cur.inc();
				}
				if (tok.prompt != null) 
				{
					CharPtr new_ = PyOS_Readline(tok.prompt);
					if (tok.nextprompt != null)
					{
						tok.prompt = tok.nextprompt;
					}
					if (new_ == null)
					{
						tok.done = E_INTR;
					}
					else if (new_[0] == '\0')
					{
						PyMem_FREE(ref new_);
						tok.done = E_EOF;
					}
					else if (tok.start != null) 
					{
						size_t start = (size_t)(tok.start - tok.buf);
						size_t oldlen = (size_t)(tok.cur - tok.buf);
						size_t newlen = oldlen + strlen(new_);
						CharPtr buf = new CharPtr(tok.buf);
						PyMem_RESIZE_char(ref buf, (int)(newlen+1));
						tok.lineno++;
						if (buf == null) 
						{
							PyMem_DEL(tok.buf);
							tok.buf = null;
							PyMem_FREE(ref new_);
							tok.done = E_NOMEM;
							return EOF;
						}
						tok.buf = buf;
						tok.cur = tok.buf + oldlen;
						strcpy(tok.buf + oldlen, new_);
						PyMem_FREE(ref new_);
						tok.inp = tok.buf + newlen;
						tok.end = tok.inp + 1;
						tok.start = tok.buf + start;
					}
					else 
					{
						tok.lineno++;
						if (tok.buf != null)
						{
							PyMem_DEL(tok.buf);
						}
						tok.buf = new CharPtr(new_);
						tok.cur = new CharPtr(tok.buf);
						tok.inp = strchr(tok.buf, '\0');
						tok.end = tok.inp + 1;
					}
				}
				else 
				{
					int done = 0;
					int cur = 0;
					CharPtr pt;
					if (tok.start == null) 
					{
						if (tok.buf == null) 
						{
							tok.buf = PyMem_NEW_char2(BUFSIZ);
							if (tok.buf == null) 
							{
								tok.done = E_NOMEM;
								return EOF;
							}
							tok.end = tok.buf + BUFSIZ;
						}
						if (fgets(tok.buf, (int)(tok.end - tok.buf),
							  tok.fp) == null) 
						{
							tok.done = E_EOF;
							done = 1;
						}
						else 
						{
							tok.done = E_OK;
							tok.inp = strchr(tok.buf, '\0');
							done = ((tok.inp[-1] == '\n')?1:0);
						}
					}
					else 
					{
						cur = tok.cur - tok.buf;
						if (0!=feof(tok.fp)) 
						{
							tok.done = E_EOF;
							done = 1;
						}
						else
						{
							tok.done = E_OK;
						}
					}
					tok.lineno++;
					while (0==done) 
					{
						int curstart = tok.start == null ? -1 :
							       tok.start - tok.buf;
						int curvalid = tok.inp - tok.buf;
						int newsize = curvalid + BUFSIZ;
						CharPtr newbuf = new CharPtr(tok.buf);
						PyMem_RESIZE_char(ref newbuf, newsize);
						if (newbuf == null) 
						{
							tok.done = E_NOMEM;
							tok.cur = tok.inp;
							return EOF;
						}
						tok.buf = newbuf;
						tok.inp = tok.buf + curvalid;
						tok.end = tok.buf + newsize;
						tok.start = curstart < 0 ? null :
							     tok.buf + curstart;
						if (fgets(tok.inp,
							       (int)(tok.end - tok.inp),
							       tok.fp) == null) 
						{
							strcpy(tok.inp, "\n");
						}
						tok.inp = strchr(tok.inp, '\0');
						done = ((tok.inp[-1] == '\n')?1:0);
					}
					tok.cur = tok.buf + cur;
		
					pt = tok.inp - 2;
					if (pt >= tok.buf && pt[0] == '\r')
					{
						pt[0] = '\n'; pt.inc();
						pt[0] = '\0';
						tok.inp = new CharPtr(pt);
					}
				}
				if (tok.done != E_OK) 
				{
					if (tok.prompt != null)
					{
						PySys_WriteStderr("\n");
					}
					tok.cur = tok.inp;
					return EOF;
				}
			}
		}
		
		private static void tok_backup(tok_state tok, int c)
		{
			if (c != EOF) 
			{
				tok.cur.dec();
				if (tok.cur < tok.buf)
				{
					Py_FatalError("tok_backup: begin of buffer");
				}
				if (tok.cur[0] != c)
				{
					tok.cur[0] = (char)c;
				}
			}
		}
		
		public static int PyToken_OneChar(int c)
		{
			switch (c) {
			case '(':	
				return LPAR;
		
			case ')':	
				return RPAR;
		
			case '[':	
				return LSQB;
		
			case ']':	
				return RSQB;
		
			case ':':	
				return COLON;
			
			case ',':	
				return COMMA;
			
			case ';':	
				return SEMI;
			
			case '+':	
				return PLUS;
			
			case '-':	
				return MINUS;
			
			case '*':	
				return STAR;
			
			case '/':	
				return SLASH;
			
			case '|':	
				return VBAR;
			
			case '&':	
				return AMPER;
			
			case '<':	
				return LESS;
			
			case '>':	
				return GREATER;
			
			case '=':	
				return EQUAL;
			
			case '.':	
				return DOT;
			
			case '%':	
				return PERCENT;
			
			case '`':	
				return BACKQUOTE;
			
			case '{':	
				return LBRACE;
			
			case '}':	
				return RBRACE;
			
			case '^':	
				return CIRCUMFLEX;
			
			case '~':	
				return TILDE;
			
			default:	
				return OP;
			}
		}
		
		
		public static int PyToken_TwoChars(int c1, int c2)
		{
			switch (c1) 
			{
			case '=':
				switch (c2) 
				{
				case '=':	
					return EQEQUAL;
				}
				break;
		
			case '!':
				switch (c2) 
				{
				case '=':	
					return NOTEQUAL;
				}
				break;
		
			case '<':
				switch (c2) 
				{
				case '>':	
					return NOTEQUAL;
				
				case '=':	
					return LESSEQUAL;
				
				case '<':	
					return LEFTSHIFT;
				}
				break;
		
			case '>':
				switch (c2) 
				{
				case '=':	
					return GREATEREQUAL;
				
				case '>':	
					return RIGHTSHIFT;
				}
				break;
			
			case '+':
				switch (c2) 
				{
				case '=':	
					return PLUSEQUAL;
				}
				break;
		
			case '-':
				switch (c2) 
				{
				case '=':	
					return MINEQUAL;
				}
				break;
		
			case '*':
				switch (c2) 
				{
				case '*':	
					return DOUBLESTAR;
				
				case '=':	
					return STAREQUAL;
				}
				break;
		
			case '/':
				switch (c2) 
				{
				case '/':	
					return DOUBLESLASH;
				
				case '=':	
					return SLASHEQUAL;
				}
				break;
		
			case '|':
				switch (c2) 
				{
				case '=':	
					return VBAREQUAL;
				}
				break;
		
			case '%':
				switch (c2) 
				{
				case '=':	
					return PERCENTEQUAL;
				}
				break;
		
			case '&':
				switch (c2) 
				{
				case '=':	
					return AMPEREQUAL;
				}
				break;
		
			case '^':
				switch (c2) 
				{
				case '=':	
					return CIRCUMFLEXEQUAL;
				}
				break;
			}
			return OP;
		}
		
		public static int PyToken_ThreeChars(int c1, int c2, int c3)
		{
			switch (c1) 
			{
			case '<':
				switch (c2) 
				{
				case '<':
					switch (c3) 
					{
					case '=':
						return LEFTSHIFTEQUAL;
					}
					break;
				}
				break;
		
			case '>':
				switch (c2) 
				{
				case '>':
					switch (c3) 
					{
					case '=':
						return RIGHTSHIFTEQUAL;
					}
					break;
				}
				break;
		
			case '*':
				switch (c2) 
				{
				case '*':
					switch (c3) 
					{
					case '=':
						return DOUBLESTAREQUAL;
					}
					break;
				}
				break;
		
			case '/':
				switch (c2) 
				{
				case '/':
					switch (c3) 
					{
					case '=':
						return DOUBLESLASHEQUAL;
					}
					break;
				}
				break;
			}
			return OP;
		}
		
		private static int indenterror(tok_state tok)
		{
			if (0!=tok.alterror) 
			{
				tok.done = E_TABSPACE;
				tok.cur = tok.inp;
				return 1;
			}
			if (0!=tok.altwarning) 
			{
				PySys_WriteStderr("%s: inconsistent use of tabs and spaces " +
		                                  "in indentation\n", tok.filename);
				tok.altwarning = 0;
			}
			return 0;
		}
		
		private static string[] PyTokenizer_Get_tabforms = {
			"tab-width:",	
			":tabstop=",		
			":ts=",			
			"set tabsize=",
		};		
		public static int PyTokenizer_Get(tok_state tok, ref CharPtr p_start,
				ref CharPtr p_end)
		{
			int c;
			int blankline;
		
			p_start = p_end = null;
		nextline:
			tok.start = null;
			blankline = 0;
		
			if (0!=tok.atbol) 
			{
				int col = 0;
				int altcol = 0;
				tok.atbol = 0;
				for (;;) 
				{
					c = tok_nextc(tok);
					if (c == ' ')
					{
						col++; altcol++;
					}
					else if (c == '\t') 
					{
						col = (col/tok.tabsize + 1) * tok.tabsize;
						altcol = (altcol/tok.alttabsize + 1)
							* tok.alttabsize;
					}
					else if (c == '\x0C')//'\014')
					{
						col = altcol = 0;
					}
					else
					{
						break;
					}
				}
				tok_backup(tok, c);
				if (c == '#' || c == '\n') 
				{
					if (col == 0 && c == '\n' && tok.prompt != null)
					{
						blankline = 0;
					}
					else
					{
						blankline = 1;
					}
				}
				if (0==blankline && tok.level == 0) 
				{
					if (col == tok.indstack[tok.indent]) 
					{
						if (altcol != tok.altindstack[tok.indent]) 
						{
							if (0!=indenterror(tok))
							{
								return ERRORTOKEN;
							}
						}
					}
					else if (col > tok.indstack[tok.indent]) 
					{
						if (tok.indent+1 >= MAXINDENT) 
						{
							tok.done = E_TOODEEP;
							tok.cur = tok.inp;
							return ERRORTOKEN;
						}
						if (altcol <= tok.altindstack[tok.indent]) 
						{
							if (0!=indenterror(tok))
							{
								return ERRORTOKEN;
							}
						}
						tok.pendin++;
						tok.indstack[++tok.indent] = col;
						tok.altindstack[tok.indent] = altcol;
					}
					else 
					{
						while (tok.indent > 0 &&
							col < tok.indstack[tok.indent]) 
						{
							tok.pendin--;
							tok.indent--;
						}
						if (col != tok.indstack[tok.indent]) 
						{
							tok.done = E_DEDENT;
							tok.cur = tok.inp;
							return ERRORTOKEN;
						}
						if (altcol != tok.altindstack[tok.indent]) 
						{
							if (0!=indenterror(tok))
							{
								return ERRORTOKEN;
							}
						}
					}
				}
			}
			
			tok.start = tok.cur;
			
			if (tok.pendin != 0) 
			{
				if (tok.pendin < 0) 
				{
					tok.pendin++;
					return DEDENT;
				}
				else 
				{
					tok.pendin--;
					return INDENT;
				}
			}
			
		again:
			tok.start = null;
			do 
			{
				c = tok_nextc(tok);
			} while (c == ' ' || c == '\t' || c == '\x0C'); //c == '\014');
			
			tok.start = tok.cur - 1;
			
			if (c == '#') 
			{
				CharPtr cbuf = new CharPtr(new char[80]);
				CharPtr tp; int cp;
				tp = new CharPtr(cbuf);
				do 
				{
					c = tok_nextc(tok); tp[0] = (char)c; tp.inc();
				} while (c != EOF && c != '\n' &&
					 tp - cbuf + 1 < 80/*sizeof(cbuf)*/);
				tp[0] = '\0';
				for (cp = 0;
				     cp < PyTokenizer_Get_tabforms.Length;
				     cp++) 
				{
					if (null != (tp = strstr(cbuf, PyTokenizer_Get_tabforms[cp])))
					{
						int newsize = atoi(tp + strlen(PyTokenizer_Get_tabforms[cp]));
		
						if (newsize >= 1 && newsize <= 40) 
						{
							tok.tabsize = newsize;
							if (0!=Py_VerboseFlag)
							{
							    PySys_WriteStderr(
								"Tab size set to %d\n",
								newsize);
							}
						}
					}
				}
				while (c != EOF && c != '\n')
				{
					c = tok_nextc(tok);
				}
			}
			
			if (c == EOF) 
			{
				return tok.done == E_EOF ? ENDMARKER : ERRORTOKEN;
			}
			
		
			if (isalpha(c) || c == '_') 
			{
				switch (c) 
				{
				case 'r':
				case 'R':
					c = tok_nextc(tok);
					if (c == '"' || c == '\'')
					{
						goto letter_quote;
					}
					break;
				
				case 'u':
				case 'U':
					c = tok_nextc(tok);
					if (c == 'r' || c == 'R')
					{
						c = tok_nextc(tok);
					}
					if (c == '"' || c == '\'')
					{
						goto letter_quote;
					}
					break;
				}
				while (isalnum(c) || c == '_') 
				{
					c = tok_nextc(tok);
				}
				tok_backup(tok, c);
				p_start = new CharPtr(tok.start);
				p_end = new CharPtr(tok.cur);
				return NAME;
			}
			
			if (c == '\n') 
			{
				tok.atbol = 1;
				if (0!=blankline || tok.level > 0)
				{
					goto nextline;
				}
				p_start = new CharPtr(tok.start);
				p_end = new CharPtr(tok.cur - 1);
				return NEWLINE;
			}
				
			if (c == '.') 
			{
				c = tok_nextc(tok);
				if (isdigit(c)) 
				{
					do 
					{
						c = tok_nextc(tok);
					} while (isdigit(c));
					if (c == 'e' || c == 'E') 
					{
						c = tok_nextc(tok);
						if (c == '+' || c == '-')
						{
							c = tok_nextc(tok);
						}
						if (!isdigit(c)) 
						{
							tok.done = E_TOKEN;
							tok_backup(tok, c);
							return ERRORTOKEN;
						}
						do 
						{
							c = tok_nextc(tok);
						} while (isdigit(c));
					}
					if (c == 'j' || c == 'J')
					{
						c = tok_nextc(tok);
					}					
					//goto fraction;
					tok_backup(tok, c);
					p_start = new CharPtr(tok.start);
					p_end = new CharPtr(tok.cur);
					return NUMBER;					
				}
				else 
				{
					tok_backup(tok, c);
					p_start = new CharPtr(tok.start);
					p_end = new CharPtr(tok.cur);
					return DOT;
				}
			}
			
			if (isdigit(c)) 
			{
				if (c == '0') 
				{
					c = tok_nextc(tok);
					if (c == '.')
					{
						do 
						{
							c = tok_nextc(tok);
						} while (isdigit(c));
						if (c == 'e' || c == 'E') 
						{
							c = tok_nextc(tok);
							if (c == '+' || c == '-')
							{
								c = tok_nextc(tok);
							}
							if (!isdigit(c)) 
							{
								tok.done = E_TOKEN;
								tok_backup(tok, c);
								return ERRORTOKEN;
							}
							do 
							{
								c = tok_nextc(tok);
							} while (isdigit(c));
						}
						if (c == 'j' || c == 'J')
						{
							c = tok_nextc(tok);
						}							
						goto fraction;
					}
					if (c == 'j' || c == 'J')
					{
						c = tok_nextc(tok);
						goto imaginary;
					}
					if (c == 'x' || c == 'X') 
					{
						do 
						{
							c = tok_nextc(tok);
						} while (isxdigit(c));
					}
					else 
					{
						int found_decimal = 0;
						while ('0' <= c && c < '8') 
						{
							c = tok_nextc(tok);
						}
						if (isdigit(c)) 
						{
							found_decimal = 1;
							do 
							{
								c = tok_nextc(tok);
							} while (isdigit(c));
						}
						if (c == '.')
						{
							do 
							{
								c = tok_nextc(tok);
							} while (isdigit(c));
							if (c == 'e' || c == 'E') 
							{
								c = tok_nextc(tok);
								if (c == '+' || c == '-')
								{
									c = tok_nextc(tok);
								}
								if (!isdigit(c)) 
								{
									tok.done = E_TOKEN;
									tok_backup(tok, c);
									return ERRORTOKEN;
								}
								do 
								{
									c = tok_nextc(tok);
								} while (isdigit(c));
							}
							if (c == 'j' || c == 'J')
							{
								c = tok_nextc(tok);
							}								
							goto fraction;
						}
						else if (c == 'e' || c == 'E')
						{
							c = tok_nextc(tok);
							if (c == '+' || c == '-')
							{
								c = tok_nextc(tok);
							}
							if (!isdigit(c)) 
							{
								tok.done = E_TOKEN;
								tok_backup(tok, c);
								return ERRORTOKEN;
							}
							do 
							{
								c = tok_nextc(tok);
							} while (isdigit(c));
							if (c == 'j' || c == 'J')
							{
								c = tok_nextc(tok);
							}							
							goto exponent;
						}
		
						else if (c == 'j' || c == 'J')
						{
							c = tok_nextc(tok);
							goto imaginary;
						}
		
						else if (0!=found_decimal) 
						{
							tok.done = E_TOKEN;
							tok_backup(tok, c);
							return ERRORTOKEN;
						}
					}
					if (c == 'l' || c == 'L')
					{
						c = tok_nextc(tok);
					}
				}
				else 
				{
					do 
					{
						c = tok_nextc(tok);
					} while (isdigit(c));
					if (c == 'l' || c == 'L')
					{
						c = tok_nextc(tok);
					}
					else 
					{
						if (c == '.') 
						{
						//fraction:
							do 
							{
								c = tok_nextc(tok);
							} while (isdigit(c));
						}
						if (c == 'e' || c == 'E') 
						{
							//exponent:
							c = tok_nextc(tok);
							if (c == '+' || c == '-')
							{
								c = tok_nextc(tok);
							}
							if (!isdigit(c)) 
							{
								tok.done = E_TOKEN;
								tok_backup(tok, c);
								return ERRORTOKEN;
							}
							do 
							{
								c = tok_nextc(tok);
							} while (isdigit(c));
						}
						if (c == 'j' || c == 'J')
						{
							//imaginary:
							c = tok_nextc(tok);
						}
					}
				}
				fraction:
				exponent:
				imaginary:
				tok_backup(tok, c);
				p_start = new CharPtr(tok.start);
				p_end = new CharPtr(tok.cur);
				return NUMBER;
			}

		letter_quote:
			if (c == '\'' || c == '"') 
			{
				int quote2 = tok.cur - tok.start + 1;
				int quote = c;
				int triple = 0;
				int tripcount = 0;
				for (;;) 
				{
					c = tok_nextc(tok);
					if (c == '\n') 
					{
						if (0==triple) 
						{
							tok.done = E_TOKEN;
							tok_backup(tok, c);
							return ERRORTOKEN;
						}
						tripcount = 0;
					}
					else if (c == EOF) 
					{
						tok.done = E_TOKEN;
						tok.cur = tok.inp;
						return ERRORTOKEN;
					}
					else if (c == quote) 
					{
						tripcount++;
						if (tok.cur - tok.start == quote2) 
						{
							c = tok_nextc(tok);
							if (c == quote) 
							{
								triple = 1;
								tripcount = 0;
								continue;
							}
							tok_backup(tok, c);
						}
						if (0==triple || tripcount == 3)
						{
							break;
						}
					}
					else if (c == '\\') 
					{
						tripcount = 0;
						c = tok_nextc(tok);
						if (c == EOF) 
						{
							tok.done = E_TOKEN;
							tok.cur = tok.inp;
							return ERRORTOKEN;
						}
					}
					else
					{
						tripcount = 0;
					}
				}
				p_start = new CharPtr(tok.start);
				p_end = new CharPtr(tok.cur);
				return STRING;
			}
			
			if (c == '\\') 
			{
				c = tok_nextc(tok);
				if (c != '\n') 
				{
					tok.done = E_TOKEN;
					tok.cur = tok.inp;
					return ERRORTOKEN;
				}
				goto again;
			}
			
			{
				int c2 = tok_nextc(tok);
				int token = PyToken_TwoChars(c, c2);
				if (token != OP) 
				{
					int c3 = tok_nextc(tok);
					int token3 = PyToken_ThreeChars(c, c2, c3);
					if (token3 != OP) 
					{
						token = token3;
					} 
					else 
					{
						tok_backup(tok, c3);
					}
					p_start = new CharPtr(tok.start);
					p_end = new CharPtr(tok.cur);
					return token;
				}
				tok_backup(tok, c2);
			}
			
			switch (c) 
			{
			case '(':
			case '[':
			case '{':
				tok.level++;
				break;
		
			case ')':
			case ']':
			case '}':
				tok.level--;
				break;
			}
			
			p_start = new CharPtr(tok.start);
			p_end = new CharPtr(tok.cur);
			return PyToken_OneChar(c);
		}
		
		#if _DEBUG
		public static void tok_dump(int type, CharPtr start, CharPtr end)
		{
			printf("%s", _PyParser_TokenNames[type]);
			if (type == NAME || type == NUMBER || type == STRING || type == OP)
			{
				printf("(%.*s)", (int)(end - start), start);
			}
		}
		#endif
	}
}
