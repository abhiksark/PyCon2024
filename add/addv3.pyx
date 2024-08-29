cpdef int add(int x):
    cdef:
        int i, j, y = 0

    for i in range(x):
        for j in range(x):
            y += i
    return y