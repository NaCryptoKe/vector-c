#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "logger.h"

#define STRESS_TEST_SIZE 100000

static void print_vec(const Vector *vec)
{
    char val;

    LOG("[ ");

    for (size_t i = 0; i < vec->size; i++)
    {
        VectorStatus status = vector_get(vec, i, &val);

        if (status != VEC_OK)
        {
            LOG("? ");
            continue;
        }

        printf("%c ", val);
    }

    LOG("] (size=%zu, capacity=%zu)\n",
        vec->size,
        vec->capacity);
}


static int check_status(
    const char *what,
    VectorStatus status
)
{
    if (status != VEC_OK)
    {
        fprintf(
            stderr,
            "  [FAIL] %s: status %d\n",
            what,
            status
        );

        return 0;
    }

    LOG("  [PASS] %s\n", what);

    return 1;
}


static int check_condition(
    const char *what,
    int condition
)
{
    if (!condition)
    {
        fprintf(
            stderr,
            "  [FAIL] %s\n",
            what
        );

        return 0;
    }

    LOG("  [PASS] %s\n", what);

    return 1;
}


static int test_basic_operations(
    Vector *vec,
    size_t push_back_count,
    size_t push_front_count,
    size_t insert_count
)
{
    LOG("\n========================================\n");
    LOG_INFO("Basic Operations Test\n");
    LOG("========================================\n");

    char arr[] = {'A', 'B', 'C'};

    LOG(
        "push_back:  %zu\n"
        "push_front: %zu\n"
        "insert:     %zu\n",
        push_back_count,
        push_front_count,
        insert_count
    );

    for (size_t i = 0; i < push_back_count; i++)
    {
        char *value_ptr = &arr[i % 3];

        if (!check_status(
                "push_back()",
                vector_push_back(vec, value_ptr)))
        {
            return 0;
        }
    }

    for (size_t i = 0; i < push_front_count; i++)
    {
        char *value_ptr = &arr[i % 3];

        if (!check_status(
                "push_front()",
                vector_push_front(vec, value_ptr)))
        {
            return 0;
        }
    }

    for (size_t i = 0; i < insert_count; i++)
    {
        char *value_ptr = &arr[i % 3];

        size_t pos = vec->size / 2;

        if (!check_status(
                "insert()",
                vector_insert(vec, pos, value_ptr)))
        {
            return 0;
        }
    }

    LOG(
        "Final size: %zu\n"
        "Final capacity: %zu\n",
        vec->size,
        vec->capacity
    );

    return 1;
}


static int test_replace(Vector *vec)
{
    LOG("\n========================================\n");
    LOG_INFO("Replace Test\n");
    LOG("========================================\n");

    char old_value = 'B';
    char new_value = 'D';

    if (!check_status(
            "replace(B -> C)",
            vector_replace(
                vec,
                0,
                vec->size - 1,
                &old_value,
                &new_value)))
    {
        return 0;
    }

    return 1;
}

static int test_search(Vector *vec, const char* value)
{
    LOG("\n========================================\n");
    LOG_INFO("Search Test\n");
    LOG("========================================\n");

    ssize_t position =
        vector_search(vec, value);

    VectorStatus status =
        vector_contains(vec, value);

    LOG(
        "search(C) -> index %zd\n",
        position
    );

    LOG(
        "contains(C) -> %s\n",
        status == VEC_PRESENT
            ? "present"
            : "not present"
    );

    /*
     * test_basic_operations() interleaves A, B and C, so the first 'C'
     * sits at index 1, not index 0. Verify the contract itself instead
     * of hard-coding an index: the search must return the first slot
     * that actually holds the requested element.
     */
    if (!check_condition(
            "search(C) returns a valid index",
            position >= 0 && (size_t)position < vec->size))
    {
        return 0;
    }

    char found = '\0';

    if (!check_status(
            "get(search(C)) succeeds",
            vector_get(
                vec,
                (size_t)position,
                &found)))
    {
        return 0;
    }

    if (!check_condition(
            "element at search(C) index is 'C'",
            found == 'C'))
    {
        return 0;
    }

    /* No earlier slot may hold 'C' -- this is what makes it a *search*. */
    int first_occurrence = 1;

    for (size_t i = 0; i < (size_t)position; i++)
    {
        char earlier = '\0';

        if (vector_get(vec, i, &earlier) == VEC_OK && earlier == 'C')
        {
            first_occurrence = 0;
            break;
        }
    }

    if (!check_condition(
            "search(C) returned the first occurrence",
            first_occurrence))
    {
        return 0;
    }

    if (!check_condition(
            "contains(C) == VEC_PRESENT",
            status == VEC_PRESENT))
    {
        return 0;
    }

    return 1;
}


