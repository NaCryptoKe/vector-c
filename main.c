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
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
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
    if (v->size == (int)(v->capacity / 4))
    {
        v->capacity /= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    return result;
}

void insert(Vector *v, int pos, int value)
{
    v->size++;
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    memmove(v->data + pos, v->data + pos - 1, (v->size - pos) * sizeof(int));
    v->data[pos - 1] = value;
}

void erase (Vector *v, int pos)
{
    memmove(v->data + pos - 1, v->data + pos, (v->size - pos) * sizeof(int));
    v->size--;
    if (v->size == v->capacity/4)
    {
        v->capacity /= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
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

    pop(&v);        // popped 3
    pop_front(&v); // popped 1

    printf("%d\n", v.data[v.size - 1]);  // prints 2
    insert(&v, 2, 3);
    erase(&v, 1);
    for (int i = 0; i < v.size; i++) {
        printf ("%d\n", v.data[i]);
    }

    return 0;
}
