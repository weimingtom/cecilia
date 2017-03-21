namespace cecilia 
{
	public partical class cecilia 
	{
		/* Bitset primitives used by the parser generator */

		public static bitset
		newbitset(int nbits)
		{
			int nbytes = NBYTES(nbits);
			bitset ss = PyMem_NEW(BYTE, nbytes);
			
			if (ss == NULL)
				Py_FatalError("no mem for bitset");
			
			ss += nbytes;
			while (--nbytes >= 0)
				*--ss = 0;
			return ss;
		}

		public static void
		delbitset(bitset ss)
		{
			PyMem_DEL(ss);
		}

		public static int
		addbit(bitset ss, int ibit)
		{
			int ibyte = BIT2BYTE(ibit);
			BYTE mask = BIT2MASK(ibit);
			
			if (ss[ibyte] & mask)
				return 0; /* Bit already set */
			ss[ibyte] |= mask;
			return 1;
		}

		#if 0 /* Now a macro */
		public static int
		testbit(bitset ss, int ibit)
		{
			return (ss[BIT2BYTE(ibit)] & BIT2MASK(ibit)) != 0;
		}
		#endif

		public static int
		samebitset(bitset ss1, bitset ss2, int nbits)
		{
			int i;
			
			for (i = NBYTES(nbits); --i >= 0; )
				if (*ss1++ != *ss2++)
					return 0;
			return 1;
		}

		public static void
		mergebitset(bitset ss1, bitset ss2, int nbits)
		{
			int i;
			
			for (i = NBYTES(nbits); --i >= 0; )
				*ss1++ |= *ss2++;
		}
	}
}
