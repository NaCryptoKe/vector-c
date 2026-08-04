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
    if (v->size == v->capacity / 4)
    {
        v->capacity /= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    return result;
}

void insert(Vector *v, int pos, int value)
{
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        int *new_data = realloc(v->data, v->capacity * sizeof(int));
        v->data = new_data;
    }
    v->size++;
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

void replace(Vector *v, size_t init_pos, size_t end_pos, int old_value, int new_value)
{
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

int main(void)
{
    Vector v = { .data = NULL, .size = 0, .capacity = 0 };
    printf("\n=== PUSH, PUSH_FRONT, POP, POP_FRONT ===\n");
    push(&v, 2);
    push(&v, 3);
    push_front(&v, 1);

    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    pop(&v);        // popped 3
    pop_front(&v);  // popped 1

    printf("\n\n");
    if (v.size > 0) {
        printf("%d\n", v.data[v.size - 1]);  // prints 2
    }

    printf("\n=== Insert, Erase ===\n");
    insert(&v, 2, 3);
    erase(&v, 1);

    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    push(&v, 2); push(&v, 3); push_front(&v, 1);
    push(&v, 2); push(&v, 3); push_front(&v, 1);
    push(&v, 2); push(&v, 3); push_front(&v, 1);
    push(&v, 2); push(&v, 3); push_front(&v, 1);

    printf("\n\n");
    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    printf("Size Count: %zu\n", v.size);

    replace(&v, 1, v.size, 3, 10);

    for (size_t i = 0; i < v.size; i++) {
        printf("%d\n", v.data[i]);
    }

    clear(&v);

    return 0;
}