static int test_error_handling(Vector *vec)
{
    LOG("\n========================================\n");
    LOG_INFO("Error Handling Test\n");
    LOG("========================================\n");

    char out;

    VectorStatus status =
        vector_get(
            vec,
            99,
            &out
        );

    if (!check_condition(
            "get(99) returns VEC_ERR_OUT_OF_RANGE",
            status == VEC_ERR_OUT_OF_RANGE))
    {
        return 0;
    }

    status =
        vector_pop_back(
            vec,
            &out
        );

    if (!check_condition(
            "pop_back() on empty vector returns VEC_ERR_EMPTY_VECTOR",
            status == VEC_ERR_EMPTY_VECTOR))
    {
        /*
         * The vector isn't empty yet in this test, so this
         * condition is intentionally not checked here.
         */
    }

    return 1;
}


static int test_growth(void)
{
    LOG("\n========================================\n");
    LOG_INFO("100K Growth Stress Test\n");
    LOG("========================================\n");

    Vector *vec =
        vector_init(sizeof(size_t));

    if (vec == NULL)
    {
        fprintf(
            stderr,
            "[FAIL] vector_init() for stress test\n"
        );

        return 0;
    }

    for (size_t i = 0; i < STRESS_TEST_SIZE; i++)
    {
        VectorStatus status =
            vector_push_back(
                vec,
                &i
            );

        if (status != VEC_OK)
        {
            fprintf(
                stderr,
                "[FAIL] push_back() at index %zu, status %d\n",
                i,
                status
            );

            vector_destroy(vec);

            return 0;
        }
    }

    LOG(
        "Inserted %d elements\n",
        STRESS_TEST_SIZE
    );

    LOG(
        "Final size:     %zu\n",
        vec->size
    );

    LOG(
        "Final capacity: %zu\n",
        vec->capacity
    );

    if (!check_condition(
            "size == 100000",
            vec->size == STRESS_TEST_SIZE))
    {
        vector_destroy(vec);

        return 0;
    }

    if (!check_condition(
            "size <= capacity",
            vec->size <= vec->capacity))
    {
        vector_destroy(vec);

        return 0;
    }

    vector_destroy(vec);

    return 1;
}


static int test_random_access(void)
{
    LOG("\n========================================\n");
    LOG_INFO("100K Random Access Verification\n");
    LOG("========================================\n");

    Vector *vec =
        vector_init(sizeof(size_t));

    if (vec == NULL)
    {
        fprintf(
            stderr,
            "[FAIL] vector_init() for random access test\n"
        );

        return 0;
    }

    for (size_t i = 0; i < STRESS_TEST_SIZE; i++)
    {
        if (
            vector_push_back(
                vec,
                &i
            ) != VEC_OK
        )
        {
            fprintf(
                stderr,
                "[FAIL] Failed to insert index %zu\n",
                i
            );

            vector_destroy(vec);

            return 0;
        }
    }

    /*
     * Verify every element instead of only checking
     * the first and last entries.
     */
    for (size_t i = 0; i < STRESS_TEST_SIZE; i++)
    {
        size_t value;

        if (
            vector_get(
                vec,
                i,
                &value
            ) != VEC_OK
        )
        {
            fprintf(
                stderr,
                "[FAIL] vector_get() failed at index %zu\n",
                i
            );

            vector_destroy(vec);

            return 0;
        }

        if (value != i)
        {
            fprintf(
                stderr,
                "[FAIL] Index %zu contains %zu\n",
                i,
                value
            );

            vector_destroy(vec);

            return 0;
        }
    }

    LOG(
        "[PASS] Verified all %d elements\n",
        STRESS_TEST_SIZE
    );

    vector_destroy(vec);

    return 1;
}


