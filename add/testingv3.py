import timeit

v2 = timeit.timeit('''addv2.add(1000)''',setup='import addv2',number=1000)
v3 = timeit.timeit('''addv3.add(1000)''',setup='import addv3', number=1000)

print(v2, v3)
print('Cython is {}x faster'.format(v2/v3))