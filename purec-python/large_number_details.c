#include <Python.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGITS_TO_PRINT 140
#define SCREEN_WIDTH 140
#define BOX_WIDTH 32

void print_separator(char c, int length) {
    for (int i = 0; i < length; i++) {
        putchar(c);
    }
    putchar('\n');
}

void draw_box(const char *label, const char *value) {
    printf("+------------------------------+\n");
    printf("| %-28s |\n", label);
    printf("| %-28s |\n", value);
    printf("+------------------------------+\n");
}

void draw_four_boxes(const char *label1, const char *value1, const char *label2, const char *value2, 
                     const char *label3, const char *value3, const char *label4, const char *value4) {
    printf("+------------------------------+------------------------------+------------------------------+------------------------------+\n");
    printf("| %-28s | %-28s | %-28s | %-28s |\n", label1, label2, label3, label4);
    printf("| %-28s | %-28s | %-28s | %-28s |\n", value1, value2, value3, value4);
    printf("+------------------------------+------------------------------+------------------------------+------------------------------+\n");
}

void print_large_number_details(PyObject *pNum, const char *label) {
    if (!PyLong_Check(pNum)) {
        printf("Provided object is not a Python long integer.\n");
        return;
    }

    PyLongObject *v = (PyLongObject *)pNum;
    PyObject *obj = (PyObject *)v;
    size_t bits = _PyLong_NumBits(pNum);
    Py_ssize_t size = Py_ABS(Py_SIZE(v));

    print_separator('=', SCREEN_WIDTH);
    printf("%s at %p:\n", label, (void *)v);
    print_separator('-', SCREEN_WIDTH);

    // PyObject_HEAD and PyVarObject_HEAD representation
    char buffer1[64], buffer2[64], buffer3[64], buffer4[64];
    snprintf(buffer1, sizeof(buffer1), "%zd", Py_REFCNT(obj));
    snprintf(buffer2, sizeof(buffer2), "%p", (void *)Py_TYPE(obj));
    snprintf(buffer3, sizeof(buffer3), "%s", Py_TYPE(obj)->tp_name);
    snprintf(buffer4, sizeof(buffer4), "%zd", Py_SIZE(v));
    draw_four_boxes("ob_refcnt", buffer1, "ob_type", buffer2, "type_name", buffer3, "ob_size", buffer4);

    // ob_digit representation
    printf("\nob_digit:\n");
    for (Py_ssize_t i = 0; i < size && i < MAX_DIGITS_TO_PRINT; i += 4) {
        char label1[64], value1[64], label2[64], value2[64], label3[64], value3[64], label4[64], value4[64];
        snprintf(label1, sizeof(label1), "[%zd]", i);
        snprintf(value1, sizeof(value1), "%p: %u", (void *)&v->ob_digit[i], v->ob_digit[i]);
        
        if (i + 1 < size && i + 1 < MAX_DIGITS_TO_PRINT) {
            snprintf(label2, sizeof(label2), "[%zd]", i + 1);
            snprintf(value2, sizeof(value2), "%p: %u", (void *)&v->ob_digit[i + 1], v->ob_digit[i + 1]);
        } else {
            snprintf(label2, sizeof(label2), " ");
            snprintf(value2, sizeof(value2), " ");
        }
        
        if (i + 2 < size && i + 2 < MAX_DIGITS_TO_PRINT) {
            snprintf(label3, sizeof(label3), "[%zd]", i + 2);
            snprintf(value3, sizeof(value3), "%p: %u", (void *)&v->ob_digit[i + 2], v->ob_digit[i + 2]);
        } else {
            snprintf(label3, sizeof(label3), " ");
            snprintf(value3, sizeof(value3), " ");
        }
        
        if (i + 3 < size && i + 3 < MAX_DIGITS_TO_PRINT) {
            snprintf(label4, sizeof(label4), "[%zd]", i + 3);
            snprintf(value4, sizeof(value4), "%p: %u", (void *)&v->ob_digit[i + 3], v->ob_digit[i + 3]);
        } else {
            snprintf(label4, sizeof(label4), " ");
            snprintf(value4, sizeof(value4), " ");
        }
        
        draw_four_boxes(label1, value1, label2, value2, label3, value3, label4, value4);
    }
    if (size > MAX_DIGITS_TO_PRINT) {
        printf("... (truncated, %zd more digits)\n", size - MAX_DIGITS_TO_PRINT);
    }

    // Number properties
    print_separator('-', SCREEN_WIDTH);
    printf("Number Properties:\n");
    snprintf(buffer1, sizeof(buffer1), "%s", v->ob_base.ob_size >= 0 ? "Positive" : "Negative");
    snprintf(buffer2, sizeof(buffer2), "%zu", bits);
    snprintf(buffer3, sizeof(buffer3), "%zd", size);
    snprintf(buffer4, sizeof(buffer4), " ");
    draw_four_boxes("Sign", buffer1, "Bits", buffer2, "Base 2^30 digits", buffer3, " ", " ");
    
    // Decimal representation
    PyObject *str = PyObject_Str(pNum);
    if (str != NULL) {
        const char *number_str = PyUnicode_AsUTF8(str);
        if (number_str != NULL) {
            size_t str_len = strlen(number_str);
            printf("\nDecimal Representation:\n");
            for (size_t i = 0; i < str_len; i++) {
                printf("%c", number_str[i]);
                if ((str_len - i - 1) % 3 == 0 && i != str_len - 1) {
                    printf(",");
                }
            }
            printf(" (%zu digits)\n", str_len);
        }
        Py_DECREF(str);
    }
    print_separator('=', SCREEN_WIDTH);
}

