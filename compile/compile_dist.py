from distutils.core import setup
from Cython.Build import cythonize
from setuptools import Extension

# write the object file here itself

ext = Extension(
    name="addv1",
    sources=["addv1.pyx"],
    extra_compile_args=["-O3", "-march=native"],
)

setup(
    ext_modules=cythonize(ext),
    build_dir=".",
)