#include "vector.h"
#include "logger.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INITIAL_CAPACITY 4

static VectorStatus vector_input_check(size_t count, ...)
{
    va_list args;
    va_start(args, count);

    VectorStatus status = VEC_OK;

    for (size_t i = 0; i < count; i++)
    {
        /*
         * All variadic arguments must be explicitly passed as void *
         * by the callers. This keeps va_arg() type-correct.
         */
        void *ptr = va_arg(args, void *);

        if (ptr == NULL)
        {
            status = VEC_ERR_NULL_ARG;
            break;
        }
    }

    va_end(args);

    return status;
}

static VectorStatus vector_resize(Vector *vec, size_t new_capacity)
{
    if (new_capacity > SIZE_MAX / vec->elem_size)
    {
        return VEC_ERR_ALLOC;
    }

    void *new_data =
        realloc(
            vec->data,
            new_capacity * vec->elem_size
        );

    if (new_data == NULL)
    {
        LOG_ERROR("Out of memory");
        return VEC_ERR_ALLOC;
    }

    vec->data = new_data;
    vec->capacity = new_capacity;

    return VEC_OK;
}

static VectorStatus vector_grow(Vector *vec)
{
    if (vec->capacity > SIZE_MAX / 2)
    {
        return VEC_ERR_ALLOC;
    }

    size_t new_capacity =
        vec->capacity ? vec->capacity * 2 : VECTOR_INITIAL_CAPACITY;

    return vector_resize(vec, new_capacity);
}

static VectorStatus vector_shrink(Vector *vec)
{
    if (vec->capacity <= VECTOR_INITIAL_CAPACITY)
    {
        return VEC_OK;
    }

    size_t new_capacity = vec->capacity / 2;

    if (new_capacity < VECTOR_INITIAL_CAPACITY)
        new_capacity = VECTOR_INITIAL_CAPACITY;

    if (new_capacity < vec->size)
        return VEC_OK;

    return vector_resize(vec, new_capacity);
}

Vector *vector_init(const size_t elem_size)
{
    if (elem_size == 0) return NULL;

    if (elem_size > SIZE_MAX / VECTOR_INITIAL_CAPACITY) return NULL;

    Vector *vec = malloc(sizeof(Vector));

    if (vec == NULL) return NULL;

    vec->capacity = VECTOR_INITIAL_CAPACITY;
    vec->size = 0;
    vec->elem_size = elem_size;

    vec->data =
        malloc(
            vec->capacity * vec->elem_size
        );

    if (vec->data == NULL)
    {
        free(vec);
        return NULL;
    }

    return vec;
}

VectorStatus vector_push_back(Vector *vec, const void *value_ptr)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            (void *)value_ptr
        );

    if (status != VEC_OK) return status;

    if (vec->size >= vec->capacity)
    {
        status = vector_grow(vec);

        if (status != VEC_OK)
        {
            return status;
        }
    }

    char *data = (char *)vec->data;

    char *target_address = data + (vec->size++ * vec->elem_size);

    memcpy(
        target_address,
        value_ptr,
        vec->elem_size
    );

    return VEC_OK;
}

VectorStatus vector_push_front(Vector *vec, const void *value_ptr)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            (void *)value_ptr
        );

    if (status != VEC_OK)
    {
        return status;
    }

    if (vec->size >= vec->capacity)
    {
        status = vector_grow(vec);

        if (status != VEC_OK)
        {
            return status;
        }
    }

    char *data = (char *)vec->data;

    memmove(
        data + vec->elem_size,
        data,
        vec->size * vec->elem_size
    );

    memcpy(
        data,
        value_ptr,
        vec->elem_size
    );

    vec->size++;

    return VEC_OK;
}

VectorStatus vector_pop_back(Vector *vec, void *out)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            out
        );

    if (status != VEC_OK)
    {
        return status;
    }

    if (vec->size == 0)
    {
        return VEC_ERR_EMPTY_VECTOR;
    }

    char *data = (char *)vec->data;

    char *target_address =
        data + (--vec->size * vec->elem_size);

    memcpy(
        out,
        target_address,
        vec->elem_size
    );

    /*
     * Shrinking is an optimization. If realloc() fails,
     * the pop operation itself has still succeeded.
     */
    if (
        vec->size <= vec->capacity / 4 &&
        vec->capacity > VECTOR_INITIAL_CAPACITY
    )
    {
        (void)vector_shrink(vec);
    }

    return VEC_OK;
}

VectorStatus vector_pop_front(Vector *vec, void *out)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            out
        );

    if (status != VEC_OK)
    {
        return status;
    }

    if (vec->size == 0)
    {
        return VEC_ERR_EMPTY_VECTOR;
    }

    char *data = (char *)vec->data;

    memcpy(
        out,
        data,
        vec->elem_size
    );

    memmove(
        data,
        data + vec->elem_size,
        --vec->size * vec->elem_size
    );

    /*
     * Shrinking is an optimization. If realloc() fails,
     * the pop operation itself has still succeeded.
     */
    if (
        vec->size <= vec->capacity / 4 &&
        vec->capacity > VECTOR_INITIAL_CAPACITY
    )
    {
        (void)vector_shrink(vec);
    }

    return VEC_OK;
}

