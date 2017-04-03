#pragma once

DL_IMPORT(PyObject *) PyEval_EvalCode(PyCodeObject *, PyObject *, PyObject *);

DL_IMPORT(PyObject *) PyEval_EvalCodeEx(PyCodeObject *co,
					PyObject *globals,
					PyObject *locals,
					PyObject **args, int argc,
					PyObject **kwds, int kwdc,
					PyObject **defs, int defc,
					PyObject *closure);

