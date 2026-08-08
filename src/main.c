#include <stdio.h>
#include "../include/vector.h"

int main(void)
{
    Vector *int_vec = vector_init(sizeof(int));
    int val1 = 42;
    vector_push_back(int_vec, &val1);
    vector_push_back(int_vec, &val1);
    vector_push_back(int_vec, &val1);
    vector_push_back(int_vec, &val1);
    vector_push_back(int_vec, &val1);

    return 0;
}