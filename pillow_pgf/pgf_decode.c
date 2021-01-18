#include <Python.h>


static PyObject* decode(PyObject* self, PyObject* args)
{
    printf("Hello World\n");
    return Py_None;
}


static PyMethodDef pgfMethods[] = {
    { "decode", decode, METH_NOARGS, "Prints Hello World" },
    { NULL, NULL, 0, NULL }
};


static struct PyModuleDef _decode = {
    PyModuleDef_HEAD_INIT,
    "pgf_decode",
    "PGF Decode",
    -1,
    pgfMethods
};


PyMODINIT_FUNC PyInit__decode(void)
{
    return PyModule_Create(&_decode);
}
