import sys
import gc
import ctypes

def inspect_object(obj):
    print(f"Object: {obj}")
    print(f"Type: {type(obj)}")
    print(f"ID (memory address): {id(obj)}")
    print(f"Reference count: {sys.getrefcount(obj) - 1}")  # Subtract 1 for the temporary reference
    print(f"Size: {sys.getsizeof(obj)} bytes")

    # Base PyObject info
    print("PyObject info:")
    print(f"  ob_refcnt: {sys.getrefcount(obj) - 1}")
    print(f"  ob_type: {type(obj)}")

    # Check if it's a variable-sized object (PyVarObject)
    var_sized = hasattr(obj, '__len__')
    if var_sized:
        print("PyVarObject info:")
        print(f"  ob_size: {len(obj)}")

    # Type-specific information
    if isinstance(obj, int):
        print("PyLongObject info:")
        print(f"  Bit length: {obj.bit_length()}")
        print(f"  Number of digits in base 2: {obj.bit_length()}")
        print(f"  Number of digits in base 10: {len(str(abs(obj)))}")
        print(f"  Sign: {'-' if obj < 0 else '+'}")
        
        # Attempt to access ob_digit (this is not guaranteed to work and may be unsafe)
        try:
            long_obj = ctypes.cast(id(obj), ctypes.POINTER(ctypes.c_longlong)).contents
            print(f"  First digit (ob_digit[0]): {long_obj.value & ((1 << 30) - 1)}")
        except:
            print("  Unable to safely access ob_digit")

    elif isinstance(obj, str):
        print("PyUnicodeObject info:")
        print(f"  Length: {len(obj)}")
        print(f"  UTF-8 Size: {len(obj.encode('utf-8'))} bytes")
        print(f"  Is ASCII: {obj.isascii()}")

    elif isinstance(obj, list):
        print("PyListObject info:")
        print(f"  Length: {len(obj)}")
        print(f"  Allocated: {sys.getsizeof(obj) // ctypes.sizeof(ctypes.c_voidp)}")

    elif isinstance(obj, dict):
        print("PyDictObject info:")
        print(f"  Length: {len(obj)}")
        print(f"  Keys: {list(obj.keys())}")

    elif isinstance(obj, tuple):
        print("PyTupleObject info:")
        print(f"  Length: {len(obj)}")

    print("Attributes:")
    for attr in dir(obj):
        if not attr.startswith('__'):
            try:
                value = getattr(obj, attr)
                print(f"  {attr}: {value}")
            except:
                print(f"  {attr}: <unable to retrieve>")

    print("Memory address of referenced objects:")
    for ref in gc.get_referents(obj):
        print(f"  {type(ref)}: {id(ref)}")

# Test with different objects
test_objects = [
    1,
    1234567890,
    -9876543210,
    2**100,
    "Hello, World!",
    "안녕하세요",  # non-ASCII string
    [1, 2, 3],
    {'a': 1, 'b': 2},
    (1, 2, 3)
]

for obj in test_objects:
    print("\n" + "=" * 70)
    inspect_object(obj)

# Print information about integer representation
print("\n" + "=" * 70)
print(f"sys.int_info: {sys.int_info}")