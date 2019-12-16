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
		}	
			
		
		
		public class labellist 
		{
		    public int		 ll_nlabels;
		    public labelPtr	ll_label;
		}	
		
		public class arc 
		{
		    public short	a_lbl;
		    public short	a_arrow;		
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
		}	
		
		public class grammar
		{
		    public int		 g_ndfas;
		    public dfaPtr		g_dfa;
		    public labellist	 g_ll = new labellist();
		    public int		 g_start;
		    public int		 g_accel;		
		}
		
		public class state
		{
		    public int		 s_narcs;
		    public arcPtr	 s_arc;
		    public int		 s_lower;
		    public int		 s_upper;
		    public intPtr    s_accel;
		    public int		 s_accept;		
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
		}
		
		public class dfa
		{
		    public int		 d_type;
	//	    public char	*d_name;
		    public int		 d_initial;
		    public int		 d_nstates;
		    public statePtr	d_state;
		    public bitset	 d_first;		
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
			
		}
		
		public static void PyMem_DEL(ref intPtr x) {}
		public static intPtr PyMem_NEW_int(int n) { return null; }		
		
		public const int NT_OFFSET = 256;
		
		public static bool ISTERMINAL(int x) { return ((x) < NT_OFFSET); }
		public static bool ISNONTERMINAL(int x)	{ return ((x) >= NT_OFFSET); }
		public static dfa PyGrammar_FindDFA(grammar g, int type) { return null; }	
		public static bool testbit(bitset ss, int ibit) { return false; }
		
		public const int EMPTY = 0;	
	}
}
