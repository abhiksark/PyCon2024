# file: nogil_calc.pyx
cimport cython
from cython.parallel import prange
from libc.math cimport sin, cos, exp, log, sqrt
import time

@cython.boundscheck(False)
@cython.wraparound(False)
def calculate_nogil(int start, int end):
    cdef double result = 0.0
    cdef int i
    for i in prange(start, end, nogil=True):
        result += sin(i)
        result += cos(i) 
        result += sin(i) 
    return result