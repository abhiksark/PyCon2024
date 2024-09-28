import ctypes
import sys  

# Load the shared library into ctypes
if sys.platform == 'darwin':
    example = ctypes.CDLL('./example.dylib')
else:
    example = ctypes.CDLL('./example.so')

# Set the argument types and return type for the greet function
example.greet.argtypes = [ctypes.c_char_p]
example.greet.restype = None

# Call the greet function from the C code
example.greet(b"Abhik!")