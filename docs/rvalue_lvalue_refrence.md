![](./images/rl1.png)
![](./images/rl2.png)
![](./images/rl3.png)
![](./images/rl4.png)
![](./images/rl5.png)
![](./images/rl6.png)

# **lvalue, rvalue, lvalue reference, rvalue reference** in C++ 

---

### 1. **lvalue (locator value)**

- **Definition:** An expression that refers to a memory location (basically something you can take the address of).
- **Example:** Variables, array elements, dereferenced pointers.
- **Use:** Has an identifiable location in memory.

```cpp
int x = 10;     // 'x' is an lvalue
x = 20;         // You can assign something to x (because it's an lvalue)

int* p = &x;    // You can take address of x, so x is lvalue
```

---

### 2. **rvalue (read value or right value)**

- **Definition:** An expression that is not an lvalue — usually a temporary value or literal; it doesn’t have a persistent memory address.
- **Example:** literals, the result of expressions, returned by some functions.

```cpp
int y = 5;      // 5 is an rvalue (a literal)
int z = x + y;  // 'x + y' is an rvalue (temporary expression result)
```

You **cannot assign** to rvalues—they don’t refer to a lasting memory location.

---

### 3. **lvalue references**

- Syntax: `int& ref = x;`
- An lvalue reference can bind only to an lvalue.
- Used to refer to an existing object, can be used to modify the original object.

```cpp
int x = 10;
int& ref = x;  // ref is an lvalue reference to x
ref = 20;      // changes x to 20
```

---

### 4. **rvalue references** (introduced in C++11)

- Syntax: `int&& ref = 5;`
- An rvalue reference can bind **only to rvalues** (temporaries).
- Used to implement move semantics and perfect forwarding. Enables you to "steal" resources from temporaries rather than copying them.

```cpp
int&& rref = 10;  // rref binds to temporary 10 (an rvalue)
```

---

### **Why do we need rvalue references?**

Imagine you have a class with dynamically allocated memory:

```cpp
class MyString {
    char* data;
public:
    MyString(const char* s) {
        data = new char[strlen(s) + 1];
        strcpy(data, s);
    }
    ~MyString() { delete[] data; }
    
    // Copy constructor (expensive)
    MyString(const MyString& other) {
        data = new char[strlen(other.data) + 1];
        strcpy(data, other.data);
    }
    
    // Move constructor (efficient)
    MyString(MyString&& other) noexcept {
        data = other.data; // steal resource pointer
        other.data = nullptr;  // nullify source to avoid double delete
    }
};
```

- **Copy constructor** duplicates data (expensive!).
- **Move constructor** transfers ownership of the existing data without copying (efficient!).

Without rvalue references, you’d be forced to copy all the time, which is slow.

---

### Summary Table

| Term             | Can Bind To       | Purpose                       | Example                    |
|------------------|-------------------|-------------------------------|----------------------------|
| **lvalue**       | Objects with name  | Refer to existing object       | `int x;`                   |
| **rvalue**       | Temporaries       | Temporary values, literals     | `5`, `x + y`               |
| **lvalue reference**  | lvalues only       | Refer to an existing object, modifiable | `int& ref = x;`           |
| **rvalue reference**  | rvalues only       | Implement move semantics         | `int&& rref = 10;`         |

---

### What problems does this solve?

- **Performance:** Move semantics (via rvalue references) avoid unnecessary copying for temporary objects.
- **Better resource management:** Enables efficient transfer of ownership (in classes managing dynamic memory or resources).
- **Cleaner code:** Allows perfect forwarding in templates, reducing code duplication.

---

### Quick example of move semantics

```cpp
MyString s1("Hello");
MyString s2 = std::move(s1);  // Moves s1 into s2, no deep copy
```

`std::move` casts an lvalue to an rvalue reference, enabling move semantics.

---

---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------


Great question! Usually, **lvalues** appear on the **left** side of an assignment, but they can definitely appear on the **right** side as well because they have a value and can be read.

### Simple example of an **lvalue on the right side**:

```cpp
int x = 10;    // x is an lvalue on the left side
int y = x;     // x is an lvalue on the right side now
```

