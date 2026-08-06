#include "vector.h"
#include <stdio.h>

#include <stdlib.h> // for realloc and free
#include <string.h> // for memmove

void vector_init(Vector *v)
{
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void push(Vector *v, int value)
{
    if (v->size == v->capacity)
    {
        if (v->capacity == 0) v->capacity++;
        v->capacity *= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        if (new_data == NULL)   // Failed to get memory
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        v->data = new_data;
    }
    v->data[v->size] = value;
    v->size++;
}

void push_front(Vector *v, int value)
{
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
    memmove(v->data + 1, v->data, (v->size - 1) * sizeof(int));
    v->data[0] = value;
}

int pop(Vector *v)
{
    if (v->size == 0)
    {
        return -1;  //placeholder for now
    }
    v->size--;
    int result = v->data[v->size];

    if (v->size == v->capacity / 4)
    {
        v->capacity /= 2;
        if (v->capacity == 0) {
            v->capacity = 1;
            return result;
        }
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        v->data = new_data;
    }
    return result;
}

int pop_front(Vector *v)
{
    if (v->size == 0)
    {
        return -1;  //placeholder for now
    }
    v->size--;
    int result = v->data[0];
    memmove(v->data, v->data + 1, v->size * sizeof(int));
    if (v->size == v->capacity / 4)
    {
        v->capacity /= 2;
        if (v->capacity == 0) {
            v->capacity = 1;
            return result;
        }
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        if (new_data == NULL)
        {
            fprintf(stderr, "Fatal: Out of memory\n");
            exit(EXIT_FAILURE);
        }
        v->data = new_data;
    }
    return result;
}

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
    return search(v, value) != -1;
}
