# Vector-c

A general (type-agnostic) dynamic array implementation written in c.

The library provides a simple vector-like container using `void *` and supports common operations such as insertion, removal, searching, and replacement.

## Features

- Generic `void *` storage
- Dynamic resizing
- Push and pop operations, both front and back
- Insert and erase
- Element replacement
- Element searching
- Automatic capacity growth and shrinking
- Configurable element size
- API-style responses

## Requirements

- C11 or newer (the makefile uses C17)
- GCC or Clang, (for clang change the makefile accordingly)
- Make

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

## Usage

```C
#include "vector.h"

int main()
{
    Vector vec;
    
    Vector_init(&vec, sizeof(int));

    int value = 42;

    vector_push_back(&vec, &value);

    int out;
    vector_pop_back(&vec, &out);
    printf("%d\n", out);

    vector_destroy(&vec);

    return 0;
}
```

## API

| Function | Description |
|---|---|
| `vector_init` | Initialize a vector |
| `vector_destroy` | Free vector resources |
| `vector_push_back` | Add an element to the end |
| `vector_push_front` | Add an element to the beginning |
| `vector_pop_back` | Remove the last element |
| `vector_pop_front` | Remove the first element |
| `vector_insert` | Insert an element at an index |
| `vector_erase` | Remove an element at an index |
| `vector_get` | Access an element |
| `vector_replace` | Replace an element |
| `vector_search` | Search for an element |
| `vector_contains` | Check whether an element exists |
| `vector_clear` | Remove all elements |

## Design

The vector stores elements in a contiguous dynamically allocated memory region.

Each vector tracks:

- Current number of elements (`size`)
- Allocated capacity (`capacity`)
- Size of each element (`elem_size`)

Elements are stored generically using `void *`, allowing the vector to store different C data types.

### Resizing

The vector grows when its capacity is exhausted and shrinks when the amount of unused capacity becomes sufficiently large.

## Complexity

| Operation | Complexity |
|---|---:|
| `push_back` | O(1) amortized |
| `push_front` | O(n) |
| `insert` | O(n) |
| `pop_back` | O(1) |
| `pop_front` | O(n) |
| `erase` | O(n) |
| `get` | O(1) |
| `replace` | O(1) |
| `search` | O(n) |
| `contains` | O(n) |

## Testing

Soon

## Benchmark

Soon

## Roadmap

- [x] Generic `void *` vector
- [x] Basic operations
- [x] Automatic resizing
- [ ] Tests
- [ ] Benchmarking
- [ ] Documentation improvements

## Future ideas

- [ ] Make the `void *` into `macro functions`

## License

This project is licensed under the MIT License.
See [LICENSE](LICENSE) for details.