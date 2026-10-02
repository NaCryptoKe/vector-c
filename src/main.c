#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "logger.h"

/*
 * Number of characters pushed in the smoke test. VECTOR_INITIAL_CAPACITY
 * is 4, so pushing this many exercises at least one reallocation.
 */
#define SMOKE_TEST_SIZE 6

static int pass = 0;
static int fail = 0;

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

static void print_results(void)
{
    int total = pass + fail;

    LOG("Pass: %d, Failed: %d\n", pass, fail);

    /*
     * Guard against dividing by zero when no tests were run, and use
     * floating-point division so a partial pass is reported accurately.
     */
    if (total == 0)
    {
        LOG_WARN("No tests were run\n");
        return;
    }

    LOG(
        "Pass percentage: %.1f %%\n",
        100.0 * (double)pass / (double)total
    );
}

int main(void)
{
    LOG("========================================\n");
    LOG_INFO("Vector Smoke Test\n");
    LOG("========================================\n");

    /*
     * Small test uses a character vector so that the
     * resulting contents are easy to read.
     */
    LOG("Smoke test pushes %d elements\n", SMOKE_TEST_SIZE);

    /* vector_init(0) must fail: a zero element size is rejected. */
    Vector *vec = vector_init(0);

    if (vec == NULL)
    {
        LOG_INFO("vector_init(0) correctly rejected\n");
        pass++;
    }
    else
    {
        LOG_ERROR("vector_init(0) should have returned NULL\n");
        fail++;
        vector_destroy(vec);
    }

    vec = vector_init(sizeof(char));

    if (vec == NULL)
    {
        LOG_ERROR("vector_init(sizeof(char)) failed\n");
        fail++;
    }
    else
    {
        LOG_INFO("vector_init(sizeof(char)) succeeded\n");
        pass++;

        char out = '\0';

        if (vector_get(vec, 0, &out) == VEC_ERR_OUT_OF_RANGE)
        {
            LOG_INFO("get() on empty vector correctly out of range\n");
            pass++;
        }
        else
        {
            LOG_ERROR("get() on empty vector should be out of range\n");
            fail++;
        }

        /*
         * Round-trip a short run of characters so that print_vec() is
         * exercised and the generic (void*) storage is visible.
         */
        const char alphabet[] = {'v', 'e', 'c', 't', 'o', 'r'};

        LOG("Actual:   ");
        print_vec(vec);

        for (size_t i = 0; i < sizeof alphabet; i++)
        {
            if (vector_push_back(vec, &alphabet[i]) != VEC_OK)
            {
                LOG_ERROR("push_back() failed at index %zu\n", i);
                fail++;
                break;
            }
        }

        LOG("Pushed:   ");
        print_vec(vec);

        if (vec->size == sizeof alphabet)
        {
            LOG_INFO("push_back() stored every element\n");
            pass++;
        }
        else
        {
            LOG_ERROR(
                "expected %zu elements, got %zu\n",
                sizeof alphabet,
                vec->size
            );
            fail++;
        }

        /* Confirm the stored contents survived the reallocation. */
        int contents_match = 1;

        for (size_t i = 0; i < sizeof alphabet; i++)
        {
            char stored = '\0';

            if (
                vector_get(vec, i, &stored) != VEC_OK ||
                stored != alphabet[i])
            {
                contents_match = 0;
                break;
            }
        }

        if (contents_match)
        {
            LOG_INFO("contents verified after growth\n");
            pass++;
        }
        else
        {
            LOG_ERROR("stored contents do not match what was pushed\n");
            fail++;
        }

        while (vec->size > 0)
        {
            char popped = '\0';

            if (vector_pop_back(vec, &popped) != VEC_OK)
            {
                LOG_ERROR("pop_back() failed while draining\n");
                fail++;
                break;
            }
        }

        LOG("Drained:  ");
        print_vec(vec);

        vector_destroy(vec);
    }

    print_results();

    return fail == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}