#define _DEBUG

using System;

namespace Cecilia
{
	using size_t = System.UInt32;
	
	public partial class Python
	{
		//20170403
		//#include "pgenheaders.h"
		//#include "tokenizer.h"
		//#include "node.h"
		//#include "grammar.h"
		//#include "parser.h"
		//#include "parsetok.h"
		//#include "errcode.h"
		
		public static int Py_TabcheckFlag;
		
		//static node *parsetok(struct tok_state *, grammar *, int, perrdetail *, int);
		//static void initerr(perrdetail *err_ret, char* filename);
		
		public static node PyParser_ParseString(CharPtr s, grammar g, int start, perrdetail err_ret)
		{
			return PyParser_ParseStringFlags(s, g, start, err_ret, 0);
		}
		
		public static node PyParser_ParseStringFlags(CharPtr s, grammar g, int start, 
										perrdetail err_ret, int flags)
		{
			tok_state tok;
		
			initerr(err_ret, null);
		
			if ((tok = PyTokenizer_FromString(s)) == null) 
			{
				err_ret.error = E_NOMEM;
				return null;
			}
		
			if (0!=Py_TabcheckFlag || 0!=Py_VerboseFlag) 
			{
				tok.filename = "<string>";
				tok.altwarning = (tok.filename != null)?1:0;
				if (Py_TabcheckFlag >= 2)
				{
					tok.alterror++;
				}
			}
			return parsetok(tok, g, start, err_ret, flags);
		}
		
		public static node PyParser_ParseFile(FILEPtr fp, CharPtr filename, grammar g, int start,
				   CharPtr ps1, CharPtr ps2, perrdetail err_ret)
		{
			return PyParser_ParseFileFlags(fp, filename, g, start, ps1, ps2,
						       err_ret, 0);
		}
		
		public static node PyParser_ParseFileFlags(FILEPtr fp, CharPtr filename, grammar g, int start,
					CharPtr ps1, CharPtr ps2, perrdetail err_ret, int flags)
		{
			tok_state tok;
		
			initerr(err_ret, filename);
		
			if ((tok = PyTokenizer_FromFile(fp, ps1, ps2)) == null) 
			{
				err_ret.error = E_NOMEM;
				return null;
			}
			if (0!=Py_TabcheckFlag || 0!=Py_VerboseFlag) 
			{
				tok.filename = filename;
				tok.altwarning = (filename != null)?1:0;
				if (Py_TabcheckFlag >= 2)
				{
					tok.alterror++;
				}
			}
		
			return parsetok(tok, g, start, err_ret, flags);
		}
		
		private static CharPtr yield_msg =
			"%s:%d: Warning: 'yield' will become a reserved keyword in the future\n";
		
		private static node parsetok(tok_state tok, grammar g, int start, perrdetail err_ret, int flags)
		{
			parser_state ps;
			node n;
			int started = 0;
		
			if ((ps = PyParser_New(g, start)) == null) 
			{
				fprintf(stderr, "no mem for new parser\n");
				err_ret.error = E_NOMEM;
				return null;
			}
			if (0!=(flags & PyPARSE_YIELD_IS_KEYWORD))
			{
				ps.p_generators = 1;
			}
			for (;;) 
			{
				CharPtr a = null, b = null;
				int type;
				size_t len;
				CharPtr str;
		
				type = PyTokenizer_Get(tok, ref a, ref b);
				if (type == ERRORTOKEN) 
				{
					err_ret.error = tok.done;
					break;
				}
				if (type == ENDMARKER && 0!=started) 
				{
					type = NEWLINE;
					started = 0;
				}
				else
				{
					started = 1;
				}
				len = (size_t)(b - a);
				str = PyMem_NEW_char2((int)(len + 1));
				if (str == null) 
				{
					fprintf(stderr, "no mem for next token\n");
					err_ret.error = E_NOMEM;
					break;
				}
				if (len > 0)
				{
					strncpy(str, a, (int)len);
				}
				str[len] = '\0';
				if (type == NAME && 0==ps.p_generators &&
				    len == 5 && str[0] == 'y' && strcmp(str, "yield") == 0)
				{
					PySys_WriteStderr(yield_msg,
							  err_ret.filename==null ?
							  "<string>" : err_ret.filename,
							  tok.lineno);
				}
				if ((err_ret.error =
				     PyParser_AddToken(ps, (int)type, str, tok.lineno,
						       ref err_ret.expected)) != E_OK) 
				{
					if (err_ret.error != E_DONE)
					{
						PyMem_DEL(str);
					}
					break;
				}
			}
			if (err_ret.error == E_DONE) 
			{
				n = ps.p_tree;
				ps.p_tree = null;
			}
			else
			{
				n = null;
			}
			PyParser_Delete(ps);
		
			if (n == null) 
			{
				if (tok.lineno <= 1 && tok.done == E_EOF)
				{
					err_ret.error = E_EOF;
				}
				err_ret.lineno = tok.lineno;
				err_ret.offset = tok.cur - tok.buf;
				if (tok.buf != null) 
				{
					size_t len = (size_t)(tok.inp - tok.buf);
					err_ret.text = PyMem_NEW_char2((int)(len + 1));
					if (err_ret.text != null) 
					{
						if (len > 0)
						{
							strncpy(err_ret.text, tok.buf, (int)len);
						}
						err_ret.text[len] = '\0';
					}
				}
			}
		
			PyTokenizer_Free(tok);
		
			return n;
		}
		
		private static void initerr(perrdetail err_ret, CharPtr filename)
		{
			err_ret.error = E_OK;
			err_ret.filename = filename;
			err_ret.lineno = 0;
			err_ret.offset = 0;
			err_ret.text = null;
			err_ret.token = -1;
			err_ret.expected = -1;
		}
	}
}
