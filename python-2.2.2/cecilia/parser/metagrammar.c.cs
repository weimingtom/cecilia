#define _DEBUG

using System;

namespace Cecilia
{
	public partial class Python
	{
		//20170415
		//#include "pgenheaders.h"
		//#include "metagrammar.h"
		//#include "grammar.h"
		//#include "pgen.h"
		
		private static arc[] arcs_0_0 = {
			new arc(2, 0),
			new arc(3, 0),
			new arc(4, 1),
		};
		
		private static arc[] arcs_0_1 = {
			new arc(0, 1),
		};
		
		private static state[] states_0 = {
			new state(3, arcs_0_0),
			new state(1, arcs_0_1),
		};
		
		private static arc[] arcs_1_0 = {
			new arc(5, 1),
		};
		
		private static arc[] arcs_1_1 = {
			new arc(6, 2),
		};
		
		private static arc[] arcs_1_2 = {
			new arc(7, 3),
		};
		
		private static arc[] arcs_1_3 = {
			new arc(3, 4),
		};
		
		private static arc[] arcs_1_4 = {
			new arc(0, 4),
		};
		
		private static state[] states_1 = {
			new state(1, arcs_1_0),
			new state(1, arcs_1_1),
			new state(1, arcs_1_2),
			new state(1, arcs_1_3),
			new state(1, arcs_1_4),
		};
		
		private static arc[] arcs_2_0 = {
			new arc(8, 1),
		};
		
		private static arc[] arcs_2_1 = {
			new arc(9, 0),
			new arc(0, 1),
		};
		
		private static state[] states_2 = {
			new state(1, arcs_2_0),
			new state(2, arcs_2_1),
		};
		
		private static arc[] arcs_3_0 = {
			new arc(10, 1),
		};
		
		private static arc[] arcs_3_1 = {
			new arc(10, 1),
			new arc(0, 1),
		};
		
		private static state[] states_3 = {
			new state(1, arcs_3_0),
			new state(2, arcs_3_1),
		};
		
		private static arc[] arcs_4_0 = {
			new arc(11, 1),
			new arc(13, 2),
		};
		
		private static arc[] arcs_4_1 = {
			new arc(7, 3),
		};
		
		private static arc[] arcs_4_2 = {
			new arc(14, 4),
			new arc(15, 4),
			new arc(0, 2),
		};
		
		private static arc[] arcs_4_3 = {
			new arc(12, 4),
		};
		
		private static arc[] arcs_4_4 = {
			new arc(0, 4),
		};
		
		private static state[] states_4 = {
			new state(2, arcs_4_0),
			new state(1, arcs_4_1),
			new state(3, arcs_4_2),
			new state(1, arcs_4_3),
			new state(1, arcs_4_4),
		};
		
		private static arc[] arcs_5_0 = {
			new arc(5, 1),
			new arc(16, 1),
			new arc(17, 2),
		};
		
		private static arc[] arcs_5_1 = {
			new arc(0, 1),
		};
		
		private static arc[] arcs_5_2 = {
			new arc(7, 3),
		};
		
		private static arc[] arcs_5_3 = {
			new arc(18, 1),
		};
		
		private static state[] states_5 = {
			new state(3, arcs_5_0),
			new state(1, arcs_5_1),
			new state(1, arcs_5_2),
			new state(1, arcs_5_3),
		};
		
		private static dfa[] dfas = {
			new dfa(256, "MSTART", 0, 2, states_0,
			 "\070\000\000"),
			new dfa(257, "RULE", 0, 5, states_1,
			 "\040\000\000"),
			new dfa(258, "RHS", 0, 2, states_2,
			 "\040\010\003"),
			new dfa(259, "ALT", 0, 2, states_3,
			 "\040\010\003"),
			new dfa(260, "ITEM", 0, 5, states_4,
			 "\040\010\003"),
			new dfa(261, "ATOM", 0, 4, states_5,
			 "\040\000\003"),
		};
		
		private static label[] labels = {
			new label(0, "EMPTY"),
			new label(256, null),
			new label(257, null),
			new label(4, null),
			new label(0, null),
			new label(1, null),
			new label(11, null),
			new label(258, null),
			new label(259, null),
			new label(18, null),
			new label(260, null),
			new label(9, null),
			new label(10, null),
			new label(261, null),
			new label(16, null),
			new label(14, null),
			new label(3, null),
			new label(7, null),
			new label(8, null),
		};
		
		private static grammar _PyParser_Grammar = new grammar(
			6,
			dfas,
			new labellist(19, labels),
			256
		);
		
		public static grammar meta_grammar()
		{
			return _PyParser_Grammar;
		}
	}
}
