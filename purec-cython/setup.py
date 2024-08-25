from setuptools import setup, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension("example", ["example.pyx", "greet.c"])
]

setup(
    name="example",
    ext_modules=cythonize(ext_modules)
)