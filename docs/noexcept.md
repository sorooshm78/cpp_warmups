The `noexcept` keyword in C++ is used to specify whether a function is guaranteed not to throw exceptions. Introduced in C++11, it provides a way to declare the exception-safety of functions, which can help with optimizations and improving program reliability.

## Why Use `noexcept`?

1. **Optimization**: The compiler can optimize code better when it knows that certain functions won't throw exceptions. For example, move constructors and move assignment operators marked as `noexcept` can be utilized in more optimization scenarios, such as in the implementation of standard library containers like `std::vector`.

2. **Exception Safety**: Marking functions as `noexcept` communicates to other developers and the compiler about the function's exception guarantees, improving code reliability and maintainability.

3. **Program Termination**: If a `noexcept` function does throw an exception, `std::terminate` is called, leading to program termination. This enforces strict exception safety guarantees.

## Syntax

There are two primary ways to use `noexcept`:

1. **As a specifier**:
   ```cpp
   void myFunction() noexcept;
   ```

2. **With a condition**:
   ```cpp
   void myFunction() noexcept(condition);
   ```
   Here, `condition` is a compile-time constant expression that evaluates to `true` or `false`.

## Examples

### Basic Usage

Here's a simple example demonstrating how to use `noexcept`:

```cpp
#include <iostream>
#include <vector>
#include <string>

class MyClass {
public:
    // Constructor that does not throw exceptions
    MyClass() noexcept {
        // Initialization code
    }

    // Function that does not throw exceptions
    void display() const noexcept {
        std::cout << "MyClass display function called.\n";
    }

    // Function that might throw exceptions
    void riskyOperation() {
        throw std::runtime_error("An error occurred!");
    }
};

int main() {
    MyClass obj;
    obj.display();

    try {
        obj.riskyOperation();
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << '\n';
    }

    return 0;
}
```

**Explanation:**

- `MyClass` constructor and `display` function are marked with `noexcept`, indicating they won't throw exceptions.
- `riskyOperation` is a regular function that can throw exceptions.
- In `main`, calling `display` is safe and guaranteed not to throw, while `riskyOperation` is enclosed in a `try-catch` block to handle potential exceptions.

### Conditional `noexcept`

You can also specify `noexcept` based on a condition, which is particularly useful for template programming:

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void process(T&& t) noexcept(std::is_nothrow_move_constructible<T>::value) {
    // Function implementation
}

struct NoThrowMove {
    NoThrowMove() {}
    NoThrowMove(const NoThrowMove&) {}
    NoThrowMove(NoThrowMove&&) noexcept {} // Move constructor is noexcept
};

struct MayThrowMove {
    MayThrowMove() {}
    MayThrowMove(const MayThrowMove&) {}
    MayThrowMove(MayThrowMove&&) {} // Move constructor might throw
};

int main() {
    std::cout << std::boolalpha;
    std::cout << "process<NoThrowMove> is noexcept: "
              << noexcept(process(NoThrowMove())) << '\n';
    std::cout << "process<MayThrowMove> is noexcept: "
              << noexcept(process(MayThrowMove())) << '\n';
    return 0;
}
```

**Explanation:**

- The `process` function is conditionally marked as `noexcept` based on whether the type `T` has a `noexcept` move constructor.
- `NoThrowMove` has a `noexcept` move constructor, so `process<NoThrowMove>` is `noexcept`.
- `MayThrowMove` has a move constructor that might throw, so `process<MayThrowMove>` is not `noexcept`.

**Output:**
```
process<NoThrowMove> is noexcept: true
process<MayThrowMove> is noexcept: false
```

### Interaction with Standard Library

Marking move constructors and move assignment operators as `noexcept` can significantly impact the performance of standard library containers. For example, `std::vector` uses move operations during reallocation only if they are `noexcept`. Otherwise, it falls back to copy operations to ensure exception safety.

```cpp
#include <vector>
#include <string>
#include <iostream>

struct MoveNoexcept {
    MoveNoexcept() {}
    MoveNoexcept(const MoveNoexcept&) {}
    MoveNoexcept(MoveNoexcept&&) noexcept {} // noexcept move constructor
};

struct MoveMayThrow {
    MoveMayThrow() {}
    MoveMayThrow(const MoveMayThrow&) {}
    MoveMayThrow(MoveMayThrow&&) {} // move constructor might throw
};

int main() {
    std::vector<MoveNoexcept> vec1;
    vec1.emplace_back();
    vec1.emplace_back();

    std::vector<MoveMayThrow> vec2;
    vec2.emplace_back();
    vec2.emplace_back();

    std::cout << "vec1 size: " << vec1.size() << '\n';
    std::cout << "vec2 size: " << vec2.size() << '\n';

    return 0;
}
```

**Explanation:**

- `MoveNoexcept` has a `noexcept` move constructor, allowing `std::vector` to use move operations during reallocation, which can be more efficient.
- `MoveMayThrow` does not have a `noexcept` move constructor, so `std::vector` will use copy operations instead, which might be less efficient.

## Best Practices

1. **Mark Functions as `noexcept` When Possible**: If a function is guaranteed not to throw, mark it as `noexcept`. This enables compiler optimizations and conveys intent.

2. **Use Conditional `noexcept` in Templates**: When writing generic code, use conditional `noexcept` to propagate the exception guarantees of the underlying types.

   ```cpp
   template <typename T>
   void swap(T& a, T& b) noexcept(noexcept(a.swap(b))) {
       a.swap(b);
   }
   ```

3. **Be Cautious with `noexcept`**: Only mark a function as `noexcept` if you are certain it won't throw. If a `noexcept` function does throw, `std::terminate` is called, leading to program termination.

4. **Use `noexcept` for Move Constructors and Move Assignment Operators**: This allows standard library containers to prefer move operations over copy operations, enhancing performance.

## Conclusion

The `noexcept` keyword is a powerful feature in C++ that helps in writing more efficient and reliable code by specifying the exception guarantees of functions. Proper use of `noexcept` can lead to better optimizations and clearer communication of intent regarding exception safety.