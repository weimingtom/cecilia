/*
 * Created by SharpDevelop.
 * User: 
 * Date: 2019/12/16
 * Time: 9:55
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;

namespace Cecilia
{	
	public partial class Python
	{
		public class label
		{
		    public int		 lb_type;
		    public CharPtr lb_str;	

		    public label(int lb_type, string lb_str)
		    {
		    	this.lb_type = lb_type;
		    	this.lb_str = lb_str;
		    }
		    public label() {}
		}
		public class labelPtr
		{
			public label this[int offset]
			{
				get { return null; }
				set { }
			}
			public void inc() { }
			public void dec() { }
			public labelPtr(labelPtr ptr) { }
			public labelPtr(label[] arr, int index) { }
		}	
			
		
		
		public class labellist 
		{
		    public int		 ll_nlabels;
		    public labelPtr	ll_label;
		    
		    public labellist(int ll_nlabels, label[] ll_label)
		    {
		    	this.ll_nlabels = ll_nlabels;
		    	this.ll_label = new labelPtr(ll_label, 0);
		    }
		}	
		
		public class arc 
		{
		    public short	a_lbl;
		    public short	a_arrow;
			
		    public arc(short a_lbl, short a_arrow)
		    {
		    	this.a_lbl = a_lbl;
		    	this.a_arrow = a_arrow;
		    }
		}
		public class arcPtr
		{
			public arc this[int offset]
			{
				get { return null; }
				set { }
			}
			public void inc() { }
			public void dec() { }
			public arcPtr(arcPtr ptr) { }
			public arcPtr(arc[] arr, int index) { }
		}	
		
		public class grammar
		{
		    public int		 g_ndfas;
		    public dfaPtr		g_dfa;
		    public labellist	 g_ll = new labellist(0, null);
		    public int		 g_start;
		    public int		 g_accel;
		    
		    public grammar(int g_ndfas, dfa[] g_dfa, labellist g_ll, int g_start)
		    {
		    	this.g_ndfas = g_ndfas;
		    	this.g_dfa = new dfaPtr(g_dfa, 0);
		    	this.g_ll = g_ll;
		    	this.g_start = g_start;
		    }
		}
		
		public class state
		{
		    public int		 s_narcs;
		    public arcPtr	 s_arc;
		    public int		 s_lower;
		    public int		 s_upper;
		    public intPtr    s_accel;
		    public int		 s_accept;

		    public state(int s_narcs, arc[] s_arc)
		    {
		    	this.s_narcs = s_narcs;
		    	this.s_arc = new arcPtr(s_arc, 0);
		    }
		}
		public class statePtr
		{
			public state this[int offset]
			{
				get { return null; }
				set { }
			}
			public void inc() { }
			public void dec() { }
			public statePtr(statePtr ptr) { }
			public statePtr(state[] arr, int index) { }
		}
		
		public class dfa
		{
		    public int		 d_type;
		    public CharPtr d_name;
		    public int		 d_initial;
		    public int		 d_nstates;
		    public statePtr	d_state;
		    public bitset	 d_first;

		    public dfa(int d_type, string d_name, int d_initial, int d_nstates , state[] d_state, string d_first)
		    {
		    	this.d_type = d_type;
		    	this.d_name = d_name;
		    	this.d_initial = d_initial;
		    	this.d_nstates = d_nstates;
		    	this.d_state = new statePtr(d_state, 0);
		    	this.d_first = new bitset(d_first);
		    }
		}
		public class dfaPtr
		{
			public dfa this[int offset]
			{
				get { return null; }
				set { }
			}		
			public void inc() { }
			public void dec() { }
			public dfaPtr(dfaPtr ptr) { }
			public dfaPtr(dfa[] arr, int index) { }
		}
	
		public class intPtr
		{
			public int this[int offset]
			{
				get { return 0; }
				set { }
			}		
			public void inc() { }
			public void dec() { }
			public intPtr(intPtr ptr) { }
		}
		
		public class bitset
		{
			public char this[int offset]
			{
				get { return '\0'; }
				set { }
			}		
			public void inc() { }
			public void inc(int n) { }
			public void dec() { }
			public bitset(bitset ptr) { }
			public bitset(string str) { }			
		}
		
		//FIXME: may be Ptr
		public static void PyMem_DEL(ref intPtr x) {}
		public static void PyMem_DEL(ref bitset x) {}
		public static void PyMem_DEL(ref node x) {}
		public static void PyMem_DEL(ref nodePtr x) {}
		public static void PyMem_DEL(CharPtr x) {}
		public static void PyMem_DEL(ref parser_state x) {}
		
		public static intPtr PyMem_NEW_int(int n) { return null; }
		public static bitset PyMem_NEW_char(int n) { return null; }	
		public static node PyMem_NEW_node(int n) { return null; }
		public static parser_state PyMem_NEW_parser_state(int n) {return null;}
		public static CharPtr PyMem_NEW_char2(int n) { return null; }	
		
		public static void PyMem_FREE(ref CharPtr p) {}
		public static CharPtr PyMem_REALLOC(CharPtr p, uint n) {return null;}
		public static CharPtr PyMem_MALLOC(uint n) {return null;}
		public static void PyMem_RESIZE_node(ref nodePtr x, int n) {}
		
		public const int NT_OFFSET = 256;
		
		public static bool ISTERMINAL(int x) { return ((x) < NT_OFFSET); }
		public static bool ISNONTERMINAL(int x)	{ return ((x) >= NT_OFFSET); }
		public static bool testbit(bitset ss, int ibit) { return false; }
		
		public const int EMPTY = 0;	
		
		public const int BITSPERBYTE = (8*1/*sizeof(char)*/);
		public static int NBYTES(int nbits) { return (((nbits) + BITSPERBYTE - 1) / BITSPERBYTE); }
		public static void Py_FatalError(CharPtr msg) {}
		public static int BIT2BYTE(int ibit) { return ((ibit) / BITSPERBYTE); }
		public static int BIT2SHIFT(int ibit) { return ((ibit) % BITSPERBYTE); }
		public static char BIT2MASK(int ibit) { return (char)(1 << BIT2SHIFT(ibit)); }
		public static int BYTE2BIT(int ibyte) { return ((ibyte) * BITSPERBYTE); }
		
		public const int ENDMARKER = 0;
		
		public static void PyOS_snprintf(CharPtr s, int n, CharPtr format,  params object[] par) {}
		public static string[] _PyParser_TokenNames = {};
		
		public class node {
		    public short		n_type;
		    public CharPtr n_str;
		    public int			n_lineno;
		    public int			n_nchildren;
		    public nodePtr	n_child;
		}
		public class nodePtr
		{
			public node this[int offset]
			{
				get { return null; }
				set { }
			}		
			public void inc() { }
			public void dec() { }
			public nodePtr(nodePtr ptr) { }
			public nodePtr(nodePtr ptr, int index) { }
		}		
		public static int NCH(node n) { return (n.n_nchildren); }
		public static node CHILD(node n, int i)	{ return (n.n_child[i]); }
		public static short TYPE(node n) { return (n.n_type); }
		public static CharPtr STR(node n) { return (n.n_str); }
		public static void REQ(node n, short type) { assert(TYPE(n) == (type)); }
		public const int INDENT	= 5;
		public const int DEDENT	= 6;
		public const int NEWLINE = 4;
		public static int PyOS_InterruptOccurred() { return 0; }
		
		public class PyThreadState
		{
			
		}
		public static PyThreadState PyEval_SaveThread() { return null; }
		public static void PyEval_RestoreThread(PyThreadState save) {}
		
		public class PyObject
		{
			
		}
		public static PyObject PyExc_OverflowError;
		public static void PyErr_SetString(PyObject o, CharPtr str) { }
		
		public const int E_OVERFLOW = 19;
		public const int E_NOMEM = 15;
		
		public static int Py_DebugFlag = 0;
		public const int NAME = 1;
		
		public const int EINTR = 4;
		public const int NUMBER = 2;
		public const int E_SYNTAX = 14;
		public const int E_OK = 10;
		public const int E_DONE = 16;
		public const int E_EOF = 11;		

		public class perrdetail {
		    public int error;
		    public CharPtr filename;
		    public int lineno;
		    public int offset;
		    public CharPtr text;
		    public int token;
		    public int expected;
		};
		
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
		
		public static tok_state PyTokenizer_FromString(CharPtr str) { return null; }
		public static int Py_VerboseFlag;
		public static tok_state PyTokenizer_FromFile(FILEPtr fp, CharPtr ps1, CharPtr ps2) { return null; }
		public static int PyTokenizer_Get(tok_state tok, ref CharPtr p_start, ref CharPtr p_end) { return 0; }
		
		public const int PyPARSE_YIELD_IS_KEYWORD = 0x0001;
		public const int ERRORTOKEN = 51;
		
		public static void PySys_WriteStderr(CharPtr format, params object[] par) {}
		public static void PyTokenizer_Free(tok_state x) {}
	}
}
