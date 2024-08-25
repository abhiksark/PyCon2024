cdef extern from "greet.h":
    void greet(const char* name)

def greet_from_python(name):
    greet(name.encode('utf-8'))