Here, `x` is **an lvalue used on the right side** of the assignment to provide the value that goes into `y`.

---

### Why can lvalues appear on the right?

- Because **lvalues have a value** (stored in some memory), so you can read/use them in expressions.
- The assignment takes a value from the right (whether lvalue or rvalue) to assign to the left (which must be an lvalue).

---

### Another example:

```cpp
int a = 5;
int b = 10;
int c = a + b;  // 'a' and 'b' are lvalues appearing on the right side
```

Here `a` and `b` are lvalues used to calculate the right-side expression of the assignment to `c`.

---

**Summary:**

- **Lvalues can appear on the left or right of assignments.**
- They **must have a memory location**.
- **Rvalues only appear on the right.**

---


---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------

Great question! **`std::vector<bool>`** is a special case in C++ with a proxy type for its elements, which changes how lvalues and rvalues behave.

### Reminder: Normally, **rvalues cannot be on the left side** of assignment.

---

### What about `std::vector<bool>`?

- It does **not return a real reference (`bool&`)** on `operator[]` because `bool` is just 1 bit and can’t be referenced directly.
- Instead, **`vector<bool>::operator[]` returns a proxy object**, often named `reference` (a small helper class).
- This proxy **behaves like a reference**, allowing you to assign to it.

### Example:

```cpp
#include <vector>
#include <iostream>

int main() {
    std::vector<bool> v(1, false);

    // v[0] returns a proxy object (like a reference):
    v[0] = true;   // OK: assigning 'true' to proxy object

    // But what about rvalue on the left side?

    // std::move(v[0]) is an rvalue proxy:
    std::move(v[0]) = false;  // Also OK! Because operator= is defined on the proxy

    std::cout << v[0] << "\n";  // prints 0 (false) because we assigned false via rvalue
}
```

---

### What’s going on?

- `v[0]` is a **proxy object**, which is an **lvalue**.
- `std::move(v[0])` returns an **rvalue proxy object**.
- The proxy implements an **assignment operator** for itself, so **you can assign to the rvalue proxy**.

---

### So, in `std::vector<bool>`:

- The **proxy object** may **allow rvalues on the left side of assignment** because it overloads `operator=`.
- This is different from normal built-in types where **rvalues cannot be assigned to**.

---

### Summary:

| Expression            | Can sit at left side of assignment?                     |
|-----------------------|---------------------------------------------------------|
| lvalue (normal bool)  | Yes                                                     |
| rvalue (like `true`)  | No                                                      |
| `v[0]` (proxy lvalue) | Yes                                                     |
| `std::move(v[0])` (proxy rvalue) | Yes, because proxy has operator= overload |

---

This is an **exception due to the proxy design in `std::vector<bool>`**.

---


---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------

This little toy is just showing you how overload‐resolution treats lvalues, rvalues and (non-)constness:

```cpp
void foo(int&   lvalueref)  { cout << "in Lvalueref foo!\n"; }
void foo(int&&  rvalueref)  { cout << "in Rvalueref foo!\n"; }

int main() {
    int i   = 3;
    foo(i);    // i is a non-const lvalue → binds to foo(int&), prints “in Lvalueref foo!”
    foo(3);    // 3 is a prvalue         → binds to foo(int&&), prints “in Rvalueref foo!”
    const int j = i;
    foo(j);    // ?!
}
```

1. **`foo(i)`** → `i` is an _lvalue_ of type `int`, and `foo(int&)` takes a **non-const** lvalue reference, so that matches perfectly.

2. **`foo(3)`** → `3` is a _prvalue_ (rvalue) of type `int`, so it binds to the `int&&` overload.

3. **`foo(j)`** → `j` is an _lvalue_ **but** it’s `const int`.  
   - It cannot bind to `int&` (that requires a _non-const_ lvalue).  
   - It cannot bind to `int&&` (that requires an rvalue).  

Hence **neither** overload will accept `j` and the call is ill-formed: you get a compile-time “no matching function for call to `foo(const int)`” error.

If you wanted to handle const lvalues, you’d need yet another overload, e.g.:

```cpp
void foo(const int& cj) { cout << "in Const-L-value-ref foo!\n"; }
```

— that one would then catch the `foo(j)` call.


---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------