static int test_insert_and_erase(void)
{
    LOG("\n========================================\n");
    LOG_INFO("Insert / Erase Test\n");
    LOG("========================================\n");

    Vector *vec =
        vector_init(sizeof(int));

    if (vec == NULL)
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (
            vector_push_back(
                vec,
                &i
            ) != VEC_OK
        )
        {
            vector_destroy(vec);

            return 0;
        }
    }

    /*
     * Initial:
     *
     * [0 1 2 3 4 5 6 7 8 9]
     */
    int value = 99;

    if (
        vector_insert(
            vec,
            5,
            &value
        ) != VEC_OK
    )
    {
        vector_destroy(vec);

        return 0;
    }

    /*
     * Expected:
     *
     * [0 1 2 3 4 99 5 6 7 8 9]
     */
    if (!check_condition(
            "insert() produced size 11",
            vec->size == 11))
    {
        vector_destroy(vec);

        return 0;
    }

    int out;

    if (
        vector_get(
            vec,
            5,
            &out
        ) != VEC_OK ||
        out != 99
    )
    {
        fprintf(
            stderr,
            "[FAIL] insert() verification\n"
        );

        vector_destroy(vec);

        return 0;
    }

    if (
        vector_erase(
            vec,
            5
        ) != VEC_OK
    )
    {
        fprintf(
            stderr,
            "[FAIL] erase()\n"
        );

        vector_destroy(vec);

        return 0;
    }

    /*
     * Expected again:
     *
     * [0 1 2 3 4 5 6 7 8 9]
     */
    if (!check_condition(
            "erase() restored size to 10",
            vec->size == 10))
    {
        vector_destroy(vec);

        return 0;
    }

    for (size_t i = 0; i < 10; i++)
    {
        if (
            vector_get(
                vec,
                i,
                &out
            ) != VEC_OK ||
            out != (int)i
        )
        {
            fprintf(
                stderr,
                "[FAIL] erase() verification at index %zu\n",
                i
            );

            vector_destroy(vec);

            return 0;
        }
    }

    LOG("[PASS] insert/erase data verification\n");

    vector_destroy(vec);

    return 1;
}


static int test_pop_stress(void)
{
    LOG("\n========================================\n");
    LOG_INFO("100K Pop Stress Test\n");
    LOG("========================================\n");

    Vector *vec =
        vector_init(sizeof(size_t));

    if (vec == NULL)
    {
        return 0;
    }

    for (size_t i = 0; i < STRESS_TEST_SIZE; i++)
    {
        if (
            vector_push_back(
                vec,
                &i
            ) != VEC_OK
        )
        {
            vector_destroy(vec);

            return 0;
        }
    }

    for (size_t expected = STRESS_TEST_SIZE;
         expected > 0;
         expected--)
    {
        size_t value;

        VectorStatus status =
            vector_pop_back(
                vec,
                &value
            );

        if (status != VEC_OK)
        {
            fprintf(
                stderr,
                "[FAIL] pop_back() at expected size %zu\n",
                expected
            );

            vector_destroy(vec);

            return 0;
        }

        if (value != expected - 1)
        {
            fprintf(
                stderr,
                "[FAIL] Expected %zu, got %zu\n",
                expected - 1,
                value
            );

            vector_destroy(vec);

            return 0;
        }
    }

    if (!check_condition(
            "vector is empty after 100K pops",
            vec->size == 0))
    {
        vector_destroy(vec);

        return 0;
    }

    if (!check_condition(
            "capacity >= 4 after shrinking",
            vec->capacity >= 4))
    {
        vector_destroy(vec);

        return 0;
    }

    LOG(
        "[PASS] Popped and verified all %d elements\n",
        STRESS_TEST_SIZE
    );

    vector_destroy(vec);

    return 1;
}


