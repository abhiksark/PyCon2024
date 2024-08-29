cdef int add_cy(int x):
    cdef:
        int i, j, y = 0
        
    for i in range(x):
        for j in range(x):
            y += i
    return y

def add(x):
    return add_cy(x)