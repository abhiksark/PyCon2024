import timeit

v1 = timeit.timeit('''addv1.add(10000)''',setup='import addv1',number=100)
v4= timeit.timeit('''addv4.add(10000)''',setup='import addv4', number=100)

print(v1, v4)
print('Cython is {}x faster'.format(v1/v4))