//20180324
#pragma once

#define STOP_CODE	0
#define POP_TOP		1
#define ROT_TWO		2
#define ROT_THREE	3
#define DUP_TOP		4
#define ROT_FOUR	5

#define UNARY_POSITIVE	10
#define UNARY_NEGATIVE	11
#define UNARY_NOT	12
#define UNARY_CONVERT	13

#define UNARY_INVERT	15

#define BINARY_POWER	19

#define BINARY_MULTIPLY	20
#define BINARY_DIVIDE	21
#define BINARY_MODULO	22
#define BINARY_ADD	23
#define BINARY_SUBTRACT	24
#define BINARY_SUBSCR	25
#define BINARY_FLOOR_DIVIDE 26
#define BINARY_TRUE_DIVIDE 27
#define INPLACE_FLOOR_DIVIDE 28
#define INPLACE_TRUE_DIVIDE 29

#define SLICE		30

#define STORE_SLICE	40

#define DELETE_SLICE	50

#define INPLACE_ADD	55
#define INPLACE_SUBTRACT	56
#define INPLACE_MULTIPLY	57
#define INPLACE_DIVIDE	58
#define INPLACE_MODULO	59
#define STORE_SUBSCR	60
#define DELETE_SUBSCR	61

#define BINARY_LSHIFT	62
#define BINARY_RSHIFT	63
#define BINARY_AND	64
#define BINARY_XOR	65
#define BINARY_OR	66
#define INPLACE_POWER	67
#define GET_ITER	68

#define PRINT_EXPR	70
#define PRINT_ITEM	71
#define PRINT_NEWLINE	72
#define PRINT_ITEM_TO   73
#define PRINT_NEWLINE_TO 74
#define INPLACE_LSHIFT	75
#define INPLACE_RSHIFT	76
#define INPLACE_AND	77
#define INPLACE_XOR	78
#define INPLACE_OR	79
#define BREAK_LOOP	80

#define LOAD_LOCALS	82
#define RETURN_VALUE	83
#define IMPORT_STAR	84
#define EXEC_STMT	85
#define YIELD_VALUE	86

#define POP_BLOCK	87
#define END_FINALLY	88
#define BUILD_CLASS	89

#define HAVE_ARGUMENT	90	

#define STORE_NAME	90	
#define DELETE_NAME	91	
#define UNPACK_SEQUENCE	92
#define FOR_ITER	93

#define STORE_ATTR	95
#define DELETE_ATTR	96
#define STORE_GLOBAL	97
#define DELETE_GLOBAL	98
#define DUP_TOPX	99
#define LOAD_CONST	100
#define LOAD_NAME	101
#define BUILD_TUPLE	102
#define BUILD_LIST	103
#define BUILD_MAP	104
#define LOAD_ATTR	105
#define COMPARE_OP	106
#define IMPORT_NAME	107
#define IMPORT_FROM	108

#define JUMP_FORWARD	110
#define JUMP_IF_FALSE	111
#define JUMP_IF_TRUE	112
#define JUMP_ABSOLUTE	113
#define FOR_LOOP	114

#define LOAD_GLOBAL	116

#define CONTINUE_LOOP	119
#define SETUP_LOOP	120
#define SETUP_EXCEPT	121
#define SETUP_FINALLY	122

#define LOAD_FAST	124
#define STORE_FAST	125
#define DELETE_FAST	126

#define SET_LINENO	127

#define RAISE_VARARGS	130
#define CALL_FUNCTION	131
#define MAKE_FUNCTION	132
#define BUILD_SLICE 	133

#define MAKE_CLOSURE    134     
#define LOAD_CLOSURE    135     
#define LOAD_DEREF      136      
#define STORE_DEREF     137     

#define CALL_FUNCTION_VAR          140	
#define CALL_FUNCTION_KW           141	
#define CALL_FUNCTION_VAR_KW       142	

#define EXTENDED_ARG  143

enum cmp_op {LT=Py_LT, LE=Py_LE, EQ=Py_EQ, NE=Py_NE, GT=Py_GT, GE=Py_GE,
	     IN, NOT_IN, IS, IS_NOT, EXC_MATCH, BAD};

#define HAS_ARG(op) ((op) >= HAVE_ARGUMENT)
