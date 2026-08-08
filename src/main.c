#include <stdio.h>
#include "../include/vector.h"

static void print_vec(Vector *vec)
{
    char val;
    printf("[ ");
    for (size_t i = 0; i < vec->size; i++)
    {
        vector_get(vec, i, &val);
        printf("%c ", val);
    }
    printf("] (size=%zu, capacity=%zu)\n", vec->size, vec->capacity);
}

/* 
 * Every mutating call returns a VectorStatus - check it instead of
 * trusting the operation silently worked. 
*/
static void check(const char *what, VectorStatus status)
{
    if (status != VEC_OK)
        fprintf(stderr, "  ! %s failed: status %d\n", what, status);
}

int main(void)
{
    Vector *vec = vector_init(sizeof(char));
    if (vec == NULL)
    {
        fprintf(stderr, "vector_init failed\n");
        return 1;
    }

    char a = 'A', b = 'B', c = 'C';

    check("push_back(A)", vector_push_back(vec, &a));
    check("push_front(B)", vector_push_front(vec, &b));
    check("push_back(B)", vector_push_back(vec, &b));
    check("insert(2, C)", vector_insert(vec, 2, &c));
    printf("After pushes/insert:\n");
    print_vec(vec); /* B A C B */

    check("replace(B -> C)", vector_replace(vec, 0, vec->size - 1, &b, &c));
    printf("\nAfter replacing B with C:\n");
    print_vec(vec); /* C A C C */

    size_t pos = vector_search(vec, &c);
    VectorStatus present = vector_contains(vec, &c);
    printf("\nsearch(C) -> index %zu, contains(C) -> %s\n",
        pos, present == VEC_PRESENT ? "present" : "not present");

    /* 
     * Deliberately going out of range to show the error code in action,
     * instead of a silent no-op. 
    */
    char out;
    VectorStatus st = vector_get(vec, 99, &out);
    if (st == VEC_ERR_OUT_OF_RANGE)
        printf("\nget(99) correctly reported VEC_ERR_OUT_OF_RANGE (%d)\n", st);

    printf("\nPopping everything from the back:\n");
    while (vector_pop_back(vec, &out) == VEC_OK)
        printf("  popped %c -> ", out), print_vec(vec);

    st = vector_pop_back(vec, &out);
    if (st == VEC_ERR_EMPTY_VECTOR)
        printf("pop_back on empty vector correctly reported VEC_ERR_EMPTY_VECTOR (%d)\n", st);

    check("clear", vector_clear(vec));
    printf("\nAfter clear:\n");
    print_vec(vec);

    /* 
     * Vector must still be usable after clear() - this is the case
     * that used to crash before the capacity-doubling fix. 
    */
    check("push_back(A) after clear", vector_push_back(vec, &a));
    printf("After push_back post-clear:\n");
    print_vec(vec);

    check("destroy", vector_destroy(vec));
    return 0;
}