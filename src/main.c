#include <stdio.h>
#include "../include/vector.h"
#include "../include/logger.h"

static void print_vec(Vector *vec)
{
    char val;
    LOG("[ ");
    for (size_t i = 0; i < vec->size; i++)
    {
        vector_get(vec, i, &val);
        printf("%c ", val);
    }
    LOG("] (size=%zu, capacity=%zu)\n", vec->size, vec->capacity);
}

/* 
 * Every mutating call returns a VectorStatus - check it instead of
 * trusting the operation silently worked. 
*/
static void check(const char *what, VectorStatus status)
{
    if (status != VEC_OK)
        fprintf(stderr, "  ! %s failed: status %d\n", what, status);
    else
        LOG("%s: %d\n", what, status);
}

int main(void)
{
    LOG("========================================\n");
    Vector *vec = vector_init(sizeof(char));
    if (vec == NULL)
    {
        fprintf(stderr, "vector_init failed\n");
        return 1;
    }
    LOG_INFO("Passed Initialization phase.\n\n");
     
    char a = 'A', b = 'B', c = 'C';

    LOG("========================================\n");
    LOG_INFO("Started Pushing/Inserting phase.\n");
    check("push_back(A)", vector_push_back(vec, &a));
    print_vec(vec);
    check("push_front(B)", vector_push_front(vec, &b));
    print_vec(vec);
    check("push_back(B)", vector_push_back(vec, &b));
    print_vec(vec);
    check("insert(2, C)", vector_insert(vec, 2, &c));
    print_vec(vec);
    LOG_INFO("After pushes/insert it should be `B A C B`\n");
    print_vec(vec); // B A C B 
    LOG_INFO("Finished Pushing/Insert phase.\n\n"); 
    
    LOG("========================================\n");
    LOG_INFO("Started Replacing.\n");
    check("replace(B -> C)", vector_replace(vec, 0, vec->size - 1, &b, &c));
    LOG("After replacing B with C:\n");
    print_vec(vec); // C A C C 
    LOG_INFO("Finished Replacing.\n\n");

    LOG("========================================\n");
    LOG_INFO("Started Searching.\n");
    size_t pos = (size_t)vector_search(vec, &c);
    VectorStatus present = vector_contains(vec, &c);
    // Output should be: search(C) -> index 0, contains(C) -> present
    LOG("search(C) -> index %zu, contains(C) -> %s\n",
        pos, present == VEC_PRESENT ? "present" : "not present");
    LOG_INFO("Finished Searching.\n\n");
    
    /* 
     * Deliberately going out of range to show the error code in action,
     * instead of a silent no-op.
     */ 
    LOG("========================================\n");
    LOG_INFO("Started Breaking the system.\n");
    char out;
    VectorStatus st = vector_get(vec, 99, &out);
    if (st == VEC_ERR_OUT_OF_RANGE)
        LOG("get(99) correctly reported VEC_ERR_OUT_OF_RANGE (%d)\n", st);
    else
        LOG("get(99) failed to reported VEC_ERR_OUT_OF_RANGE (%d)\n", st);

    LOG("========================================\n");
    LOG("Popping everything from the back:\n");
    while(vector_pop_back(vec, &out) == VEC_OK)
        LOG("\tpopped %c -> ", out), print_vec(vec);

    st = vector_pop_back(vec, &out);
    if (st == VEC_ERR_EMPTY_VECTOR)
        LOG("pop_back on empty vector correctly reported VEC_ERR_EMPTY_VECTOR (%d)\n", st);
    LOG("Finished Popping everything from the back\n\n");

    LOG("========================================\n");
    LOG("Started Clearing\n");
    check("clear", vector_clear(vec));
    printf("After clear: ");
    print_vec(vec);

    /* 
     * Vector must still be usable after clear() - this is the case
     * that used to crash before the capacity-doubling fix. 
    */
    LOG("Started Use After Clear\n");
    check("push_back(A) after clear", vector_push_back(vec, &a));
    // printf("After push_back post-clear:\n");
    // print_vec(vec);

    // check("destroy", vector_destroy(vec));
    
    return 0;
}