VectorStatus vector_insert(
    Vector *vec,
    size_t pos,
    const void *value_ptr
)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            (void *)value_ptr
        );

    if (status != VEC_OK)
    {
        return status;
    }

    if (pos > vec->size)
    {
        return VEC_ERR_OUT_OF_RANGE;
    }

    /*
     * Inserting at size is equivalent to push_back().
     */
    if (pos == vec->size) return vector_push_back(vec, value_ptr);

    if (vec->size >= vec->capacity)
    {
        status = vector_grow(vec);

        if (status != VEC_OK)
        {
            return status;
        }
    }

    char *data = (char *)vec->data;

    char *src_address = data + (pos * vec->elem_size);

    char *dest_address = src_address + vec->elem_size;

    /*
     * Move every element from pos onward one position right.
     */
    memmove(
        dest_address,
        src_address,
        (vec->size++ - pos) * vec->elem_size
    );

    memcpy(
        src_address,
        value_ptr,
        vec->elem_size
    );

    return VEC_OK;
}

VectorStatus vector_erase(Vector *vec, size_t pos)
{
    VectorStatus status =
        vector_input_check(
            1,
            (void *)vec
        );

    if (status != VEC_OK) return status;

    if (pos >= vec->size)
    {
        return VEC_ERR_OUT_OF_RANGE;
    }

    char *data = (char *)vec->data;

    char *dest_address = data + (pos * vec->elem_size);

    char *src_address = dest_address + vec->elem_size;

    /*
     * Move every element after pos one position left.
     */
    memmove(
        dest_address,
        src_address,
        (vec->size-- - pos - 1) * vec->elem_size
    );

    /*
     * Shrinking is an optimization. If realloc() fails,
     * the erase operation itself has still succeeded.
     */
    if (
        vec->size <= vec->capacity / 4 &&
        vec->capacity > VECTOR_INITIAL_CAPACITY
    )
    {
        (void)vector_shrink(vec);
    }

    return VEC_OK;
}

VectorStatus vector_replace(
    Vector *vec,
    size_t init_pos,
    size_t end_pos,
    const void *old_value_ptr,
    const void *new_value_ptr
)
{
    VectorStatus status =
        vector_input_check(
            3,
            (void *)vec,
            old_value_ptr,
            new_value_ptr
        );

    if (status != VEC_OK) return status;

    if (
        init_pos >= vec->size ||
        end_pos >= vec->size ||
        init_pos > end_pos
    )
    {
        return VEC_ERR_OUT_OF_RANGE;
    }

    for (size_t i = init_pos; i <= end_pos; i++)
    {
        char *target_address =
            (char *)vec->data +
            (i * vec->elem_size);

        if (
            memcmp(
                target_address,
                old_value_ptr,
                vec->elem_size
            ) == 0
        )
        {
            memcpy(
                target_address,
                new_value_ptr,
                vec->elem_size
            );
        }
    }

    return VEC_OK;
}

VectorStatus vector_get(
    const Vector *vec,
    size_t pos,
    void *out
)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            out
        );

    if (status != VEC_OK) return status;

    if (pos >= vec->size) return VEC_ERR_OUT_OF_RANGE;

    memcpy(
        out,
        (char *)vec->data +
            (pos * vec->elem_size),
        vec->elem_size
    );

    return VEC_OK;
}

ssize_t vector_search(const Vector *vec, const void *value_ptr)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            value_ptr
        );

    if (status != VEC_OK) return -1;

    for (size_t i = 0; i < vec->size; i++)
    {
        char *target_address =
            (char *)vec->data +
            (i * vec->elem_size);

        if (
            memcmp(
                target_address,
                value_ptr,
                vec->elem_size
            ) == 0
        )
        {
            return (ssize_t)i;
        }
    }

    return -1;
}

VectorStatus vector_contains(const Vector *vec, const void *value_ptr)
{
    VectorStatus status =
        vector_input_check(
            2,
            (void *)vec,
            value_ptr
        );

    if (status != VEC_OK)
    {
        return status;
    }

    return vector_search(vec, value_ptr) != (ssize_t)-1
        ? VEC_PRESENT
        : VEC_NOT_PRESENT;
}

VectorStatus vector_clear(Vector *vec)
{
    VectorStatus status =
        vector_input_check(
            1,
            (void *)vec
        );

    if (status != VEC_OK) return status;

    /*
     * Return the vector to its initial capacity.
     * If realloc() fails, the existing vector remains unchanged.
     */
    if (vec->capacity > VECTOR_INITIAL_CAPACITY)
    {
        status =
            vector_resize(
                vec,
                VECTOR_INITIAL_CAPACITY
            );

        if (status != VEC_OK)
        {
            return status;
        }
    }

    vec->size = 0;

    return VEC_OK;
}

VectorStatus vector_destroy(Vector *vec)
{
    if (vec == NULL)
    {
        return VEC_ERR_NULL_ARG;
    }

    free(vec->data);
    free(vec);

    return VEC_OK;
}