cdef extern from "add.h":
    int _add "add"(int a)

def add(a):
    ''' Sum of two numbers'''
    return _add(a)
