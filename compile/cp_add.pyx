cdef int add(int x):
    cdef int y = 0
    cdef int i
    for i in range(x):
        y +=i
    return y