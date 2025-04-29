![](./images/rl1.png)
![](./images/rl2.png)
![](./images/rl3.png)
![](./images/rl4.png)
![](./images/rl5.png)
![](./images/rl6.png)
![](./images/rl7.png)
![](./images/rl8.png)
![](./images/rl9.png)
![](./images/rl10.png)

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

![](./images/rl6.png)

In C++, an **rvalue** can bind to a function parameter that is a **const lvalue reference** (`const &`). This is a special rule in C++ that allows temporary objects (rvalues) to be passed to functions expecting `const &` parameters without requiring an explicit copy.

### In C++ the reference-binding rules are designed around two ideas:
1. Safety: a reference must never dangle.
2. Efficiency & convenience: you should be able to pass both named objects (lvalues) and unnamed temporaries (rvalues) to ordinary functions without needless copying.

### Key Points:
1. **Rvalues** are temporary objects (e.g., literals, function returns, or expressions like `x + y`).
2. **`const &`** (const lvalue reference) can bind to both **lvalues** and **rvalues**.
3. This avoids unnecessary copies while ensuring the function cannot modify the original temporary object.

---

### Example:
```cpp
#include <iostream>

// Function taking a const lvalue reference
void printValue(const int &x) {
    std::cout << "Value: " << x << std::endl;
}

int getNumber() {
    return 42;  // Returns an rvalue
}

int main() {
    int a = 10;  // 'a' is an lvalue

    // Case 1: Passing an lvalue (works)
    printValue(a);

    // Case 2: Passing an rvalue (works due to const &)
    printValue(20);  // 20 is an rvalue

    // Case 3: Passing a function return (rvalue)
    printValue(getNumber());

    // Case 4: Passing an expression (rvalue)
    printValue(a + 5);  // (a + 5) is an rvalue

    return 0;
}
```

### Output:
```
Value: 10
Value: 20
Value: 42
Value: 15
```

### Explanation:
- The function `printValue(const int &x)` accepts both **lvalues** (`a`) and **rvalues** (`20`, `getNumber()`, `a + 5`).
- The `const &` ensures that the function cannot modify the passed object, making it safe for temporaries.
- This rule exists to improve efficiency by avoiding copies when passing temporary objects.

### Contrast with Non-const Lvalue References:
If the function took a **non-const lvalue reference** (`int &x`), it would **not** accept rvalues:
```cpp
void modifyValue(int &x) {
    x += 10;
}

int main() {
    int a = 10;
    modifyValue(a);  // OK: lvalue
    modifyValue(20); // ERROR: cannot bind rvalue to non-const &
}
```
This fails because modifying a temporary (rvalue) makes no sense, so C++ disallows it.

### Summary:
- **`const &`** can bind to **rvalues** (temporaries).
- **Non-const `&`** can only bind to **lvalues** (named objects).
- This rule enables efficient passing of temporary objects without copying.

---------------
---------------
---------------
---------------
---------------

![](./images/rl7.png)
![](./images/rl8.png)
```
#include <iostream>

using namespace std;

void foo(int&  lvalueRef)  { std::cout << "in l-value  foo\n"; }
void foo(int&& rvalueRef)  { std::cout << "in r-value  foo\n"; }

void bar(int&& i)
{
    std::cout << "-- inside bar\n";

    std::cout << "foo(i):";
    foo(i); // i has a *name* → it is an **lvalue** → calls foo(int&)

    std::cout << "foo(std::move(i)): ";
    foo(std::move(i)); // std::move turns it back into an **rvalue** → calls foo(int&&)
}

int main()
{
    bar(3); // 3 is an rvalue, so bar(int&&) is selected
}
```

Here’s what’s going on, line-by-line, and why the two calls to `foo` pick different overloads.

---

### 1.  Two overloads of `foo`

```cpp
void foo(int&  lvalueRef) { std::cout << "in l-value  foo\n"; }
void foo(int&& rvalueRef) { std::cout << "in r-value  foo\n"; }
```

* **`foo(int& )`** is selected when the argument is an **lvalue** (something that has a persistent identity you can take the address of).
* **`foo(int&&)`** is selected when the argument is an **xvalue/rvalue** (a “temporary” object that can be moved-from).

---

### 2.  `bar` takes an rvalue-reference **parameter**

```cpp
void bar(int&& i)
{
    std::cout << "-- inside bar\n";
```

*Because* you write `bar(3)` in `main`, overload resolution chooses `bar(int&&)`.  
Inside `bar` the parameter is named **`i`**. And here’s the first key rule:

> **Any named variable is always an lvalue** when you refer to it, no matter its original type.

So inside `bar` the expression `i` is an **lvalue of type `int`**.

---

### 3.  First call – `foo(i)`

```cpp
    foo(i);          // i is an lvalue → calls foo(int&)
```

Because `i` is an lvalue, the compiler prefers the `foo(int&)` overload, printing

```
in l-value  foo
```

---

### 4.  Second call – `foo(std::move(i))`

```cpp
    foo(std::move(i));  // std::move casts i to an rvalue → calls foo(int&&)
```

`std::move` doesn’t actually move anything; it’s just `static_cast<int&&>(i)`. The cast tells the compiler, “Treat this expression as an rvalue.” Now overload resolution picks `foo(int&&)`, printing

```
in r-value  foo
```

---

### 5.  `main` drives the whole thing

