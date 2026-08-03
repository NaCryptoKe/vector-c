#include <stdio.h>
#include <stddef.h> // for size_t
#include <stdlib.h> // for realloc
#include <string.h> // for memmove

typedef struct
{
    int* data;
    size_t size;
} Vector;

void push(Vector *v, int value)
{
    int *new_data = realloc(v->data, (v->size + 1) * sizeof(int));
    v->data = new_data;
    v->data[v->size] = value;
    v->size++;
}

void push_front(Vector *v, int value)
{
    v->size++;
    int *new_data = realloc(v->data, v->size * sizeof(int));
    v->data = new_data;
    memmove(v->data + 1, v->data, (v->size - 1) * sizeof(int));
    v->data[0] = value;
}

int pop(Vector *v)
{
    v->size--;
    int result = v->data[v->size];
    int *new_data = realloc(v->data, (v->size) * sizeof(int));
    v->data = new_data;
    return result;
}

int pop_front(Vector *v)
{
    v->size--;
    int result = v->data[0];
    memmove(v->data, v->data + 1, v->size * sizeof(int));
    int *new_data = realloc(v->data, (v->size) * sizeof(int));
    v->data = new_data;
    return result;
}
int main(void)
{
    Vector v = { .data = NULL, .size = 0};
    push(&v, 4);
    printf("%d\n", v.data[0]);
    push(&v, 5);
    printf("%d\n", v.data[1]);
    pop(&v);
    printf("%d\n", v.data[0]);
    printf("%d\n", v.data[1]);  // will return garbage.might return the previous pushed value.
}
