#include "../include/vector.h"
#include "../include/logger.h"
#include <stdio.h>

#include <stdlib.h> // for realloc and free
#include <string.h> // for memmove
#include <stdarg.h> // for variadic function

VectorStatus vector_input_check(size_t count, ...)
{
    va_list args;
    va_start(args, count);

    VectorStatus status = VEC_OK;

    for (size_t i = 0; i < count; i++)
    {
        if (va_arg(args, void*) == NULL)
        {
            status = VEC_ERR_NULL_ARG;
        }
    }

    va_end(args);
    return status;
}

Vector* vector_init(const size_t elem_size)
{
    if (elem_size == 0) return NULL;

    // Initialize the vector
    Vector* vec = (Vector *)malloc(sizeof(Vector));

    // Checking if input element size is valid
    if (vector_input_check(1, vec) != VEC_OK)
    {
        free(vec);
        return NULL;
    }

    // default starting point
    vec->capacity = 4;
    vec->size = 0;
    vec->elem_size = elem_size;

    vec->data = malloc(vec->capacity * vec->elem_size);
    if (vector_input_check(1, vec->data) != VEC_OK)
    {
        free(vec);
        return NULL;
    }

    return vec;
}

VectorStatus vector_push_back(Vector *vec, const void *value_ptr)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(1, value_ptr);
    if (status != VEC_OK) return status;

    // If size has reached capacity, increase
    if (vec->size >= vec->capacity)
    {
        vec->capacity = vec->capacity ? vec->capacity * 2 : 4;

        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);

        if (new_data == NULL)   // Failed to get memory
        {
            LOG_ERROR("Out of memory");
            return VEC_ERR_ALLOC;
        }

        vec->data = new_data;
    }

    char *target_address = (char *)vec->data + (vec->size++ * vec->elem_size);
    memcpy(target_address, value_ptr, vec->elem_size);

    return VEC_OK;
}

VectorStatus vector_push_front(Vector *vec, const void *value_ptr)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(2, vec, value_ptr);
    if (status != VEC_OK) return status;

    // If size has reached capacity, increase
    if (vec->size >= vec->capacity)
    {
        vec->capacity = vec->capacity ? vec->capacity * 2 : 4;

        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);

        if (new_data == NULL)
        {
            LOG_ERROR("Out of memory");
            return VEC_ERR_ALLOC;
        }

        vec->data = new_data;
    }

    char *dest_address = (char *)vec->data + (vec->elem_size);
    char *src_address = (char *)vec->data;
    
    // Moving memory and putting data to the index 0 position
    memmove(dest_address, src_address, vec->size++ * vec->elem_size);
    memcpy(src_address, value_ptr, vec->elem_size);

    return VEC_OK;
}

VectorStatus vector_pop_back(Vector *vec, void *out)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(2, vec, out);
    if (status != VEC_OK) return status;

    if (vec->size == 0) return VEC_ERR_EMPTY_VECTOR;

    char *target_address = (char *)vec->data + (--vec->size * vec->elem_size);
    memcpy(out, target_address, vec->elem_size);
    
    // If size has reached capacity's quarter
    // and capacity hasn't went below initial value 4, decrease
    if (
        vec->size <= vec->capacity / 4 &&
        vec->capacity > 4
    )
    {
        size_t new_capacity = vec->capacity / 2;

        void *new_data = realloc(vec->data, new_capacity * vec->elem_size);
        
        if (new_data == NULL)
        {
            LOG_ERROR("Out of memory");
            vec->size++;
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
        vec->capacity = new_capacity;
    }

    return VEC_OK;
}

VectorStatus vector_pop_front(Vector *vec, void *out)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(2, vec, out);
    if (status != VEC_OK)
        return status;

    if (vec->size == 0)
        return VEC_ERR_EMPTY_VECTOR;

    char *data = (char *)vec->data;

    // Copy the first element to out
    memcpy(out, data, vec->elem_size);

    // Remove the first element logically
    vec->size--;

    // Shift remaining elements one position to the left
    memmove(
        data,
        data + vec->elem_size,
        vec->size * vec->elem_size
    );

    // If size has reached capacity's quarter
    // and capacity hasn't gone below initial value 4, decrease
    if (
        vec->size <= vec->capacity / 4 &&
        vec->capacity > 4
    )
    {
        size_t new_capacity = vec->capacity / 2;

        void *new_data = realloc( vec->data, new_capacity * vec->elem_size);

        if (new_data == NULL)
        {
            LOG_ERROR("Out of memory");
            vec->size++;
            return VEC_ERR_ALLOC;
        }

        vec->data = new_data;
        vec->capacity = new_capacity;
    }

    return VEC_OK;
}

