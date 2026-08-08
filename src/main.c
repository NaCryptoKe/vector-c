#include <stdio.h>
#include "../include/vector.h"

int main(void)
{
    Vector *int_vec = vector_init(sizeof(int));
    int val1 = 42;
    int val2 = 40;
    int val4 = 50;
    vector_push_back(int_vec, &val1);   // size 1
    vector_push_front(int_vec, &val2);  // size 2
    vector_push_back(int_vec, &val2);   // size 3
    vector_insert(int_vec, 2, &val4);   // size 4

    vector_erase(int_vec, 2); // removed 50
    int val3;
    vector_pop_back(int_vec, &val3);
    printf("%d\n", val3); //40
    vector_pop_back(int_vec, &val3);
    printf("%d\n", val3); // 42
    vector_pop_front(int_vec, &val3);
    printf("%d\n", val3); // 40
    
    return 0;
}