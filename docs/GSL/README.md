# Guideline Support Library

## link
* [microsoft-GSL](https://github.com/microsoft/GSL)
* [c-core-guideline-the-guidelines-support-library](https://www.modernescpp.com/index.php/c-core-guideline-the-guidelines-support-library/)
* [CppCoreGuidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#gsl-guidelines-support-library)
* [cpp-core-guidelines-docs.vercel.app](https://cpp-core-guidelines-docs.vercel.app/gsl)

## gsl::owner<T*>
`gsl::owner<T*>` is a utility provided by the [Guidelines Support Library (GSL)](https://github.com/microsoft/GSL), a library that implements some of the C++ Core Guidelines. The primary purpose of `gsl::owner` is to explicitly indicate ownership of a raw pointer in the code. This helps improve code readability and conveys intent, making it clear when a function or class is responsible for managing the lifetime of a pointer.

Here’s a breakdown:

- **`gsl::owner<T*>`**: Used to annotate a raw pointer to indicate that the code is responsible for managing its lifetime (e.g., allocating and deallocating memory for it).
- This is purely an annotation and does not affect runtime behavior. However, tools like static analyzers can use this information to identify potential issues, such as memory leaks or dangling pointers.

### Example 1: Without `gsl::owner`
```cpp
#include <iostream>

void deletePointer(int* ptr) {
    delete ptr; // The caller must ensure proper ownership transfer
}

int main() {
    int* data = new int(42);
    deletePointer(data); // It's unclear who owns `data`
    return 0;
}
```
The code above does not make ownership of the pointer explicit. `data` is allocated with `new`, but it’s unclear who is responsible for deleting it. This can lead to confusion or bugs.

---

### Example 2: With `gsl::owner`
```cpp
#include <gsl/gsl>

void deletePointer(gsl::owner<int*> ptr) {
    delete ptr; // It's now clear this function takes ownership
}

int main() {
    gsl::owner<int*> data = new int(42); // Ownership is explicit
    deletePointer(data);                 // Ownership transferred
    return 0;
}
```
In this version, using `gsl::owner` makes it explicit that:
1. `data` is owned and needs to be deleted at some point.
2. The `deletePointer` function takes ownership of the pointer.

---

### Example 3: Returning Ownership
When a function allocates memory and returns a pointer, `gsl::owner` can make it clear that the caller is now responsible for the resource.

```cpp
#include <gsl/gsl>

gsl::owner<int*> createInteger() {
    return new int(100); // Explicitly marks that ownership is being returned
}

int main() {
    gsl::owner<int*> num = createInteger();
    std::cout << *num << '\n';
    delete num; // Caller is responsible for deleting the allocated memory
    return 0;
}
```

---

### Example 4: Mixing with Smart Pointers
`gsl::owner` is not a replacement for smart pointers (`std::unique_ptr` or `std::shared_ptr`), but it can be useful when working with raw pointers for legacy code or performance reasons.

```cpp
#include <gsl/gsl>
#include <memory>

void transferOwnership(gsl::owner<int*> ptr) {
    delete ptr;
}

int main() {
    // Explicit ownership with gsl::owner
    gsl::owner<int*> rawPtr = new int(200);

    // Converting to a smart pointer
    std::unique_ptr<int> smartPtr(rawPtr); // Smart pointer now owns the memory

    // transferOwnership(rawPtr); // Error: rawPtr is now managed by smartPtr
    return 0;
}
```
This example highlights that once ownership is transferred to a smart pointer, you should not use the raw pointer elsewhere.

---

### Key Takeaways:
1. **Intent**: `gsl::owner` is a documentation tool that improves code clarity.
2. **Static Analysis**: Tools like Clang-Tidy can detect ownership-related issues using `gsl::owner`.
3. **Raw Pointers**: Use `gsl::owner` with raw pointers when smart pointers (e.g., `std::unique_ptr`) are not an option.
4. **Runtime Behavior**: `gsl::owner` does not change runtime behavior—it’s purely a compile-time annotation.

When possible, prefer smart pointers for automatic memory management. Use `gsl::owner` in cases where you must work with raw pointers but want to make ownership intentions explicit.

## Expects() and Ensures()
`Expects()` and `Ensures()` are part of the **Guidelines Support Library (GSL)**, designed to help you write safer, more self-documenting code. They are used for **contract programming** by defining **preconditions** (`Expects`) and **postconditions** (`Ensures`) for functions.

---

### What is `Expects()`?
`Expects()` is used to define a **precondition** for a function. A **precondition** is a condition that must be true when the function is called. If the precondition is violated, the behavior of the function is undefined, and `Expects()` signals this error.

#### Syntax:
```cpp
void Expects(bool condition);
```

#### Example:
```cpp
#include <gsl/gsl>

void divide(int a, int b) {
    Expects(b != 0); // Precondition: denominator must not be zero
    int result = a / b;
}
```
In this example:
- The function assumes `b` is not zero.
- If `b == 0`, `Expects()` will terminate the program (or invoke custom error handling based on the GSL implementation).

---

### What is `Ensures()`?
`Ensures()` is used to define a **postcondition** for a function. A **postcondition** is a condition that must be true when the function finishes execution. It helps ensure the function behaves as intended after completing its task.

#### Syntax:
```cpp
void Ensures(bool condition);
```

#### Example:
```cpp
#include <gsl/gsl>

int increment(int x) {
    int result = x + 1;
    Ensures(result > x); // Postcondition: result must be greater than input
    return result;
}
```
In this example:
- The function guarantees that the returned value will always be greater than the input.
- If the postcondition is violated, `Ensures()` will terminate the program.

---

### Difference Between `Expects()` and `Ensures()`
| **Aspect**       | **Expects()**                          | **Ensures()**                          |
|-------------------|----------------------------------------|----------------------------------------|
| **Purpose**       | Defines **preconditions** (what must be true before a function runs). | Defines **postconditions** (what must be true after a function runs). |
| **Timing**        | Checked **before** the function executes. | Checked **after** the function executes. |
| **Use Case**      | Used to ensure valid inputs to a function. | Used to ensure the function's output or state is as expected. |
| **When It Runs**  | At the start of the function.          | At the end of the function.            |

---

### Full Example
```cpp
#include <gsl/gsl>
#include <vector>
#include <iostream>

int getValueAt(const std::vector<int>& v, size_t index) {
    Expects(index < v.size()); // Precondition: index must be valid
    int value = v[index];
    Ensures(value >= 0); // Postcondition: value must be non-negative
    return value;
}

int main() {
    std::vector<int> numbers = {10, 20, 30};
    
    // Valid call
    std::cout << getValueAt(numbers, 1) << '\n'; // Output: 20
    
    // Invalid precondition
    // getValueAt(numbers, 5); // Terminates: Precondition violated
    
    return 0;
}
```

---

### How `Expects()` and `Ensures()` Work
- By default, they call `std::terminate()` if the condition is violated.
- They serve as **documentation** for readers and allow **static analysis tools** to identify contract violations during compilation or runtime.

---

### Key Takeaways:
1. **`Expects()`** ensures the function is called correctly with valid arguments.
2. **`Ensures()`** guarantees the function produces valid results and satisfies its intended behavior.
3. **Static Analysis**: Modern compilers and static analysis tools can validate contracts and catch violations early.
4. **Runtime Behavior**: Violations terminate the program unless you configure a custom handler. 

Together, they help make code more robust, self-explanatory, and easier to debug!