VectorStatus vector_insert(Vector *vec, size_t pos, const void *value_ptr)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(2, vec, value_ptr);
    if (status != VEC_OK) return status;

    if (pos > vec->size) return VEC_ERR_OUT_OF_RANGE;

    // pos = vec->size is just a basic push back
    if (pos == vec->size)
    {
        return vector_push_back(vec, value_ptr);
    }

    if (vec->size >= vec->capacity)
    {
        vec->capacity = vec->capacity ? vec->capacity * 2 : 4;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            LOG_ERROR("Out of memory");
            return VEC_ERR_ALLOC;
        }

        vec->data = new_data;
    }

    char *src_address = (char *)vec->data + (pos * vec->elem_size);
    char *dest_address = (char *)vec->data + (pos * vec->elem_size) + vec->elem_size;
    memmove(dest_address, src_address, vec->size++ * vec->elem_size);
    memcpy(src_address, value_ptr, vec->elem_size);

    return VEC_OK;
}

VectorStatus vector_erase(Vector *vec, size_t pos)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(1, vec);
    if (status != VEC_OK) return status;

    if (pos >= vec->size) return VEC_ERR_OUT_OF_RANGE;

    char *dest_address = (char *)vec->data + (pos * vec->elem_size);
    char *src_address = (char *)vec->data + (pos * vec->elem_size) + vec->elem_size;
    memmove(dest_address, src_address,(vec->size-- - pos) * vec->elem_size);

    if (
        vec->size <= vec->capacity/4 && 
        vec->capacity > 4
    )
    {
        vec->capacity /= 2;

        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            LOG_ERROR("Out of memory");
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
    }

    return VEC_OK;
}

VectorStatus vector_replace(Vector *vec, size_t init_pos, size_t end_pos, 
                            void *old_value_ptr, void *new_value_ptr)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(3, vec, old_value_ptr, new_value_ptr);
    if (status != VEC_OK) return status;

    if (
        init_pos >= vec->size ||
        end_pos >= vec->size ||
        init_pos > end_pos
    )
        return VEC_ERR_OUT_OF_RANGE;

    for (size_t i = init_pos; i <= end_pos; i++) {
        char *target_address = (char *)vec->data + (i * vec->elem_size);

        if (memcmp(target_address, old_value_ptr, vec->elem_size) == 0)
        {
            memcpy(target_address, new_value_ptr, vec->elem_size);
        }
    }

    return VEC_OK;
}

VectorStatus vector_get(Vector *vec, size_t pos, void *out)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(2, vec, out);
    if (status != VEC_OK) return status;

    if (pos >= vec->size) return VEC_ERR_OUT_OF_RANGE;

    memcpy(out, (char *)vec->data + (pos * vec->elem_size), vec->elem_size);

    return VEC_OK;
}

ssize_t vector_search(Vector *vec, void *value_ptr)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(2, vec, value_ptr);
    if (status != VEC_OK) return -1;

    for (size_t i = 0; i < vec->size; i++)
    {
        char *target_address = (char *)vec->data + (i * vec->elem_size);

        if (memcmp(target_address, value_ptr, vec->elem_size) == 0)
        {
            return (ssize_t)i;
        }
    }
    return -1;
}

VectorStatus vector_contains(Vector *vec, void *value_ptr)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(2, vec, value_ptr);
    if (status != VEC_OK) return status;

    return vector_search(vec, value_ptr) != (ssize_t)-1 ? 
                            VEC_PRESENT : VEC_NOT_PRESENT;
}

VectorStatus vector_clear(Vector *vec)
{
    // Checking if the vector is valid or not
    VectorStatus status = vector_input_check(1, vec);
    if (status != VEC_OK) return status;

    // going back to initial state.
    // Also if I wanted I can simply destroy the vector and re-initialize a new one
    vec->data = NULL;
    vec->capacity = 4;
    vec->size = 0;

    return VEC_OK;
}

VectorStatus vector_destroy(Vector *vec)
{
    if (vec == NULL)
        return VEC_ERR_NULL_ARG;

    free(vec->data);
    free(vec);

    return VEC_OK;
}