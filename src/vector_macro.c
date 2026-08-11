/*
 * This utility is a 1-1 copy of my vector.c file
 * Instead of using void* functions, here I'll be using macro functions.
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define VECTOR(T, Name)                                               \
    typedef struct {                                                  \
        T      *data;                                                 \
        size_t  size;                                                 \
        size_t  capacity;                                             \
    } Name##_t;                                                       \
                                                                      \
    Name##_t Name = { NULL, 0, 0 };                                   \
                                                                      \
    void Name##_push(T value)                                         \
    {                                                                 \
        if (Name.size == Name.capacity) {                             \
            size_t new_cap = Name.capacity ? Name.capacity * 2 : 4;   \
            T *new_data = realloc(Name.data, new_cap * sizeof(T));    \
            if (!new_data) return;                                    \
            Name.data     = new_data;                                 \
            Name.capacity = new_cap;                                  \
        }                                                             \
        Name.data[Name.size++] = value;                               \
    }

VECTOR(int, numbers)

