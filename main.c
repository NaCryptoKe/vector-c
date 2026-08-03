#include <stdio.h>
#include <stddef.h> // for size_t
#include <stdlib.h> // for realloc
#include <string.h> // for memmove

typedef struct
{
    int* data;
    size_t size;        // actual size of the vector
    size_t capacity;    // how much it can store
} Vector;

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
    v->size++;
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    memmove(v->data + 1, v->data, (v->size - 1) * sizeof(int));
    v->data[0] = value;
}

int pop(Vector *v)
{
    v->size--;
    int result = v->data[v->size];
    v->data[v->size] = -1; // This is indicating that it is a garbage
    return result;
}

int pop_front(Vector *v)
{
    v->size--;
    int result = v->data[0];
    memmove(v->data, v->data + 1, v->size * sizeof(int));
    return result;
}

int main(void)
{
    Vector v = { .data = NULL, .size = 0, .capacity = 0};
    push(&v, 2);
    push(&v, 3);
    push_front(&v, 1);

    for (int i = 0; i < v.size; i++) {
        printf ("%d\n", v.data[i]);
    }

    return 0;
}
