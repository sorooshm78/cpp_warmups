`strcpy` is for C-strings; `memcpy` is for raw bytes. [stackoverflow](https://stackoverflow.com/questions/2898364/strcpy-vs-memcpy)

## Conceptual difference

- `strcpy(char* dest, const char* src)`  
  - Copies a **null-terminated string**.  
  - Reads from `src` and copies until it sees the first `'\0'`, and also copies that `'\0'`. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)
  - Does **not** take a length; you must ensure `dest` is large enough. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)

- `memcpy(void* dest, const void* src, size_t n)`  
  - Copies **exactly `n` bytes**, no matter what the contents are. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)
  - Works on any data (structs, arrays, binary blobs), not just text. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)
  - Does **not** add or look for `'\0'`. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)

## Safety implications

- With `strcpy`:
  - Risk: if `src` is longer than the buffer behind `dest`, you overflow because there is no length limit. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)
  - Needs `src` to be a valid null-terminated string.

- With `memcpy`:
  - You control the length, so overflow risk is on *you*: you must pass the correct `n` and ensure buffers are large enough and non-overlapping. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)
  - No concept of string termination; if you copy text you must handle `'\0'` yourself.

## Simple examples

### `strcpy` example (string only)

```cpp
#include <cstring>
#include <iostream>

int main() {
    const char* src = "Hello";
    char dest[10];

    std::strcpy(dest, src);  // copies 'H','e','l','l','o','\0'
    std::cout << dest << "\n"; // prints Hello
}
```

### `memcpy` example with a string

```cpp
#include <cstring>
#include <iostream>

int main() {
    const char src[] = "Hello";
    char dest[10];

    std::memcpy(dest, src, sizeof(src)); // copies all 6 bytes, including '\0'
    std::cout << dest << "\n";           // prints Hello
}
```

Note: here we had to know `sizeof(src)` (6) to also copy the terminator.

### `memcpy` with non-string data

```cpp
#include <cstring>
#include <iostream>

struct Point {
    int x;
    int y;
};

int main() {
    Point p1{10, 20};
    Point p2{};

    std::memcpy(&p2, &p1, sizeof(Point)); // fine: raw memory copy
    std::cout << p2.x << ", " << p2.y << "\n"; // 10, 20
}
```

You cannot do this with `strcpy`, because `Point` is not a null-terminated string.

## When to use which

- Use **`strcpy`** (or better, `std::string`)  
  - When dealing with classic C strings. [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)

- Use **`memcpy`**  
  - When copying raw memory blocks (structs, arrays, buffers) of known size, or when copying a string and you explicitly know the length (including `'\0'`). [groups.google](https://groups.google.com/g/comp.lang.c.moderated/c/Aw6EA2grk_k)
