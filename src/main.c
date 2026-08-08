#include <stdio.h>
#include "../include/vector.h"

int main(void)
{
    Vector *int_vec = vector_init(sizeof(int));
    int val1 = 42;
    int val2 = 40;
    vector_push_back(int_vec, &val1);
    vector_push_front(int_vec, &val2);
    vector_push_back(int_vec, &val2);

    int val3;
    vector_pop_back(int_vec, &val3);
    printf("%d\n", val3);
    vector_pop_front(int_vec, &val3);
    printf("%d\n", val3);
    
    return 0;
}