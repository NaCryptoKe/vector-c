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
/*
void insert(Vector *v, size_t pos, int value)
{
    if (pos > v->size)
    {
        return;
    }

    if (v->size == v->capacity)
    {
        if (v->capacity == 0) v->capacity++;
        v->capacity *= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        v->data = new_data;
    }
    v->size++;
    memmove(v->data + pos + 1, v->data + pos, (v->size - pos + 1) * sizeof(int));
    v->data[pos] = value;
}

void erase (Vector *v, size_t pos)
{
    if (pos > v->size)
    {
        return;
    }
    memmove(v->data + pos, v->data + pos + 1, (v->size - pos + 1) * sizeof(int));
    v->size--;
    if (v->size == v->capacity/4)
    {
        v->capacity /= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        v->data = new_data;
    }
}

void replace(Vector *v, size_t init_pos, size_t end_pos, int old_value, int new_value)
{
    if (init_pos > v->size ||
        end_pos > v->size ||
        init_pos > end_pos)
    {
        return;
    }

    for (size_t i = init_pos; i <= end_pos; i++) {
        if (v->data[i] == old_value)
        {
            v->data[i] = new_value;
        }
    }
}

void clear(Vector *v)
{
    free(v->data);
    vector_init(v);
}

size_t search(Vector *v, int value)
{
    for (size_t i = 0; i < v->size; i++)
    {
        if (v->data[i] == value)
        {
            return i;
        }
    }
    return -1;
}

int contains(Vector *v, int value)
{
    return search(v, value) != (size_t)-1;
}
*/