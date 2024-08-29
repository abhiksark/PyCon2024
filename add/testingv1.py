import timeit

v1 = timeit.timeit('''addv0.add(100)''',setup='import addv0',number=1000)
v2 = timeit.timeit('''addv1.add(100)''',setup='import addv1', number=1000)

print(v1, v2)
print('Cython is {}x faster'.format(v1/v2))