import ctypes
import sys

# Define the ctypes structure to simulate PyObject
class PyObject(ctypes.Structure):
    _fields_ = [
        ("ob_refcnt", ctypes.c_long),  # Reference count
        ("ob_type", ctypes.c_void_p)   # Pointer to the type object
    ]

# Define the ctypes structure to simulate PyVarObject (used by variable-sized objects like PyLongObject)
class PyVarObject(PyObject):
    _fields_ = [
        ("ob_size", ctypes.c_long)  # Number of elements in variable-sized objects
    ]

# Define the ctypes structure to simulate PyLongObject
class PyLongObject(PyVarObject):
    _fields_ = [
        ("ob_digit", ctypes.c_uint * 1)  # Array of digits (this will be resized dynamically)
    ]

def print_pylong_object(num):
    # Access the integer object's address in memory
    address_of_num = id(num)

    # Cast the address to a PyLongObject pointer
    long_obj = ctypes.cast(address_of_num, ctypes.POINTER(PyLongObject)).contents

    print(f"Integer value: {num}")
    print(f"ob_refcnt: {long_obj.ob_refcnt}")
    print(f"ob_type: {long_obj.ob_type}")
    print(f"ob_size: {abs(long_obj.ob_size)}")  # Use abs() as ob_size can be negative for negative integers

    # Dynamically resize the ob_digit array based on ob_size
    class PyLongObject_Dynamic(PyVarObject):
        _fields_ = [
            ("ob_digit", ctypes.c_uint * abs(long_obj.ob_size))
        ]

    # Re-cast with the correct size
    long_obj_dynamic = ctypes.cast(address_of_num, ctypes.POINTER(PyLongObject_Dynamic)).contents

    print("ob_digit values:")
    for i in range(abs(long_obj.ob_size)):
        print(f"  ob_digit[{i}]: {long_obj_dynamic.ob_digit[i]}")

# Test with different integer sizes
numbers_to_test = [
    1,
    1234567890,
    2**63 - 1,  # Maximum 64-bit signed integer
    2**64,
    2**128,
    -1234567890
]

for num in numbers_to_test:
    print("\n" + "="*40)
    print_pylong_object(num)

# Print information about integer representation
print("\n" + "="*40)
print(f"sys.int_info: {sys.int_info}")