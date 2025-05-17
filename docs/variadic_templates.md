### `template<typename... T>` — Variadic Templates in C++

---

#### ✅ **What Is It?**

`template<typename... T>` is part of **variadic templates**, a C++ feature introduced in **C++11** that allows a function or class to take **a variable number of template arguments** — not just one or two, but **zero or more**.

Think of it like `varargs` in C (e.g., `printf`), but for **types** (and optionally, values).

---

### ✅ **Syntax**

```cpp
template<typename... T> // T is a *parameter pack*
```

Here, `T...` is a **template parameter pack** — a placeholder for **zero or more types**.

---

### ✅ **Why Use It? What Problem Does It Solve?**

Before C++11, you had to manually overload or specialize functions for each number of arguments:

```cpp
void print(int);
void print(int, float);
void print(int, float, std::string);
```

This becomes **repetitive and hard to maintain**.

With variadic templates, you can write one template that handles **any number** of types.

---

## ✅ Example 1: Variadic Function Template (Recursive)

```cpp
#include <iostream>

// Base case: no arguments
void print() {
    std::cout << "End of print.\n";
}

// Recursive case: one argument and a parameter pack
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first << "\n";
    print(rest...); // Recursive call
}

int main() {
    print(1, 2.5, "hello", 'A');
    return 0;
}
```

### 🔍 Output:

```
1
2.5
hello
A
End of print.
```

---

## ✅ Example 2: Class Template with Variadic Types

```cpp
#include <tuple>
#include <iostream>

template<typename... Args>
class MyContainer {
public:
    std::tuple<Args...> data;
    
    MyContainer(Args... args) : data(args...) {}
    
    void printSize() {
        std::cout << "Stored " << sizeof...(Args) << " elements.\n";
    }
};

int main() {
    MyContainer<int, double, std::string> mc(1, 3.14, "hi");
    mc.printSize();
    return 0;
}
```

### 🔍 Output:

```
Stored 3 elements.
```

---

### ✅ Real Use Cases

* **Generic print/log functions**.
* **Forwarding multiple constructor arguments (e.g., in smart pointers or containers)**.
* **Tuple-like data structures**.
* **Perfect forwarding** (like in `std::make_unique`, `std::make_shared`).

---

### ✅ Key Concepts

| Concept                 | Description                                |
| ----------------------- | ------------------------------------------ |
| `typename... T`         | A **template parameter pack** (types).     |
| `T...`                  | Expands the pack.                          |
| `sizeof...(T)`          | Returns the number of types in the pack.   |
| `T first, Args... rest` | **Pack expansion** in function parameters. |
| `print(rest...)`        | Recursively calling with unpacked args.    |

---

### ✅ What Problem Does It Solve?

It **generalizes** functions/classes to work with **any number** of types or arguments without rewriting code:

* No need to overload functions for every case.
* Cleaner, reusable, extensible generic code.
* Works great with STL (e.g., `std::tuple`, `std::apply`, `std::forward`).

---

Would you like to go deeper into:

* **Perfect forwarding with variadic templates**?
* **Using `std::forward` + parameter packs**?
* **Combining with concepts or constraints (C++20+)?**
