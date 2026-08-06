#include "vector.h"

#include <stdlib.h> // for realloc and free
#include <string.h> // for memmove

void push(Vector *v, int value)
{
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        if (v->capacity == 0) v->capacity++;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    v->data[v->size] = value;
    v->size++;
}

void push_front(Vector *v, int value)
{
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        if (v->capacity == 0) v->capacity++;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    v->size++;
    memmove(v->data + 1, v->data, (v->size - 1) * sizeof(int));
    v->data[0] = value;
}

int pop(Vector *v)
{
    v->size--;
    int result = v->data[v->size];
    if (v->size == v->capacity / 4)
    {
        v->capacity /= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    return result;
}

int pop_front(Vector *v)
{
    v->size--;
    int result = v->data[0];
    memmove(v->data, v->data + 1, v->size * sizeof(int));
    if (v->size == v->capacity / 4)
    {
        v->capacity /= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    return result;
}

void insert(Vector *v, size_t pos, int value)
{
    // Check for a valid position input
    if (pos < 1)
    {
        return;
    }
    if (pos > v->size+1)
    {
        return;
    }
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        if (v->capacity == 0) v->capacity++;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    v->size++;
    memmove(v->data + pos, v->data + pos - 1, (v->size - pos) * sizeof(int));
    v->data[pos - 1] = value;
}

void erase (Vector *v, size_t pos)
{
    if (pos < 1)
    {
        return;
    }
    if (pos > v->size+1)
    {
        return;
    }
    memmove(v->data + pos - 1, v->data + pos, (v->size - pos) * sizeof(int));
    v->size--;
    if (v->size == v->capacity/4)
    {
        v->capacity /= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
}

void replace(Vector *v, size_t init_pos, size_t end_pos, int old_value, int new_value)
{
    if (init_pos < 1 && end_pos < 1)
    {
        return;
    }
    if (init_pos > v->size+1 && end_pos > v->size+1 && init_pos > end_pos)
    {
        return;
    }
    for (size_t i = init_pos - 1; i <= end_pos - 1; i++) {
        if (v->data[i] == old_value)
        {
            v->data[i] = new_value;
        }
    }
}

void clear(Vector *v)
{
    free(v->data);
    v->data = NULL;
    v->capacity = 0;
    v->size = 0;
}

int search(Vector *v, int value)
{
    for (size_t i = 0; i < v->size; i++)
    {
        if (v->data[i] == value)
        {
            return i+1;
        }
    }
    return -1;
}

int contains(Vector *v, int value)
{
    for (size_t i = 0; i < v->size; i++)
    {
        if (v->data[i] == value)
        {
            return 1;   // return true
        }
    }
    return 0;       // return false
}
