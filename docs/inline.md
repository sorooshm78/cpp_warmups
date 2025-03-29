In C++, the `inline` keyword is used to suggest to the compiler that a function should be expanded inline at the point of its invocation, rather than performing a function call. This can potentially improve performance by reducing the overhead of function calls, especially for small and frequently used functions.

Here's a basic example to illustrate how `inline` functions work:

```cpp
#include <iostream>

// Function declared as inline
inline int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 5, y = 3;
    int result = add(x, y); // Function call
    std::cout << "Result: " << result << std::endl;
    return 0;
}
```

In this example:
- The `add` function is declared `inline`. This suggests to the compiler that it should be expanded in place wherever it is called, rather than generating a separate function call.
- The `add` function performs a simple addition operation.
- In `main()`, `add(x, y)` is called like a regular function, but due to the `inline` keyword, the compiler will try to expand it directly where it's called, potentially improving performance.

### Why use `inline`?

1. **Performance**: Reduces the overhead of function calls, which can be significant for small, frequently called functions.
   
2. **Small Functions**: Ideal for short functions that are simple and frequently used.

3. **Header-Only Libraries**: Useful for functions defined in header files that may be included in multiple translation units, preventing multiple definition errors.

### Example scenarios where `inline` is useful:

#### Example 1: Mathematical Operations

```cpp
inline double square(double x) {
    return x * x;
}

int main() {
    double result = square(5.5); // Inline expansion of square function
    // ...
}
```

#### Example 2: Accessors in Classes

```cpp
class Rectangle {
private:
    double width, height;
public:
    inline double area() const {
        return width * height;
    }
};
```

#### Example 3: Template Functions

```cpp
template <typename T>
inline T max(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    int maxInt = max(10, 20); // Inline expansion of max function
    // ...
}
```

### When not to use `inline`:

- **Large Functions**: Functions with large bodies or complex control flow are generally not suitable for inlining, as they can lead to code bloat and potentially reduce performance due to increased memory usage.
  
- **Virtual Functions**: Virtual functions are implicitly `inline` but are handled differently by the compiler and should not be explicitly marked as `inline`.

In summary, `inline` in C++ is a tool to optimize performance by suggesting to the compiler that a function should be expanded inline. It's beneficial for small, frequently used functions where the overhead of function calls is a concern. Use it judiciously based on the size and frequency of function calls in your codebase.