#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h> // for size_t

typedef struct
{
    int* data;
    size_t size;        // actual size of the vector
    size_t capacity;    // how much it can store
    size_t elem_size;   // the data type's size
} Vector;

void vector_init(Vector *vec);

void push(Vector *vec, int value);
void push_front(Vector *vec, int value);

int pop(Vector *vec);
int pop_front(Vector *vec);

void insert(Vector *vec, size_t pos, int value);
void erase(Vector *vec, size_t pos);
void replace(Vector *vec, size_t init_pos, size_t end_pos, int old_value, int new_value);

size_t search(Vector *vec, int value);
int contains(Vector *vec, int value);

void clear(Vector *vec);

#endif // VECTOR_H