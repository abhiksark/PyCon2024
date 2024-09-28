#include "add.h"

int add(int a)
{
    int sum = 0;
    for (int i = 0; i < a; i++)
    {
        sum += i;
    }
    return sum;
}