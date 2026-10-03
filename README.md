# Vector-c

[![CI](https://github.com/NaCryptoKe/vector-c/actions/workflows/ci.yml/badge.svg)](https://github.com/NaCryptoKe/vector-c/actions/workflows/ci.yml)
[![CodeQL](https://github.com/NaCryptoKe/vector-c/actions/workflows/codeql.yml/badge.svg)](https://github.com/NaCryptoKe/vector-c/actions/workflows/codeql.yml)

A general (type-agnostic) dynamic array implementation written in c.

The library provides a simple vector-like container using `void *` and supports common operations such as insertion, removal, searching, and replacement.

## Requirements

- A C99-or-newer compiler (the Makefile targets C17)
- GCC or Clang
- Make

The library also compiles cleanly as C++ because `vector.h` is self-contained.

## Features

- Generic `void *` storage
- Dynamic resizing
- Push and pop operations, both front and back
- Insert and erase
- Conditional range replacement
- Element searching and containment checks
- Automatic capacity growth and shrinking, with hysteresis
- Configurable element size
- `VectorStatus`-based error reporting on every operation
- Overflow-checked allocation
- Test suite, sanitizers and CI (GCC and Clang)

## Build

Clone the repository:

```bash
git clone https://github.com/NaCryptoKe/vector-c.git
cd vector-c
```

Build:

```bash
make
```

Run:

```bash
./bin/app
```

Available targets:

| Target | Purpose |
|---|---|
| `make` | Build the example application into `bin/app` |
| `make test` | Build and run the test suite |
| `make asan` | Run the test suite under ASan + UBSan |
| `make asan-app` | Run the example app under ASan + UBSan |
| `make check` | Everything above, in order |
| `make clean` | Remove build output |

To build with a different compiler, pass it on the command line:

```bash
make CC=clang
make CC=clang check
```

## Usage

`vector_init()` allocates and returns a new vector, so hold it in a `Vector *`.
Every function returns a `VectorStatus` that should be checked.

```c
#include <stdio.h>

#include "vector.h"

int main(void)
{
    Vector *vec = vector_init(sizeof(int));

    if (vec == NULL)
    {
        fprintf(stderr, "vector_init() failed\n");
        return 1;
    }

    int value = 42;

    if (vector_push_back(vec, &value) != VEC_OK)
    {
        vector_destroy(vec);
        return 1;
    }

    int out = 0;

    if (vector_get(vec, 0, &out) != VEC_OK)
    {
        vector_destroy(vec);
        return 1;
    }

    printf("%d\n", out);   /* 42 */

    vector_destroy(vec);

    return 0;
}
```

Because storage is `void *`, the element size is fixed when the vector is
created. Any type of that size can be stored:

```c
Vector *names = vector_init(sizeof(char *));

const char *value = "vector";

vector_push_back(names, &value);   /* note the address of the pointer */

char *retrieved = NULL;

vector_get(names, 0, &retrieved);  /* retrieved == value */
vector_destroy(names);
```

### Error handling

Every operation returns a `VectorStatus`. `VEC_OK` (0) means success; all
failures are negative and are safe to compare directly.

```c
VectorStatus status = vector_get(vec, 0, &out);

if (status == VEC_ERR_OUT_OF_RANGE)
{
    /* index was past the end */
}
else if (status != VEC_OK)
{
    /* some other failure, e.g. VEC_ERR_NULL_ARG */
}
```

| Status | Value | Meaning |
|---|---:|---|
| `VEC_OK` | 0 | Success |
| `VEC_ERR_OUT_OF_RANGE` | -1 | Index is past the last element |
| `VEC_ERR_ALLOC` | -3 | `realloc`/`malloc` failed, or the size would overflow |
| `VEC_ERR_NULL_ARG` | -4 | A required pointer argument was `NULL` |
| `VEC_ERR_EMPTY_VECTOR` | -5 | Pop on an empty vector |
| `VEC_PRESENT` | -6 | Returned by `vector_contains` only |
| `VEC_NOT_PRESENT` | -7 | Returned by `vector_contains` only |
| `VEC_ERR_EMPTY` | -2 | Declared but not currently returned |
| `VEC_INVALID_SIZE` | -8 | Declared but not currently returned |

`vector_init` reports an invalid element size by returning `NULL` rather than a
`VectorStatus`.

### `vector_search` returns `ssize_t`

`vector_search` is the one function that does not return a `VectorStatus`. It
returns the **index** of the first matching element, or `-1`:

```c
int needle = 42;

ssize_t index = vector_search(vec, &needle);

if (index < 0)
{
    /* either not found, or a NULL argument was passed -- see Limitations */
}
```

Note that `-1` does not distinguish "absent" from "invalid argument", so check
your arguments before relying on a negative result. See
[Limitations](#limitations).

### `vector_replace` is a conditional range replace

`vector_replace` is not a single-element setter. It scans the inclusive range
`[init_pos, end_pos]` and rewrites **every** element in that range whose bytes
match `old_value_ptr`:

```c
int old_value = 2;
int new_value = 99;

/* On [0 1 2 3 4 5] this yields [0 1 99 3 4 5] */
vector_replace(vec, 0, 5, &old_value, &new_value);
```

If the same value occurs more than once in the range, every occurrence is
replaced. On success the function returns `VEC_OK`; it does **not** report how
many elements were changed, so verify the result with `vector_get` if you need
that count.

## API

| Function | Signature summary | Description |
|---|---|---|
| `vector_init` | `Vector *(size_t elem_size)` | Allocate a vector; returns `NULL` on failure or `elem_size == 0` |
| `vector_destroy` | `VectorStatus(Vector *)` | Free the buffer and the vector |
| `vector_clear` | `VectorStatus(Vector *)` | Remove all elements, reset capacity to the initial value |
| `vector_push_back` | `VectorStatus(Vector *, const void *)` | Append an element |
| `vector_push_front` | `VectorStatus(Vector *, const void *)` | Prepend an element |
| `vector_pop_back` | `VectorStatus(Vector *, void *out)` | Remove and copy out the last element |
| `vector_pop_front` | `VectorStatus(Vector *, void *out)` | Remove and copy out the first element |
| `vector_insert` | `VectorStatus(Vector *, size_t pos, const void *)` | Insert at `pos`; `pos == size` appends |
| `vector_erase` | `VectorStatus(Vector *, size_t pos)` | Remove the element at `pos` |
| `vector_get` | `VectorStatus(const Vector *, size_t pos, void *out)` | Copy out the element at `pos` |
| `vector_replace` | `VectorStatus(Vector *, size_t init_pos, size_t end_pos, const void *old, const void *new)` | Replace every match in the inclusive range — see above |
| `vector_search` | `ssize_t(const Vector *, const void *)` | Index of the first match, or `-1` |
| `vector_contains` | `VectorStatus(const Vector *, const void *)` | `VEC_PRESENT` or `VEC_NOT_PRESENT` |

The full declarations are in [`include/vector.h`](include/vector.h).

## Design

The vector stores elements in a contiguous dynamically allocated memory region.

Each vector tracks:

- Current number of elements (`size`)
- Allocated capacity (`capacity`)
- Size of each element (`elem_size`)

Elements are stored generically using `void *`, allowing the vector to store different C data types.

### Resizing

Capacity starts at 4 and grows by doubling. Shrinking is deliberately
separate: the buffer is only reduced once `size` has fallen to a quarter of
capacity, and then only by halving.

```text
capacity:  4 ──► 8 ──► 16 ──► 32 ──► 64
           (grow when size == capacity)

shrink:    64 ──► 32 ──► 16 ──► 8 ──► 4
           (only when size <= capacity / 4, never below 4)
```

The gap between the two thresholds is intentional. Growing at capacity and
shrinking at the same boundary would make a vector that oscillates around that
size reallocate on every single operation, turning amortized O(1) into O(n).
Separating the thresholds avoids that.

Every allocation multiplies `capacity * elem_size`, which can overflow. Each
product is checked against `SIZE_MAX` before the multiplication, and a failed
`realloc` leaves the vector unchanged and usable rather than half-freed.

## Complexity

`n` is the number of elements; `k` is the width of a `vector_replace` range.

| Operation | Complexity |
|---|---:|
| `push_back` | O(1) amortized |
| `push_front` | O(n) |
| `insert` | O(n) |
| `pop_back` | O(1) |
| `pop_front` | O(n) |
| `erase` | O(n) |
| `get` | O(1) |
| `replace` | O(k) |
| `search` | O(n) |
| `contains` | O(n) |
| `clear` | O(n) |
| `destroy` | O(1) |

`push_front`, `pop_front`, `insert` and `erase` are O(n) because the remaining
elements must be shifted to keep the buffer contiguous.

## Limitations

These are deliberate properties of the design, not oversights.

- **No compile-time type checking.** Storage is `void *` and element size is
  fixed at creation, so pushing the wrong type is undefined behaviour and is
  caught by neither the compiler nor the library. Match `elem_size` to the type
  you actually store.
- **Comparison is bytewise.** `vector_search` and `vector_contains` use
  `memcmp`. For types containing padding bytes, or pointers, two logically
  equal values may compare unequal because their representations differ.
- **No ownership of pointed-to data.** Elements are copied by value. If you
  store a pointer, the vector holds a shallow copy: it will not free the
  pointee, and you remain responsible for its lifetime. There is no element
  destructor, so `vector_destroy` frees only the buffer.
- **`vector_search` conflates "not found" with "invalid argument".** Both return
  `-1`, and `-1` is also the value of `VEC_ERR_OUT_OF_RANGE`, so the numeric
  spaces overlap. Validate arguments yourself before interpreting a negative
  result.
- **`vector_replace` does not report a count.** It returns `VEC_OK` whether it
  changed one element or many.
- **`vector_destroy` does not null the caller's pointer.** The pointer is
  dangling afterwards.
- **Not thread-safe.** Concurrent access to the same vector from multiple
  threads requires external synchronisation. A single vector used by one thread
  at a time needs none.
- **Capacity shrinks only on `pop_back`, `pop_front` and `erase`.** Holding a
  large buffer after a burst of pushes is expected; there is no `shrink_to_fit`.

## Testing

The test suite lives in [`tests/`](tests/) and is run with:

```bash
make test
```

It covers basic operations, front/back insertion and removal, `replace`,
`search`/`contains`, error handling, and stress tests of **100,000** elements
covering growth, pop ordering, and clear/reuse. The suite returns a non-zero
exit status if any check fails, so it can be used directly as a CI gate.

### Sanitizers

Because manual memory management is the entire purpose of this library, every
change is also verified under AddressSanitizer and UndefinedBehaviorSanitizer:

```bash
make asan       # full test suite under ASan + UBSan (leak detection on)
make asan-app   # the example application under ASan + UBSan
make check      # build + test + asan + asan-app
```

The current state is clean:

```text
$ make asan
========================================
[INFO] Test Summary
========================================
Passed: 10
Failed: 0
[INFO] ALL TESTS PASSED
```

No sanitizer diagnostics are reported, and LeakSanitizer reports no leaks.

### Continuous integration

Every push and pull request is built and tested on GitHub Actions with **both
GCC and Clang** ([`ci.yml`](.github/workflows/ci.yml)). Each job runs the build,
the test suite, both sanitizer targets, a `-Werror` gate with
`-Wshadow -Wconversion -Wstrict-prototypes -Wmissing-prototypes`, and a check
that `vector.h` is self-contained and C++-compatible. Static analysis runs
separately via CodeQL ([`codeql.yml`](.github/workflows/codeql.yml)).

## Roadmap

- [x] Generic `void *` vector
- [x] Basic operations
- [x] Automatic resizing
- [x] Tests
- [x] Sanitizers and CI
- [x] Documentation and licensing
- [ ] Benchmarking
- [ ] Header-only macro or `_Generic` variant, with a comparison against `void *`

## License

This project is licensed under the MIT License.
See [LICENSE](LICENSE) for details.