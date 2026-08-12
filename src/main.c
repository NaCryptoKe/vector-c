#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "logger.h"

#define STRESS_TEST_SIZE 10

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

static void print_results()
{
    LOG("Pass: %d, Failed: %d\n", pass, fail);
    LOG("Pass percentage: %d %%\n", (pass / (pass + fail)) * 100);
}

int main(void)
{
    LOG("========================================\n");
    LOG_INFO("Vector Test Suite\n");
    LOG("========================================\n");
    LOG("Stress test size: %d elements\n", STRESS_TEST_SIZE);

    /*
     * Small tests use a character vector so that the
     * resulting contents are easy to read.
     */
    Vector *vec = vector_init(0);
    char array[STRESS_TEST_SIZE];

    // Testing for safe guard of vector_init()
    if (vec == NULL)
    {
        LOG_INFO("vector_init() passed\n");
        pass++;
    }
    else
    {
        LOG_ERROR("vector_init() failed\n");
        fail++;
    }

    vec = vector_init(sizeof(char));
    if (vec != NULL)
    {
        LOG_INFO("vector_init() passed\n");
        pass++;
    }
    else
    {
        LOG_ERROR("vector_init() failed\n");
        fail++;
    }

    print_results();
    return 0;
}