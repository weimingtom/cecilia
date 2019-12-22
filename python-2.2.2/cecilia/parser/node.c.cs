#define _DEBUG

using System;

namespace Cecilia
{
	using size_t = System.UInt32;
	
	public partial class Python
	{
		//20170403
		//#include "python.h"
		//#include "node.h"
		//#include "errcode.h"
		
		public static node PyNode_New(int type)
		{
			node n = PyMem_NEW_node(1);
			if (n == null)
			{
				return null;
			}
			n.n_type = (short)type;
			n.n_str = null;
			n.n_lineno = 0;
			n.n_nchildren = 0;
			n.n_child = null;
			return n;
		}
		
		private static int fancy_roundup(int n)
		{
			int result = 256;
			assert(n > 128);
			while (result < n) 
			{
				result <<= 1;
				if (result <= 0)
				{
					return -1;
				}
			}
			return result;
		}
		
		private static int XXXROUNDUP(int n) { return ((n) == 1 ? 1 :
			(n) <= 128 ? (((n) + 3) & ~3) :
		    fancy_roundup(n)); }
		
		public static int PyNode_AddChild(node n1, int type, CharPtr str, int lineno)
		{
			int nch = n1.n_nchildren;
			int current_capacity;
			int required_capacity;
			nodePtr n;
		
			if (nch == INT_MAX || nch < 0)
			{
				return E_OVERFLOW;
			}
			current_capacity = XXXROUNDUP(nch);
			required_capacity = XXXROUNDUP(nch + 1);
			if (current_capacity < 0 || required_capacity < 0)
			{
				return E_OVERFLOW;
			}
			if (current_capacity < required_capacity) 
			{
				n = new nodePtr(n1.n_child);
				PyMem_RESIZE_node(ref n, required_capacity);
				if (n == null)
				{
					return E_NOMEM;
				}
				n1.n_child = n;
			}
			n = new nodePtr(n1.n_child, n1.n_nchildren++);
			n[0].n_type = (short)type;
			n[0].n_str = str;
			n[0].n_lineno = lineno;
			n[0].n_nchildren = 0;
			n[0].n_child = null;
			return 0;
		}
		
		//static void freechildren(node *);
		
		public static void PyNode_Free(node n)
		{
			if (n != null) 
			{
				freechildren(n);
				PyMem_DEL(ref n);
			}
		}
		
		private static void freechildren(node n)
		{
			int i;
			for (i = NCH(n); --i >= 0; )
			{
				freechildren(CHILD(n, i));
			}
			if (n.n_child != null)
			{
				PyMem_DEL(ref n.n_child);
			}
			if (STR(n) != null)
			{
				PyMem_DEL(STR(n));
			}
		}
	}
}
