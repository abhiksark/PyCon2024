from setuptools import setup
from Cython.Build import cythonize

setup(
    ext_modules=cythonize(
        "addv2.pyx",
        compiler_directives={"language_level": "3"},
        annotate=True,
        build_dir=".",
    )
)