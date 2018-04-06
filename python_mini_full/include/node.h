//20180318
#pragma once

typedef struct _node {
    short		n_type;
    char		*n_str;
    int			n_lineno;
    int			n_nchildren;
    struct _node	*n_child;
} node;

extern DL_IMPORT(node *) PyNode_New(int type);
extern DL_IMPORT(int) PyNode_AddChild(node *n, int type,
                                      char *str, int lineno);
extern DL_IMPORT(void) PyNode_Free(node *n);

#define NCH(n)		((n)->n_nchildren)
#define CHILD(n, i)	(&(n)->n_child[i])
#define TYPE(n)		((n)->n_type)
#define STR(n)		((n)->n_str)

#define REQ(n, type) assert(TYPE(n) == (type))

extern DL_IMPORT(void) PyNode_ListTree(node *);
