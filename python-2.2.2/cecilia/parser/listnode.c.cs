#define _DEBUG

using System;

namespace Cecilia
{
	public partial class Python
	{
		//20170403
		//#include "pgenheaders.h"
		//#include "token.h"
		//#include "node.h"
		
		//static void list1node(FILE *, node *);
		//static void listnode(FILE *, node *);
		
		public static void PyNode_ListTree(node n)
		{
			listnode(stdout, n);
		}
		
		private static int level, atbol;
		
		private static void listnode(FILEPtr fp, node n)
		{
			level = 0;
			atbol = 1;
			list1node(fp, n);
		}
		
		private static void list1node(FILEPtr fp, node n)
		{
			if (n == null)
			{
				return;
			}
			if (ISNONTERMINAL(TYPE(n))) 
			{
				int i;
				for (i = 0; i < NCH(n); i++)
				{
					list1node(fp, CHILD(n, i));
				}
			}
			else if (ISTERMINAL(TYPE(n))) 
			{
				switch (TYPE(n)) 
				{
				case INDENT:
					++level;
					break;
		
				case DEDENT:
					--level;
					break;
		
				default:
					if (atbol != 0) 
					{
						int i;
						for (i = 0; i < level; ++i)
						{
							fprintf(fp, "\t");
						}
						atbol = 0;
					}
					if (TYPE(n) == NEWLINE) 
					{
						if (STR(n) != null)
						{
							fprintf(fp, "%s", STR(n));
						}
						fprintf(fp, "\n");
						atbol = 1;
					}
					else
					{
						fprintf(fp, "%s ", STR(n));
					}
					break;
				}
			}
			else
			{
				fprintf(fp, "? ");
			}
		}
	}
}