void illustrate_addition(PyObject *num1, PyObject *num2) {
    PyLongObject *v1 = (PyLongObject *)num1;
    PyLongObject *v2 = (PyLongObject *)num2;
    
    Py_ssize_t size1 = Py_ABS(Py_SIZE(v1));
    Py_ssize_t size2 = Py_ABS(Py_SIZE(v2));
    Py_ssize_t max_size = (size1 > size2) ? size1 : size2;

    print_separator('=', SCREEN_WIDTH);
    printf("Addition Process (base 2^30 chunks)\n");
    print_separator('-', SCREEN_WIDTH);

    digit carry = 0;
    PyObject *result = PyLong_FromLong(0);

    for (Py_ssize_t i = 0; i < max_size || carry; i++) {
        digit d1 = (i < size1) ? v1->ob_digit[i] : 0;
        digit d2 = (i < size2) ? v2->ob_digit[i] : 0;

        twodigits sum = (twodigits)d1 + d2 + carry;
        digit new_digit = (digit)(sum & PyLong_MASK);
        carry = (digit)(sum >> PyLong_SHIFT);

        char label1[64], value1[64], label2[64], value2[64], label3[64], value3[64], label4[64], value4[64];
        snprintf(label1, sizeof(label1), "Step %zd", i + 1);
        snprintf(value1, sizeof(value1), "%u+%u+%u carry", d1, d2, carry);
        snprintf(label2, sizeof(label2), "Result");
        snprintf(value2, sizeof(value2), "sum: %u", (unsigned int)sum);
        snprintf(label3, sizeof(label3), "New digit");
        snprintf(value3, sizeof(value3), "%u", new_digit);
        snprintf(label4, sizeof(label4), "New carry");
        snprintf(value4, sizeof(value4), "%u", carry);
        draw_four_boxes(label1, value1, label2, value2, label3, value3, label4, value4);

        PyObject *temp = PyLong_FromLong(new_digit);
        PyObject *shifted = PyNumber_Lshift(temp, PyLong_FromLong(i * PyLong_SHIFT));
        PyObject *new_result = PyNumber_Add(result, shifted);
        Py_DECREF(result);
        result = new_result;
        Py_DECREF(temp);
        Py_DECREF(shifted);
    }

    print_separator('-', SCREEN_WIDTH);
    print_large_number_details(result, "Final Result");
    Py_DECREF(result);
}

int main() {
    Py_Initialize();

    PyObject *pNum1 = PyLong_FromString("987654321098765432109876543210987", NULL, 10);
    PyObject *pNum2 = PyLong_FromString("12345678901234567", NULL, 10);

    if (pNum1 == NULL || pNum2 == NULL) {
        printf("Failed to create PyLong objects.\n");
        Py_Finalize();
        return 1;
    }

    print_large_number_details(pNum1, "Number 1");
    print_large_number_details(pNum2, "Number 2");
    illustrate_addition(pNum1, pNum2);

    Py_DECREF(pNum1);
    Py_DECREF(pNum2);
    Py_Finalize();
    return 0;
}