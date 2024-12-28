In C++, marking function parameters as `const` (especially when passing them by reference or pointer) serves several important purposes:

1. **Preventing unintended modification:**  
   By declaring a parameter as `const`, you communicate both to the compiler and to other developers that this function will *not* modify the argument. This helps catch mistakes at compile time if there is any attempt to alter the value.

2. **Enabling optimizations:**  
   While not always guaranteed, in some cases knowing a parameter is `const` gives compilers more room for optimization because they “know” the function does not change that object.

3. **Allowing function overloading with const and non-const versions:**  
   In C++, you can have two functions that differ only by the constness of their parameters (especially important in member functions where `this` can be `const`).

4. **Allowing function calls with both const and non-const objects:**  
   If your function parameter is a `const` reference (e.g., `const std::string&`), you can safely pass both const and non-const objects to it. Conversely, a non-const reference parameter cannot bind to a temporary object or a const object.

---

## Example 1: Passing by const reference

Consider a function that prints a string without modifying it:

```cpp
#include <iostream>
#include <string>

// Non-const version:
void printString(std::string& str) {
    std::cout << str << std::endl;
}

// Const version:
void printStringConst(const std::string& str) {
    std::cout << str << std::endl;
}

int main() {
    std::string s = "Hello, World!";
    
    // This works fine with non-const version, but the function signature does not
    // promise that `str` won't be changed.
    printString(s);

    // This also works fine with the const version, but now the function
    // promises it will NOT modify the string.
    printStringConst(s);

    // However, notice that you CANNOT pass a temporary string to the non-const version:
    // printString("Hello, Temporary!"); // Error: cannot bind temporary to non-const reference

    // But you can pass a temporary to the const reference version:
    printStringConst("Hello, Temporary!");

    return 0;
}
```

- In the **non-const** version (`void printString(std::string& str)`), you cannot pass a temporary string (like `"Hello, Temporary!"`) because C++ does not allow a temporary to bind to a non-const reference.
- In the **const** version (`void printStringConst(const std::string& str)`), you *can* pass a temporary or a const `std::string` (or non-const `std::string`) because const references can bind to temporaries.

Moreover, if someone accidentally tries to modify the string in `printStringConst`, for example:

```cpp
void printStringConst(const std::string& str) {
    str[0] = 'h'; // This will NOT compile, str is const
    std::cout << str << std::endl;
}
```

the compiler will reject it. This helps prevent bugs where you accidentally change data that should remain unchanged.

---

## Example 2: Passing a pointer to a const object

Similarly, if you pass a pointer to a function and want to ensure the data it points to cannot be changed, use `const`:

```cpp
#include <iostream>

// This function cannot modify the contents of arr because it's a pointer to const int.
int sumArray(const int* arr, size_t size) {
    int total = 0;
    for (size_t i = 0; i < size; ++i) {
        total += arr[i];
        // arr[i] = 42;  // Error: arr[i] is const
    }
    return total;
}

int main() {
    int myArray[5] = {1, 2, 3, 4, 5};
    std::cout << sumArray(myArray, 5) << std::endl;

    // sumArray won't modify myArray because arr is pointer to const
    return 0;
}
```

Here, `arr` is a “pointer to const int”, which tells the compiler and your team that `sumArray` will not change the elements of the array.

---

## When to use const in function parameters?

- **Read-only access:** If your function only needs to read an object (and not modify it), prefer passing it as a `const` reference (or pointer to const) to:
    1. Prevent accidental modifications.
    2. Potentially allow passing temporaries.

- **Small types vs. large objects:**
    - For large objects (e.g., large structs/classes), use `const T&` to avoid expensive copies.
    - For small trivial types (e.g., `int`, `char`, `double`), passing by value (e.g. `int n`) is typically fine and might even be more efficient than passing by reference.

In summary, using `const` in function parameters is a good practice whenever you do not intend to modify the passed-in data. It not only prevents logical errors but also can lead to better-optimized and more flexible code.