def add(int x):
    cdef int i
    cdef int j
    cdef int y=0
    for i in range(x):
        for j in range(x):
            y += i
    return y