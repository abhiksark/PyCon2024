#include <Python.h>

static PyObject* greet(PyObject* self, PyObject* args) {
    const char* name;
    
    if (!PyArg_ParseTuple(args, "s", &name)) {
        return NULL;
    }
    
    printf("Hello, %s!\n", name);
    
    Py_RETURN_NONE;
}

static PyMethodDef ExampleMethods[] = {
    {"greet", greet, METH_VARARGS, "Greet the user."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef examplemodule = {
    PyModuleDef_HEAD_INIT,
    "example",
    "Example module",
    -1,
    ExampleMethods
};

PyMODINIT_FUNC PyInit_example(void) {
    return PyModule_Create(&examplemodule);
}