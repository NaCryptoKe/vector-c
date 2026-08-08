#include "../include/vector.h"
#include "../include/logger.h"
#include <stdio.h>

#include <stdlib.h> // for realloc and free
#include <string.h> // for memmove

VectorStatus vector_init_check(const size_t elem_size)
{
    if (elem_size == 0)
        return VEC_INVALID_SIZE;
    
    return VEC_OK;
}

VectorStatus check(void *vec, void *value_ptr)
{
    if (vec == NULL || value_ptr == NULL)
        return VEC_ERR_NULL_ARG;
    
    return VEC_OK;
}

Vector* vector_init(const size_t elem_size)
{
    if (vector_init_check(elem_size) != VEC_OK)
        return NULL;
    Vector* vec = (Vector *)malloc(sizeof(Vector));
    
    // Just placeholder till I make check varaiadic.
    int value = 42;

    if (check(vec, &value) != VEC_OK)
        return NULL;

    vec->capacity = 4;  // default starting point
    vec->size = 0;
    vec->elem_size = elem_size;

    vec->data = malloc(vec->capacity * vec->elem_size);
    if (check(vec->data, &value) != VEC_OK)
    {
        free(vec);
        return NULL;
    }

    return vec;
}

VectorStatus vector_push_back(Vector *vec, const void *value_ptr)
{
    LOG("Push Back\n");

    if (vec == NULL || value_ptr == NULL)
        return VEC_ERR_NULL_ARG;
    
    if (vec->size >= vec->capacity)
    {        
        vec->capacity = vec->capacity ? vec->capacity * 2 : 4;
        void *new_data = realloc(
                                vec->data, 
                                vec->capacity * vec->elem_size
                            );
        if (new_data == NULL)   // Failed to get memory
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
    }

    char *target_address = (char *)vec->data + (vec->size * vec->elem_size);
    memcpy(target_address, value_ptr, vec->elem_size); // dereferences the target_address and changes the value of data
    vec->size++;

    return VEC_OK;    
}

VectorStatus vector_push_front(Vector *vec, const void *value_ptr)
{
    LOG("Push Front\n");
    
    if (vec == NULL || value_ptr == NULL)
        return VEC_ERR_NULL_ARG;

    if (vec->size >= vec->capacity)
    {
        vec->capacity = vec->capacity ? vec->capacity * 2 : 4;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
    }
    vec->size++;
    memmove(
        (char *)vec->data + vec->elem_size, 
        (char *)vec->data, 
        (vec->size - 1) * vec->elem_size
    );
    memcpy(vec->data, value_ptr, vec->elem_size);

    return VEC_OK;
}

VectorStatus vector_pop_back(Vector *vec, void *out)
{
    LOG("Pop Back\n");
    if (vec == NULL || out == NULL)
        return VEC_ERR_NULL_ARG;
        
    if (vec->size == 0)
        return VEC_ERR_EMPTY_VECTOR;

    vec->size--;
    memcpy(
        out,
        (char *)vec->data + (vec ->size * vec->elem_size),
        vec->elem_size
    );
    
    if (
        vec->size <= vec->capacity / 4 && 
        vec->capacity > 4
    )
    {
        vec->capacity /= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
    }

    return VEC_OK;
}

VectorStatus vector_pop_front(Vector *vec, void *out)
{
    LOG("Pop Front\n");
    if (vec == NULL || out == NULL)
        return VEC_ERR_NULL_ARG;

    if (vec->size == 0)
        return VEC_ERR_EMPTY_VECTOR;

    vec->size--;
    memcpy(
        out, 
        vec->data, 
        vec->elem_size
    );
    memmove(
        (char *)vec->data, 
        (char *)vec->data + vec->elem_size, 
        vec->size * vec->elem_size
    );

    if (
        vec->size <= vec->capacity / 4 && 
        vec->capacity > 4
    )
    {
        vec->capacity /= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec ->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
    }
    return VEC_OK;
}

