![](./images/ppp1.png)
![](./images/ppp2.png)

These two images illustrate how optimizing compilers can dramatically reduce complex high-level code into very simple machine instructions — a testament to their sophistication.

---

### 📄 **Image 1 Explanation: High-Level Code**

```cpp
#include <string>

int main()
{
    std::string s("a");
    return s.size();
}
```

This C++ program:

* Includes the `<string>` header for using `std::string`.
* In `main()`, creates a string `s` with value `"a"`.
* Returns the size of the string, which is `1`.

### ⚙️ **Image 2 Explanation: Optimized Assembly Output (g++ 5.1+)**

```asm
main:
    mov     eax, 1
    ret
```

This is the **entire assembly output** from compiling the code with optimization enabled (e.g., `-O2` or `-O3` with `g++ 5.1+`):

* `mov eax, 1`: The result (`1`) is directly loaded into the return register (`eax`).
* `ret`: Return from `main()`.

---

### 🤯 Why This Is Amazing

The compiler realizes:

* `std::string s("a")` creates a known constant string `"a"`.
* `s.size()` is always `1`, and nothing in the program modifies `s`.
* Therefore, it **optimizes away all the string construction logic**, directly returning `1`.

The optimizer essentially says: “This whole function will always return 1, so why bother doing all that work?”

---

### ✅ Takeaway

This is a great example of **constant folding** and **dead code elimination**:

* The compiler does not even generate code to create the string.
* It only keeps the essential outcome.

That's why the slide is titled:

> **Optimizing Compilers Are Amazing**

Because they truly are.

-------
-------
-------

![](./images/ppp3.png)
![](./images/ppp4.png)

Great observation — and this is a subtle but fascinating point about **how compilers perform optimizations** in different contexts.

---

### 📄 **Image 3 (High-Level Code)**

```cpp
#include <string>

int main()
{
    return std::string("a").size() + std::string("b").size();
}
```

This code constructs two temporary strings `"a"` and `"b"`, calls `.size()` on each, and returns their sum — which **logically is `1 + 1 = 2`**.

---

### ⚙️ **Image 4 (Assembly Output)**

This time, the compiler **did not optimize** the result to a simple `return 2`. Instead, the output shows:

* A complex series of function calls and memory manipulations.
* Calls to `std::string` constructors.
* Safety checks and error handling logic (e.g. `std::__throw_logic_error`).

---

### 🤔 **So Why Didn't the Compiler Just Return 2?**

Here’s why:

#### 1. **Temporary Objects & Side Effects**

Each `std::string("a")` and `std::string("b")` creates a **temporary object**. Although these constructors are trivial, the compiler may be conservative — it assumes that:

* `std::string` construction might have side effects.
* `std::string::size()` might be virtual (it's not, but optimization phases can be cautious).

So it generates full construction and method-call code.

#### 2. **Separate String Instances**

Because there are **two separate string instances**, the compiler may not combine their `.size()` results at compile time unless it sees that both strings are constant and have no observable side effects.

#### 3. **Optimization Limitations**

Some compilers or optimization levels might **not perform inter-statement constant folding** when it involves temporary object construction — unless **link-time optimization (LTO)** or more aggressive settings (like `-O3` + inlining + constexpr evaluation) are enabled.

---

### ✅ When Would It Return `2`?

If you change the code to something more *obviously constexpr-friendly*, like:

```cpp
constexpr int f() {
    return std::string("a").size() + std::string("b").size();
}
int main() {
    return f();
}
```

...or if you use **`constexpr` strings** with `std::string_view`, some compilers might optimize it fully.

---

### 🔁 Summary

| Scenario                                                    | Compiler Output                |
| ----------------------------------------------------------- | ------------------------------ |
| `std::string s("a"); return s.size();`                      | `mov eax, 1` (fully optimized) |
| `return std::string("a").size() + std::string("b").size();` | Full string construction logic |

🧠 **Lesson**: Optimizing compilers are amazing — but also cautious. If there's any chance of observable side effects, they err on the safe side unless optimization flags are pushed further or code is simplified.


-------
-------
-------

![](./images/ppp5.png)
![](./images/ppp6.png)
![](./images/ppp7.png)
![](./images/ppp8.png)
![](./images/ppp9.png)
![](./images/ppp10.png)
![](./images/ppp11.png)
![](./images/ppp12.png)
![](./images/ppp13.png)
![](./images/ppp14.png)

-------
-------
-------

![](./images/ppp15.png)
![](./images/ppp16.png)
![](./images/ppp17.png)
![](./images/ppp18.png)


-------
-------
-------

![](./images/ppp19.png)
![](./images/ppp20.png)
![](./images/ppp21.png)
![](./images/ppp22.png)
![](./images/ppp23.png)
![](./images/ppp24.png)

-------
-------
-------

![](./images/ppp25.png)
This slide is from a C++ programming presentation and is addressing **efficient and correct use of virtual functions and destructors** in class hierarchies. Let’s break it down carefully.

---

### **Code Explanation**

```cpp
struct Base {
    virtual ~Base() = default;
    virtual void do_a_thing() = 0;
};

struct Derived : Base {
    virtual ~Derived() = default;
    void do_a_thing() override {}
};
```

#### 1. `virtual ~Base() = default;`

* This is good practice.
* It **ensures proper cleanup** when a derived object is deleted through a `Base*` pointer.
* Marking it `= default` tells the compiler to generate the destructor automatically.

#### 2. `virtual void do_a_thing() = 0;`

* This is a **pure virtual function**.
* It makes `Base` an **abstract class**, so you cannot instantiate `Base` directly.
* Forces derived classes to implement `do_a_thing()`.

#### 3. `virtual ~Derived() = default;`

* This line is **what the slide critiques**.
* While not *incorrect*, it’s **redundant**.

---

## 🧠 Key Concepts Explained

### ✅ Why `virtual ~Base()` is **necessary**

* If you ever write `Base* b = new Derived(); delete b;`, then `Derived`'s destructor must be called.
* If `~Base()` is **not virtual**, only `Base`'s destructor is invoked — leading to **resource leaks or undefined behavior**.

---

### ❌ Why `virtual ~Derived()` is **unnecessary**

* Since `Base` already has a `virtual` destructor, **all derived destructors are automatically virtual**, whether or not you write `virtual` again.
* Writing `virtual` on `~Derived()` is **redundant** and adds noise without benefit.

---

### 🧱 "Move construction / assignment is disabled"

This refers to:

```cpp
virtual ~Base() = default;
```

* **Virtual destructors inhibit move semantics** unless **explicitly defaulted**.
* When a class has a virtual destructor, the compiler does **not implicitly generate** move constructors and move assignment operators.
* This is because adding a virtual table (`vtable`) pointer changes the semantics of moving objects (especially slicing concerns).

So:

```cpp
struct Base {
    virtual ~Base() = default;
    Base(Base&&) = default;            // Add this if you want move constructor
    Base& operator=(Base&&) = default; // Add this if you want move assignment
};
```

---

### 📋 Summary of What’s Wrong

1. **Unnecessary Work**: `virtual ~Derived()` repeats `virtual`, which is implied.
2. **Subtle Limitation**: Move constructor and assignment operator aren't generated unless manually defaulted in presence of virtual destructor.

---

### ✅ Best Practice Version

```cpp
struct Base {
    virtual ~Base() = default;
    virtual void do_a_thing() = 0;
    Base(Base&&) = default;
    Base& operator=(Base&&) = default;
};

struct Derived : Base {
    ~Derived() = default; // no need to repeat `virtual`
    void do_a_thing() override {}
};
```

![](./images/ppp26.png)

-------
-------
-------

![](./images/ppp27.png)
![](./images/ppp28.png)
![](./images/ppp29.png)

-------
-------
-------

![](./images/ppp30.png)
![](./images/ppp31.png)

This image is giving advice in the context of C++ smart pointers and object slicing: **"Don't Do More Work Than You Have To"**. It illustrates an inefficiency that can occur when passing smart pointers by value rather than by reference.

---

### 🔍 Let's break down the code:

```cpp
int use_a_base(std::shared_ptr<Base> p)
{
    return p->value();
}
```

* This function takes a `shared_ptr<Base>` **by value**.
* This **increments the reference count** of the shared pointer, which involves atomic operations and is **relatively expensive**.
* If the function just wants to **read** or **use** the object without taking ownership or modifying the pointer, this is unnecessary overhead.

---

```cpp
int main()
{
    auto ptr = std::make_shared<Derived>();
    use_a_base(ptr);
}
```

* `ptr` is a `shared_ptr<Derived>`, which gets **implicitly converted** to a `shared_ptr<Base>` when passed to `use_a_base`.
* This conversion involves **creating a new shared pointer**, which internally points to the same control block but still **bumps the ref count**.

---

### 💡 The Better Way

To follow the advice "Don't Do More Work Than You Have To", you should **pass by `const&`** instead:

```cpp
int use_a_base(const std::shared_ptr<Base>& p)
{
    return p->value();
}
```

This way:

* You **avoid copying** the smart pointer.
* You **avoid incrementing/decrementing the reference count** unnecessarily.
* You **still ensure safety** because the function is not modifying the pointer.

---

### ✅ Summary

| Original Code             | Problem                                         |
| ------------------------- | ----------------------------------------------- |
| `std::shared_ptr<Base> p` | Copying increases ref count unnecessarily       |
| `use_a_base(ptr)`         | Causes extra work due to copy & type conversion |

**Solution:** Use `const std::shared_ptr<Base>& p` in the function parameter.

![](./images/ppp32.png)
![](./images/ppp33.png)