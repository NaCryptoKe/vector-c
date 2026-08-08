#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h> // for size_t

typedef enum 
{
    VEC_OK                  =  0,
    VEC_ERR_OUT_OF_RANGE    = -1,
    VEC_ERR_EMPTY           = -2,
    VEC_ERR_ALLOC           = -3,
    VEC_ERR_NULL_ARG        = -4,
    VEC_ERR_EMPTY_VECTOR    = -5,
    VEC_PRESENT             = -6,
    VEC_NOT_PRESENT         = -7,
    VEC_INVALID_SIZE        = -8
} VectorStatus;

typedef struct
{
    void *data;         // Raw block of generic memory
    size_t size;        // actual size of the vector
    size_t capacity;    // how much it can store
    size_t elem_size;   // the data type's size
} Vector;

Vector* vector_init(const size_t elem_size);

VectorStatus vector_push_back(Vector *vec, const void *value_ptr);
VectorStatus vector_push_front(Vector *vec, const void *value_ptr);

VectorStatus vector_pop_back(Vector *vec, void *out);
VectorStatus vector_pop_front(Vector *vec, void *out);

VectorStatus vector_insert(Vector *vec, size_t pos, const void *value_ptr);
VectorStatus vector_erase(Vector *vec, size_t pos);
VectorStatus vector_replace(Vector *vec, size_t init_pos, size_t end_pos, void *old_value_ptr, void *new_value_ptr);
VectorStatus vector_get(Vector *vec, size_t pos, void *out);

size_t vector_search(Vector *vec, void *value_ptr);
VectorStatus vector_contains(Vector *vec, void *value_ptr);

VectorStatus vector_clear(Vector *vec);
VectorStatus vector_destroy(Vector *vec);

#endif // VECTOR_H