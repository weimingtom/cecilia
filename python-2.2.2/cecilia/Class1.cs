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
		
		public static void PyMem_DEL(ref intPtr x) {}
		public static void PyMem_DEL(ref bitset x) {}
		public static intPtr PyMem_NEW_int(int n) { return null; }		
		public static bitset PyMem_NEW_char(int n) { return null; }		
		
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
		}		
		public static int NCH(node n) { return (n.n_nchildren); }
		public static node CHILD(node n, int i)	{ return (n.n_child[i]); }
		public static short TYPE(node n) { return (n.n_type); }
		public static CharPtr STR(node n) { return (n.n_str); }
		public static void REQ(node n, short type) { assert(TYPE(n) == (type)); }
		public const int INDENT	= 5;
		public const int DEDENT	= 6;
		public const int NEWLINE = 4;
	}
}
