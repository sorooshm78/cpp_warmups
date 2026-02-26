`strcpy` is a C-style string function (from `<cstring>`) that copies a null-terminated string from `src` into `dest`, including the ending `'\0'` character. [en.cppreference](https://en.cppreference.com/w/cpp/string/byte/strcpy.html)

## Why `strcpy` is not safe
The big problem is that `strcpy(dest, src)` has **no idea** how large `dest` is, so if `src` is longer than the destination buffer, it will write past the end and cause undefined behavior (memory corruption, crashes, security bugs). [en.cppreference](https://en.cppreference.com/w/cpp/string/byte/strcpy.html)
It’s also undefined behavior if the source and destination memory regions overlap. [en.cppreference](https://en.cppreference.com/w/cpp/string/byte/strcpy.html)

## Example of the bug (overflow)
```cpp
#include <cstring>
#include <iostream>

int main() {
    char src[] = "this is a long string";
    char dest [sternumiot](https://sternumiot.com/iot-blog/strcpy-and-strncpy-c-functions-syntax-examples-and-security-best-practices/);                 // too small
    std::strcpy(dest, src);       // undefined behavior: buffer overflow
    std::cout << dest << "\n";
}
```
This is unsafe because `dest` is not large enough, and the standard explicitly says behavior is undefined if the destination array isn’t large enough. [en.cppreference](https://en.cppreference.com/w/cpp/string/byte/strcpy.html)

## Safer alternatives (C++)
### 1) Prefer `std::string` (recommended in C++)
If you can use C++ strings, you avoid manual buffer sizing:
```cpp
#include <string>
#include <iostream>

int main() {
    std::string src = "this is a long string";
    std::string dest = src;   // safe copy, grows as needed
    std::cout << dest << "\n";
}
```

### 2) If you must copy into a fixed `char[]`, use bounded copy + force null-termination
A common safe pattern is `strncpy` with explicit bounds and manual `'\0'` termination:
```cpp
#include <cstring>
#include <iostream>

int main() {
    const char* src = "this is a long string";
    char dest [sternumiot](https://sternumiot.com/iot-blog/strcpy-and-strncpy-c-functions-syntax-examples-and-security-best-practices/);

    std::strncpy(dest, src, sizeof(dest) - 1);
    dest[sizeof(dest) - 1] = '\0';   // ensure termination

    std::cout << dest << "\n";       // prints a truncated string safely
}
```
This avoids writing past the buffer by limiting how many bytes get copied and then ensuring `dest` is null-terminated. [geeksforgeeks](https://www.geeksforgeeks.org/c/why-strcpy-and-strncpy-are-not-safe-to-use/)

-------------------------------------------------

**`strcpy`** is a classic C function (also available in C++) that copies a **null-terminated string** from source to destination.

Header: `<cstring>` (or `<string.h>` in C)

Prototype:
```cpp
char* strcpy(char* dest, const char* src);
```

It copies **everything** from `src` (including the terminating `'\0'`) into `dest` — **without any length check**.

### Why strcpy is unsafe / dangerous

It **does not know** how big the `dest` buffer is.

If `src` is longer than the space available in `dest` → **buffer overflow** → classic security vulnerability.

Real-world consequences:
- Program crash
- Memory corruption
- Security exploits (arbitrary code execution in worst case)

### Classic strcpy danger example

```cpp
#include <iostream>
#include <cstring>

int main() {
    char buffer[12] = "Hello";           // only 12 bytes available

    const char* dangerous = "This is way too long for this tiny buffer!!!";

    strcpy(buffer, dangerous);           // ← BOOM!

    std::cout << buffer << "\n";         // undefined behavior
}
```

→ Writes **way past** the end of `buffer` → overwrites whatever is next on the stack (return address, other variables, …)

### Attempted "safer" version: strncpy (very common mistake)

```cpp
char buffer[12];
strncpy(buffer, dangerous, 12);     // most people write this
```

**Problems with strncpy**:

1. **Does NOT guarantee null-termination** if the source is ≥ n characters long
2. **Padds with zeros** if source is shorter → slow & unnecessary
3. Very easy to create **non-null-terminated** strings → later `strlen()`, `strcat()`, `printf("%s")` → crash or read garbage

So `strncpy` trades **one bug pattern** (overflow) for **another very nasty one** (missing null terminator).

Most security experts now say: **avoid strncpy too** (except in very specific cases).

### Modern safe alternatives (2025–2026 recommendations)

| Method                  | Portability       | Recommended for          | Notes / Behavior when too long                     |
|-------------------------|-------------------|---------------------------|-----------------------------------------------------|
| `std::string`           | Excellent (C++98+) | **Almost everything**     | Automatic memory management, no buffer overflows    |
| `std::string_view` + `copy` | C++17+         | Read-only views           | —                                                   |
| `strncpy_s` / `strcpy_s`| Microsoft + C11 Annex K | Windows / some embedded   | Fails + sets error (doesn't truncate silently)      |
| `strlcpy`               | BSD / macOS, many libraries | Unix-style code           | Always null-terminates, returns bytes needed        |
| Manual + `std::copy_n`  | Excellent         | Fixed-size buffers        | Most explicit & clear                               |

#### Best & cleanest solution in modern C++ (preferred)

```cpp
#include <iostream>
#include <string>
#include <string_view>

int main() {
    std::string safe = "This is very long but completely safe";
    
    // Most convenient
    std::string copy1 = safe;

    // From literal or char*
    std::string copy2("short");

    // From fixed buffer (safest way)
    char buf[20];
    std::string_view sv = "Hello dangerous world";
    
    // Option A – safest & clearest
    size_t len = std::min(sv.size(), sizeof(buf)-1);
    std::copy_n(sv.data(), len, buf);
    buf[len] = '\0';

    // Option B – very common & clean
    snprintf(buf, sizeof(buf), "%.*s", 
             static_cast<int>(std::min(sv.size(), sizeof(buf)-1)), sv.data());

    std::cout << "Safe copy: " << buf << "\n";
}
```

#### strcpy_s example (Microsoft / C11 Annex K)

```cpp
#include <cstring>    // or <string.h>
#include <iostream>

int main() {
    char buf[12];

    errno_t err = strcpy_s(buf, sizeof(buf), "Short");     // OK
    if (err != 0) std::cout << "Copy failed!\n";

    // This one fails safely
    err = strcpy_s(buf, sizeof(buf), "This string is way too long");
    if (err != 0) {
        std::cout << "Prevented overflow! Error code: " << err << "\n";
        // buf is left in defined (empty or partial) state
    }
}
```

### Quick rule of thumb – 2026 style

- Want **easy + safe**? → Use `std::string` (99% of cases)
- Must use **C-style fixed buffer**? → Use `snprintf`, `strlcpy`, or `std::copy_n` + manual null
- Need Windows-only code? → `strcpy_s` / `strncpy_s` is acceptable
- Never use plain `strcpy` in new code
- Avoid `strncpy` unless you **really** understand its traps
