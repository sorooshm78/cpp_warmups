**Title: How Does `new` Know How Many Bytes to Allocate?  
Understanding the `new` Expression vs. `operator new` in C++**

### Introduction  
One of the most common points of confusion when learning C++ memory management is this question:

> “If `operator new()` only receives a `size_t` (number of bytes) and returns untyped `void*`, how does it know whether to allocate 4 bytes for an `int`, 1 byte for a `bool`, or 24 bytes for a custom class?”

The answer is simple but important: **the `operator new` function itself does NOT know the type**. The compiler does all the work before calling it.

### Two Completely Separate Layers

C++ separates memory allocation into two distinct layers:

1. **The `new` Expression** (what you write in your code)  
   - This is the high-level, type-aware part.  
   - Examples: `new int`, `new bool`, `new std::string`, `new MyClass`.

2. **The Allocation Function `operator new`** (low-level raw memory provider)  
   - This is the function that actually asks the operating system/heap for memory.  
   - Signature: `void* operator new(std::size_t size);`  
   - It deals only with **raw, uninitialized, untyped memory** (exactly as the standard text says).

### Step-by-Step: What Really Happens

When the compiler sees this line:

```cpp
int* p = new int;
```

It internally translates it into roughly the following steps:

1. **Calculate the size at compile time**  
   `sizeof(int)` → usually 4 bytes (depends on platform)

2. **Call the allocation function**  
   `void* raw = ::operator new(sizeof(int));`   // passes 4

3. **Construct the object in the raw memory**  
   For built-in types like `int` and `bool`, no constructor is called (they are already “constructed” once memory exists).  
   For class types, the constructor is called on that memory.

4. **Return the properly typed pointer**  
   `int* p = static_cast<int*>(raw);`

The same process happens for `bool`:

```cpp
bool* q = new bool;     // compiler calculates sizeof(bool) → usually 1 byte
                        // calls operator new(1)
```

### Visual Summary

| Code you write       | What the compiler calculates | What is passed to `operator new` | Memory returned |
|----------------------|------------------------------|----------------------------------|-----------------|
| `new int`            | `sizeof(int)` = 4            | 4 bytes                          | raw `void*`     |
| `new bool`           | `sizeof(bool)` = 1           | 1 byte                           | raw `void*`     |
| `new double`         | `sizeof(double)` = 8         | 8 bytes                          | raw `void*`     |
| `new MyClass`        | `sizeof(MyClass)`            | size of your class               | raw `void*`     |
| `new int[100]`       | `sizeof(int) * 100 + overhead` | total bytes                    | raw `void*`     |

### You Can Even See It Yourself

You can overload `operator new` for a class and print the size:

```cpp
#include <iostream>
#include <cstdlib>

class Demo {
public:
    static void* operator new(std::size_t size) {
        std::cout << "Allocating " << size << " bytes for Demo\n";
        return std::malloc(size);
    }

    static void operator delete(void* p) {
        std::free(p);
    }
};

int main() {
    Demo* d = new Demo;     // prints: Allocating 1 or 4 or 8 bytes... (whatever sizeof(Demo) is)
    delete d;
}
```

Notice: the `operator new` function **never** receives the type `Demo` — it only receives the byte count that the compiler already calculated.

### Key Takeaways

- The **compiler** is responsible for knowing the type and calculating `sizeof(T)`.
- The **allocation function** (`operator new`) only ever sees a number of bytes — it is completely type-blind.
- This separation is exactly why the standard says:  
  > “The allocation and deallocation functions deal in untyped and uninitialized memory… they take arguments or return values of type `void*`.”
- The `new` expression is the bridge between the typed-object world and the raw-memory world.

### Bonus: `new[]` Works the Same Way

```cpp
int* arr = new int[50];   // compiler passes (sizeof(int) * 50 + possible array overhead)
```

The extra overhead (usually a few bytes to store the array length) is also calculated by the compiler before calling `operator new[]`.

---

**Conclusion**  
So the next time you wonder “how does `new` know the size?”, remember: **it doesn’t**. The compiler knows, calculates everything at compile time, and then hands just the byte count to the low-level allocator. That clean separation is one of the beautiful designs of C++ memory management.
