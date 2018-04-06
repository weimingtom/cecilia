//20180318
#pragma once

DL_IMPORT(long) PyImport_GetMagicNumber();
DL_IMPORT(PyObject *) PyImport_ExecCodeModule(char *name, PyObject *co);
DL_IMPORT(PyObject *) PyImport_ExecCodeModuleEx(
	char *name, PyObject *co, char *pathname);
DL_IMPORT(PyObject *) PyImport_GetModuleDict();
DL_IMPORT(PyObject *) PyImport_AddModule(char *name);
DL_IMPORT(PyObject *) PyImport_ImportModule(char *name);
DL_IMPORT(PyObject *) PyImport_ImportModuleEx(
	char *name, PyObject *globals, PyObject *locals, PyObject *fromlist);
DL_IMPORT(PyObject *) PyImport_Import(PyObject *name);
DL_IMPORT(PyObject *) PyImport_ReloadModule(PyObject *m);
DL_IMPORT(void) PyImport_Cleanup();
DL_IMPORT(int) PyImport_ImportFrozenModule(char *);

extern DL_IMPORT(PyObject *)_PyImport_FindExtension(char *, char *);
extern DL_IMPORT(PyObject *)_PyImport_FixupExtension(char *, char *);

struct _inittab {
    char *name;
    void (*initfunc)();
};

extern DL_IMPORT(struct _inittab *) PyImport_Inittab;

extern DL_IMPORT(int) PyImport_AppendInittab(char *name, void (*initfunc)());
extern DL_IMPORT(int) PyImport_ExtendInittab(struct _inittab *newtab);

struct _frozen {
    char *name;
    unsigned char *code;
    int size;
};

extern DL_IMPORT(struct _frozen *) PyImport_FrozenModules;
