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
    /*
        42
        40, 42
        40, 42, 40
        40, 42, 50, 40
    */
    int val3;
    /*
    vector_erase(int_vec, 2); // removed 50
    vector_pop_back(int_vec, &val3);
    printf("%d\n", val3); //40
    vector_pop_back(int_vec, &val3);
    printf("%d\n", val3); // 42
    vector_pop_front(int_vec, &val3);
    printf("%d\n", val3); // 40
    */

    vector_replace(int_vec, 0, 3, &val2, &val4);
    printf("--- %zu ---\n", int_vec->size);
    size_t size = int_vec->size;
    int pos = (int)vector_search(int_vec, &val4);
    int contain = vector_contains(int_vec, &val4);

    printf("Index: %d, contain: %d\n", pos, contain);
    for (size_t i = 0; i < size; i++)
    {
        vector_pop_back(int_vec, &val3);
        printf("i = %zu - %d\n", i, val3);
    }
    pos = (int)vector_search(int_vec, &val4);
    contain = vector_contains(int_vec, &val4);
    printf("Index: %d, contain: %d\n", pos, contain);

    vector_clear(int_vec);
    vector_destroy(int_vec);
    
    return 0;
}