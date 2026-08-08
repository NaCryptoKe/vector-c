#include <stdio.h>
#include "../include/vector.h"

int main(void)
{
    Vector *int_vec = vector_init(sizeof(char));
    char val1 = 'A';
    char val2 = 'B';
    char val4 = 'C';
    vector_push_back(int_vec, &val1);   // size 1
    vector_push_front(int_vec, &val2);  // size 2
    vector_push_back(int_vec, &val2);   // size 3
    vector_insert(int_vec, 2, &val4);   // size 4
    /*
        A
        B, A
        B, A, B
        B, A, C, B
    */
    char val3;
    size_t size = int_vec->size;

    for (size_t i = 0; i < size; i++)
    {
        vector_get(int_vec, i, &val3);
        printf("i = %zu - %c\n", i, val3);
    }

    printf("\n\n");
    vector_replace(int_vec, 0, 3, &val2, &val4);
    // C, A, C, C
    
    printf("--- %zu ---\n", int_vec->size);
    
    int pos = (int)vector_search(int_vec, &val4);
    int contain = vector_contains(int_vec, &val4);

    printf("Index: %d, contain: %d\n", pos, contain);
    for (size_t i = 0; i < size; i++)
    {
        vector_get(int_vec, i, &val3);
        printf("i = %zu - %c\n", i, val3);
        vector_pop_back(int_vec, &val3);
    }
    pos = (int)vector_search(int_vec, &val4);
    contain = vector_contains(int_vec, &val4);
    printf("Index: %d, contain: %d\n", pos, contain);

    vector_clear(int_vec);
    vector_destroy(int_vec);
    
    return 0;
}