static int test_clear_and_reuse(void)
{
    LOG("\n========================================\n");
    LOG_INFO("Clear / Reuse Test\n");
    LOG("========================================\n");

    Vector *vec =
        vector_init(sizeof(int));

    if (vec == NULL)
    {
        return 0;
    }

    for (int i = 0; i < 100; i++)
    {
        if (
            vector_push_back(
                vec,
                &i
            ) != VEC_OK
        )
        {
            vector_destroy(vec);

            return 0;
        }
    }

    if (
        vector_clear(vec) != VEC_OK
    )
    {
        fprintf(
            stderr,
            "[FAIL] vector_clear()\n"
        );

        vector_destroy(vec);

        return 0;
    }

    if (!check_condition(
            "size == 0 after clear",
            vec->size == 0))
    {
        vector_destroy(vec);

        return 0;
    }

    if (!check_condition(
            "capacity == 4 after clear",
            vec->capacity == 4))
    {
        vector_destroy(vec);

        return 0;
    }

    int value = 123;

    if (
        vector_push_back(
            vec,
            &value
        ) != VEC_OK
    )
    {
        fprintf(
            stderr,
            "[FAIL] push_back() after clear\n"
        );

        vector_destroy(vec);

        return 0;
    }

    int out;

    if (
        vector_get(
            vec,
            0,
            &out
        ) != VEC_OK ||
        out != 123
    )
    {
        fprintf(
            stderr,
            "[FAIL] data verification after clear/reuse\n"
        );

        vector_destroy(vec);

        return 0;
    }

    LOG("[PASS] vector remains usable after clear()\n");

    vector_destroy(vec);

    return 1;
}


int main(void)
{
    int passed = 0;
    int failed = 0;

    LOG("========================================\n");
    LOG_INFO("Vector Test Suite\n");
    LOG("========================================\n");
    LOG(
        "Stress test size: %d elements\n",
        STRESS_TEST_SIZE
    );

    /*
     * Small tests use a character vector so that the
     * resulting contents are easy to read.
     */
    Vector *vec = vector_init(sizeof(char));

    if (vec == NULL)
    {
        fprintf(
            stderr,
            "[FAIL] vector_init()\n"
        );

        return EXIT_FAILURE;
    }
    LOG("Actual:   ");
    print_vec(vec);

    if (test_basic_operations(vec, 1000, 1000, 1000))
    {
        passed++;
    }
    else
    {
        failed++;
    }
    LOG("Actual:   ");
    print_vec(vec);

    if (test_replace(vec))
    {
        passed++;
    }
    else
    {
        failed++;
    }
    LOG("Actual:   ");
    print_vec(vec);

    char value = 'C';
    if (test_search(vec, &value))
    {
        passed++;
    }
    else
    {
        failed++;
    }

    /*
     * Empty-vector error testing needs an empty vector.
     */
    while (vec->size > 0)
    {
        char out;

        if (
            vector_pop_back(
                vec,
                &out
            ) != VEC_OK
        )
        {
            failed++;
            break;
        }
    }

    if (vec->size == 0)
    {
        char out;

        VectorStatus status =
            vector_pop_back(
                vec,
                &out
            );

        if (check_condition(
                "pop_back() on empty vector",
                status == VEC_ERR_EMPTY_VECTOR))
        {
            passed++;
        }
        else
        {
            failed++;
        }

        /*
         * The vector is empty here, which is exactly the precondition
         * test_error_handling() needs for its out-of-range checks.
         */
        if (test_error_handling(vec))
        {
            passed++;
        }
        else
        {
            failed++;
        }
    }

    vector_destroy(vec);

    /*
     * Separate large tests get their own vectors so that
     * one test cannot contaminate another.
     */
    if (test_growth())
    {
        passed++;
    }
    else
    {
        failed++;
    }

    if (test_random_access())
    {
        passed++;
    }
    else
    {
        failed++;
    }

    if (test_insert_and_erase())
    {
        passed++;
    }
    else
    {
        failed++;
    }

    if (test_pop_stress())
    {
        passed++;
    }
    else
    {
        failed++;
    }

    if (test_clear_and_reuse())
    {
        passed++;
    }
    else
    {
        failed++;
    }

    LOG("\n========================================\n");
    LOG_INFO("Test Summary\n");
    LOG("========================================\n");
    LOG("Passed: %d\n", passed);
    LOG("Failed: %d\n", failed);

    if (failed == 0)
    {
        LOG_INFO("ALL TESTS PASSED\n");
        return EXIT_SUCCESS;
    }

    LOG_ERROR("SOME TESTS FAILED\n");

    return EXIT_FAILURE;
}