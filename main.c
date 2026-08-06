#include <stdio.h>
#include "vector.h"

int main(void)
{
    Vector v = { .data = NULL, .size = 0, .capacity = 0 };
    printf("\n=== PUSH, PUSH_FRONT, POP, POP_FRONT ===\n");
    push(&v, 2);
    push(&v, 3);
    push_front(&v, 1);

    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    pop(&v);        // popped 3
    pop_front(&v);  // popped 1

    printf("\n\n");
    if (v.size > 0) {
        printf("%d\n", v.data[v.size - 1]);  // prints 2
    }

    printf("\n=== Insert, Erase ===\n");
    insert(&v, 2, 3);
    erase(&v, 1);

    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    push(&v, 2); push(&v, 3); push_front(&v, 1);
    push(&v, 2); push(&v, 3); push_front(&v, 1);
    push(&v, 2); push(&v, 3); push_front(&v, 1);
    push(&v, 2); push(&v, 3); push_front(&v, 1);

    printf("\n\n");
    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    printf("Size Count: %zu\n", v.size);

    replace(&v, 1, v.size, 3, 10);

    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    clear(&v);

    return 0;
}