```cpp
int main()
{
    bar(3);   // 3 is a prvalue → matches bar(int&&)
}
```

* `3` is a pure rvalue (prvalue).  
* Overload resolution chooses `bar(int&&)`, binding that temporary to the parameter `i`.  
* Everything else follows as described.

---

### Complete runtime output

```
-- inside bar
foo(i):in l-value  foo
foo(std::move(i)): in r-value  foo
```

(The lack of a newline after “foo(i):” comes from the code’s `cout` formatting.)

---

## Why this matters

* **Correct overload selection**: Choosing between copying and moving (or between other lvalue/rvalue-specific operations) is the backbone of modern C++ efficiency.
* **Value-category awareness**: Remember that a variable’s declared type (`T&&`) and its *expression category* (lvalue/rvalue in a given context) are different things. You often need `std::move` or, in templates, `std::forward` to get the category you want.
* **Safety**: Don’t call `std::move` unless you really intend to treat the object as expendable after the move; it signals to readers (and to the compiler) that the value can be pilfered.


----------------------------
----------------------------
----------------------------
----------------------------
----------------------------

![](./images/rl9.png)

**Slide text (literal English translation)**

```
std::move

std::move is a cast to an r-value reference that preserves const-volatile (“cv”) qualifiers.

std::move doesn’t move
std::move merely prepares its argument to be moved, but it doesn’t actually
perform any move on its own!

std::string s2 = std::move(s1);
// The “move” happens here, when the move-constructor is called
```
(CppCon 2024 – “Back to Basics: R-value and Move Semantics”, Amir Kirsh)

---

### What it means

1. **What `std::move` really does**  
   It is nothing more than  
   ```cpp
   static_cast<T&&>(expr)
   ```  
   In other words, it tells the compiler “treat this expression as an r-value,” while keeping any `const`/`volatile` qualifiers intact. It does **not** relocate data.

2. **Why “`std::move` doesn’t move”**  
   Actual movement of resources occurs only when a *move-constructor* or *move-assignment operator* receives an r-value. `std::move` simply makes that possible by changing the **value category** of the argument.

3. **The example on the slide**  
   ```cpp
   std::string s2 = std::move(s1);
   ```  
   * `std::move(s1)` casts `s1` to an r-value.  
   * The move-constructor of `std::string` is then invoked to transfer `s1`’s internal buffer to `s2`.  
   * Therefore the real transfer happens in the initialization of `s2`, not inside `std::move`.

4. **Practical takeaway**  
   Use `std::move` when you want to hand an object over as a disposable temporary. Afterward, the source object remains in a *valid-but-unspecified* state, so only perform operations that are guaranteed safe (e.g., destroy it, assign a new value, or call methods documented as valid post-move).

**Bottom line:** `std::move` only *labels* the object as movable; the *move operation* itself is carried out by the receiving constructor or assignment operator.



---------------
---------------
---------------
---------------
---------------

![](./images/rl10)

### What the slide is asking

The slide shows a tiny generic function

```cpp
template<class T>
void foo(T& a, T& b) {
    T temp = std::move(a);   // ← here
    // do some more stuff
}
```

and then asks “Is it **valid** to `std::move` an **l-value** reference?”  
(The parameters `a` and `b` are both ordinary l-value references, `T&`.)

### How `std::move` really works

* `std::move(x)` **does not move** anything by itself.  
  It is nothing more than a cast:  

  ```cpp
  template<class U>
  constexpr std::remove_reference_t<U>&& move(U&& u) noexcept
  { return static_cast<std::remove_reference_t<U>&&>(u); }
  ```

* In other words it turns *any* expression you give it—even an l-value—into an r-value **expression**.  
  Whether an actual *move* happens depends on what constructor/assignment operator is selected **later**.

Because that cast is always legal, `std::move(a)` is always **well-formed C++**.  
So answers A and B (“won’t compile” / “warns”) are wrong.

### Is it Undefined Behaviour?

No. UB in C++ comes from using an object in a way the Standard forbids (out-of-bounds, double-free, etc.). Merely putting an object into a *moved-from* state is **defined behaviour**: the Standard says a moved-from object is still valid but its value is unspecified. So answer C is wrong as well.

### When moving from an l-value reference is *legit*

It can absolutely be the right thing to do—**if you really intend to give up the old value of `a` inside `foo()`.**

Typical, totally legitimate patterns:

```cpp
// Classic swap:
T temp = std::move(a);
a       = std::move(b);
b       = std::move(temp);
```

```cpp
// Efficient “consume-and-produce” algorithm:
out_vec.push_back(std::move(a));   // steal a’s guts
a.reinitialize();                  // give a fresh state
```

```cpp
// std::iter_swap’s inner loop eventually does the same trick
```

Notice all three cases satisfy the crucial rule:

> **After you move from an object, either destroy it or assign it a new value  
>  *before* you (or anyone else) rely on its contents.**

That rule is under *your* control inside `foo()`, so the cast is safe and useful.

### When it would be a mistake

* If `foo()` later needs the original value of `a`.  
* If the caller assumes `a` is untouched after the call and you don’t document otherwise.
* If `a` and `b` might alias the same object and the algorithm is not prepared for self-move.

Those are **logical** errors, not language-level UB: the code still compiles and runs, it just doesn’t do what you meant.

### So which multiple-choice answer is right?

> **D. “Yes, it can be legit in some cases.”**