VectorStatus vector_insert(Vector *vec, size_t pos, const void *value_ptr)
{
    LOG("Vector Insert\n");
    if (vec == NULL || value_ptr == NULL)
        return VEC_ERR_NULL_ARG;

    if (pos > vec->size)
        return VEC_ERR_OUT_OF_RANGE;

    if (vec->size >= vec->capacity)
    {
        vec->capacity = vec->capacity ? vec->capacity * 2 : 4;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
    }

    vec->size++;
    memmove(
        (char *)vec->data + (pos * vec->elem_size) + vec->elem_size,
        (char *)vec->data + (pos * vec->elem_size),
        (vec->size - pos - 1) * vec->elem_size
    );
    memcpy(
        (char *)vec->data + (pos * vec->elem_size),
        value_ptr,
        vec->elem_size
    );

    return VEC_OK;
}

VectorStatus vector_erase(Vector *vec, size_t pos)
{
    if (vec == NULL)
        return VEC_ERR_NULL_ARG;

    if (pos >= vec->size)
        return VEC_ERR_OUT_OF_RANGE;

    memmove(
        (char *)vec->data + (pos * vec->elem_size),
        (char *)vec->data + (pos * vec->elem_size) + vec->elem_size,
        (vec->size - pos - 1) * vec->elem_size
    );
    vec->size--;

    if (vec->size <= vec->capacity/4 && vec->capacity > 4)
    {
        vec->capacity /= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            return VEC_ERR_ALLOC;
        }
        vec->data = new_data;
    }
    return VEC_OK;
}

VectorStatus vector_replace(Vector *vec, size_t init_pos, size_t end_pos, void *old_value_ptr, void *new_value_ptr)
{
    LOG("Vector Replace\n");
    if (vec == NULL || old_value_ptr == NULL || new_value_ptr == 0)
        return VEC_ERR_NULL_ARG;

    if (
        init_pos >= vec->size ||
        end_pos >= vec->size ||
        init_pos > end_pos
    )
        return VEC_ERR_OUT_OF_RANGE;

    for (size_t i = init_pos; i <= end_pos; i++) {
        if (
            memcmp(
                (char *)vec->data + (i * vec->elem_size),
                old_value_ptr,
                vec->elem_size
            ) == 0)
        {
            memcpy(
                (char *)vec->data + (i * vec->elem_size), 
                new_value_ptr, 
                vec->elem_size
            );
        }
    }

    return VEC_OK;
}

VectorStatus vector_get(Vector *vec, size_t pos, void *out)
{
    LOG("Vector Get\n");
    if (vec == NULL || out == NULL)
        return VEC_ERR_NULL_ARG;

    if (pos >= vec->size)
        return VEC_ERR_OUT_OF_RANGE;
    
    memcpy(out, (char *)vec->data + (pos * vec->elem_size), vec->elem_size);

    return VEC_OK;
}

size_t vector_search(Vector *vec, void *value_ptr)
{
    LOG("Vector Search\n");
    if (check(vec, value_ptr) != VEC_OK)
        return -1;

    for (size_t i = 0; i < vec->size; i++)
    {
        if (memcmp((char *)vec->data + (i * vec->elem_size), value_ptr, vec->elem_size) == 0)
        {
            return i;
        }
    }
    return -1;
}

VectorStatus vector_contains(Vector *vec, void *value_ptr)
{
    LOG("Vector Contatains\n");
    if (vec == NULL || value_ptr == NULL)
        return VEC_ERR_NULL_ARG;

    return vector_search(vec, value_ptr) != (size_t)-1 ? VEC_PRESENT : VEC_NOT_PRESENT;
}

VectorStatus vector_clear(Vector *vec)
{
    LOG("Vector Clear\n");
    if (vec == NULL)
        return VEC_ERR_NULL_ARG;

    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;

    return VEC_OK;
}

VectorStatus vector_destroy(Vector *vec)
{
    LOG("Vector Destroyed\n");
    if (vec == NULL)
        return VEC_ERR_NULL_ARG;

    free(vec->data);
    free(vec);

    return VEC_OK;
}