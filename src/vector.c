#include "../include/vector.h"
#include "../include/logger.h"
#include <stdio.h>

#include <stdlib.h> // for realloc and free
#include <string.h> // for memmove

Vector* vector_init(size_t elem_size)
{
    Vector* vec = (Vector *)malloc(sizeof(Vector));
    vec->capacity = 4;  // default starting point
    vec->size = 0;
    vec->elem_size = elem_size;
    vec->data = malloc(vec->capacity * vec->elem_size);

    // Logging info
    PRINT("%zu", vec->capacity);
    PRINT("%zu", vec->size);
    PRINT("%zu", vec->elem_size);

    return vec;
}

void vector_push_back(Vector *vec, const void *value_ptr)
{
    LOG("Push Back\n");
    if (vec->capacity < 4)
        vec->capacity = 4;
    if (vec->size >= vec->capacity)
    {        
        vec->capacity *= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)   // Failed to get memory
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }

    char *target_address = (char *)vec->data + (vec->size * vec->elem_size);
    memcpy(target_address, value_ptr, vec->elem_size); // dereferences the target_address and changes the value of data
    vec->size++;
    
}

void vector_push_front(Vector *vec, const void *value_ptr)
{
    LOG("Push Front\n");
    if (vec->capacity < 4)
        vec->capacity = 4;
    if (vec->size >= vec->capacity)
    {
        vec->capacity *= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }
    vec->size++;
    memmove((char *)vec->data + vec->elem_size, (char *)vec->data, (vec->size - 1) * vec->elem_size);
    memcpy(vec->data, value_ptr, vec->elem_size);
}

int vector_pop_back(Vector *vec, void *out)
{
    LOG("Pop Back\n");
    if (vec->size == 0)
    {
        return -1;  //placeholder for now
    }

    vec->size--;
    memcpy(out, vec->data + (vec ->size * vec->elem_size), vec->elem_size);
    
    if (vec->size <= vec->capacity / 4 && vec->capacity > 4)
    {
        vec->capacity /= 2;
        if (vec->capacity < 4)
            vec->capacity = 4;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }
    return 1;
}

int vector_pop_front(Vector *vec, void *out)
{
    LOG("Pop Front\n");
    if (vec->size == 0)
    {
        return -1;  //placeholder for now
    }

    vec->size--;
    memcpy(out, vec->data, vec->elem_size);
    memmove((char *)vec->data, (char *)vec->data + vec->elem_size, vec->size * vec->elem_size);

    if (vec->size <= vec->capacity / 4 && vec->capacity > 4)
    {
        vec->capacity /= 2;
        if (vec->capacity <= 4)
            vec->capacity = 4;
        void *new_data = realloc(vec->data, vec->capacity * vec ->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }
    return 1;
}

void vector_insert(Vector *vec, size_t pos, const void *value_ptr)
{
    LOG("Vector Insert\n");
    if (pos > vec->size)
    {
        return;
    }

    if (vec->capacity < 4)
        vec->capacity = 4;

    if (vec->size >= vec->capacity)
    {
        if (vec->capacity == 0) 
            vec->capacity++;

        vec->capacity *= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }
    vec->size++;
    memmove(vec->data + (pos * vec->elem_size) + vec->elem_size, vec->data + (pos * vec->elem_size), (vec->size - pos - 1) * vec->elem_size);
    memcpy(vec->data + (pos * vec->elem_size), value_ptr, vec->elem_size);
}

void vector_erase(Vector *vec, size_t pos)
{
    if (pos >= vec->size)
    {
        return;
    }
    memmove(vec->data + (pos * vec->elem_size), vec->data + (pos * vec->elem_size) + vec->elem_size, (vec->size - pos - 1) * vec->elem_size);
    vec->size--;
    if (vec->size <= vec->capacity/4 && vec->capacity > 4)
    {
        vec->capacity /= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec->elem_size);
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }
}

void vector_replace(Vector *vec, size_t init_pos, size_t end_pos, void *old_value_ptr, void *new_value_ptr)
{
    LOG("Vector Replace\n");
    if (init_pos >= vec->size ||
        end_pos >= vec->size ||
        init_pos > end_pos)
    {
        return;
    }

    for (size_t i = init_pos; i <= end_pos; i++) {
        if (memcmp(vec->data + (i * vec->elem_size), old_value_ptr, vec->elem_size) == 0)
        {
            memcpy(vec->data + (i * vec->elem_size), new_value_ptr, vec->elem_size);
        }
    }
}

void vector_get(Vector *vec, size_t pos, void *out)
{
    LOG("Vector Get\n");
    if (pos >= vec->size)
        return;
    
    memcpy(out, vec->data + (pos * vec->elem_size), vec->elem_size);
}

size_t vector_search(Vector *vec, void *value_ptr)
{
    LOG("Vector Search\n");
    for (size_t i = 0; i < vec->size; i++)
    {
        if (memcmp(vec->data + (i * vec->elem_size), value_ptr, vec->elem_size) == 0)
        {
            return i;
        }
    }
    return -1;
}

int vector_contains(Vector *vec, void *value_ptr)
{
    LOG("Vector Contatains\n");
    return (int)(vector_search(vec, value_ptr) != (size_t)-1);
}

void vector_clear(Vector *vec)
{
    LOG("Vector Clear\n");
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
}

void vector_destroy(Vector *vec)
{
    LOG("Vector Destroyed\n");
    free(vec->data);
    free(vec);
}