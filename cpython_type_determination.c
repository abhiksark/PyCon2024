#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DIGIT_SIZE 15

// Enum for different Python object types
enum PyObjectType {
    PyObject_INTEGER,
    PyObject_FLOAT,
    PyObject_STRING
};

// Struct representing a Python long object
typedef struct {
    size_t ob_refcnt;
    size_t ob_size;
    int ob_digit[DIGIT_SIZE];
} PyLongObject;

// Struct representing a Python object
typedef struct {
    enum PyObjectType type;
    union {
        PyLongObject long_value;
        double float_value;
        char* str_value;
    } value;
} PyObject;

// Forward declaration of PyTypeObject
typedef struct PyTypeObject PyTypeObject;

// Struct representing a Python type object
struct PyTypeObject {
    const char* name;
    size_t size;
    PyObject* (*tp_alloc)(PyTypeObject*, size_t);
    void (*tp_free)(void*);
    void (*tp_print)(PyObject*);
    // Other type-specific fields and function pointers
};

// Python type objects
PyTypeObject PyType_Integer;
PyTypeObject PyType_Float;
PyTypeObject PyType_String;

// Function to create a new Python object of a specific type
PyObject* PyObject_New(PyTypeObject* type) {
    PyObject* obj = (PyObject*)type->tp_alloc(type, 0);
    obj->type = (enum PyObjectType)(type - &PyType_Integer);
    return obj;
}

// Function to determine the type of a Python object
const char* PyObject_GetTypeName(PyObject* obj) {
    PyTypeObject* type = &PyType_Integer + obj->type;
    return type->name;
}

// Python type objects and their associated functions
PyObject* PyType_Integer_Alloc(PyTypeObject* type, size_t nitems) {
    PyObject* obj = (PyObject*)malloc(sizeof(PyObject));
    obj->value.long_value.ob_refcnt = 1;
    obj->value.long_value.ob_size = 0;
    memset(obj->value.long_value.ob_digit, 0, sizeof(obj->value.long_value.ob_digit));
    return obj;
}

void PyType_Integer_Free(void* obj) {
    free(obj);
}

void PyType_Integer_Print(PyObject* obj) {
    PyLongObject* longObj = &obj->value.long_value;
    size_t size = longObj->ob_size;

    if (size == 0) {
        printf("0");
        return;
    }

    if (longObj->ob_digit[size - 1] < 0) {
        printf("-");
    }

    // Print from most significant digit to least significant
    for (ssize_t i = size - 1; i >= 0; i--) {
        int digit = abs(longObj->ob_digit[i]);
        
        // For all digits except the most significant, ensure 9 digits are printed
        if (i != size - 1) {
            printf("%09d", digit);
        } else {
            printf("%d", digit);
        }
    }
}

PyObject* PyType_Float_Alloc(PyTypeObject* type, size_t nitems) {
    return (PyObject*)malloc(sizeof(PyObject));
}

void PyType_Float_Free(void* obj) {
    free(obj);
}

void PyType_Float_Print(PyObject* obj) {
    printf("%lf", obj->value.float_value);
}

PyObject* PyType_String_Alloc(PyTypeObject* type, size_t nitems) {
    return (PyObject*)malloc(sizeof(PyObject));
}

void PyType_String_Free(void* obj) {
    PyObject* strObj = (PyObject*)obj;
    free(strObj->value.str_value);
    free(obj);
}

void PyType_String_Print(PyObject* obj) {
    printf("%s", obj->value.str_value);
}

PyTypeObject PyType_Integer = {
    "int",
    sizeof(PyObject),
    PyType_Integer_Alloc,
    PyType_Integer_Free,
    PyType_Integer_Print
    // Other type-specific fields and function pointers
};

PyTypeObject PyType_Float = {
    "float",
    sizeof(PyObject),
    PyType_Float_Alloc,
    PyType_Float_Free,
    PyType_Float_Print
    // Other type-specific fields and function pointers
};

PyTypeObject PyType_String = {
    "str",
    sizeof(PyObject),
    PyType_String_Alloc,
    PyType_String_Free,
    PyType_String_Print
    // Other type-specific fields and function pointers
};

// Function to create a Python object from a given value
PyObject* PyObject_FromValue(const char* value) {
    PyObject* obj = NULL;

    // Check if the value is an integer
    if (strspn(value, "-0123456789") == strlen(value)) {
        obj = PyObject_New(&PyType_Integer);
        PyLongObject* longObj = &obj->value.long_value;

        size_t len = strlen(value);
        int sign = 1;
        size_t i = 0;

        if (value[0] == '-') {
            sign = -1;
            i = 1;
        }

        int base = 1000000000;  // Base for storing digits (10^9)

        while (i < len) {
            int digit = 0;
            int digit_count = 0;

            while (i < len && digit_count < 9) {
                digit = digit * 10 + (value[i] - '0');
                digit_count++;
                i++;
            }

            longObj->ob_digit[longObj->ob_size] = digit * sign;
            longObj->ob_size++;
        }
    }
    // Check if the value is a float
    else if (strspn(value, "-0123456789.") == strlen(value)) {
        obj = PyObject_New(&PyType_Float);
        obj->value.float_value = atof(value);
    }
    // Otherwise, assume it's a string
    else {
        obj = PyObject_New(&PyType_String);
        obj->value.str_value = strdup(value);
    }

    return obj;
}

int main() {
    const char* values[] = {
        "-1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111",
        "3.14",
        "2.14",
        "$11",
        "Hello, World!"
    };
    int num_values = sizeof(values) / sizeof(values[0]);

    PyObject* objects[num_values];

    // Create Python objects from the provided values
    for (int i = 0; i < num_values; i++) {
        objects[i] = PyObject_FromValue(values[i]);
    }

    // Print the type and value of each object
    for (int i = 0; i < num_values; i++) {
        PyObject* obj = objects[i];
        const char* typeName = PyObject_GetTypeName(obj);
        printf("Object %d - Type: %s, Value: ", i + 1, typeName);
        PyTypeObject* type = &PyType_Integer + obj->type;
        type->tp_print(obj);
        // print reference count 
        printf("\n");
    }

    // Free the allocated memory
    for (int i = 0; i < num_values; i++) {
        PyObject* obj = objects[i];
        PyTypeObject* type = &PyType_Integer + obj->type;
        type->tp_free(obj);
    }

    return 0;
}