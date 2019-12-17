#define _DEBUG

using System;

namespace Cecilia
{
	public partial class Python
	{
		//20170403
		//#include "pgenheaders.h"
		//#include "bitset.h"

		public static bitset newbitset(int nbits)
		{
			int nbytes = NBYTES(nbits);
			bitset ss = PyMem_NEW_char(nbytes);
			
			if (ss == null)
			{
				Py_FatalError("no mem for bitset");
			}
			ss.inc(nbytes);
			while (--nbytes >= 0)
			{
				ss.dec(); ss[0] = (char)0;
			}
			return ss;
		}
		
		public static void delbitset(bitset ss)
		{
			PyMem_DEL(ref ss);
		}
		
		public static int addbit(bitset ss, int ibit)
		{
			int ibyte = BIT2BYTE(ibit);
			char mask = BIT2MASK(ibit);
			
			if (0!=(ss[ibyte] & mask))
			{
				return 0;
			}
			ss[ibyte] |= mask;
			return 1;
		}
		
		public static int samebitset(bitset ss1, bitset ss2, int nbits)
		{
			int i;
			ss1 = new bitset(ss1);
			ss2 = new bitset(ss2);
			
			for (i = NBYTES(nbits); --i >= 0; )
			{
				char ss1_c = ss1[0]; ss1.inc();
				char ss2_c = ss2[0]; ss2.inc();
				if (ss1_c != ss2_c)
				{
					return 0;
				}
			}
			return 1;
		}
		
		public static void mergebitset(bitset ss1, bitset ss2, int nbits)
		{
			int i;
			ss1 = new bitset(ss1);
			ss2 = new bitset(ss2);
			
			for (i = NBYTES(nbits); --i >= 0; )
			{
				ss1[0] |= ss2[0];
				ss1.inc();
				ss2.inc();
			}
		}
	}
}
