## 🔹 Background: Lambdas in C++

A **lambda expression** is an *anonymous function object* (a functor) that can capture variables from its surrounding scope.

Basic form:

```cpp
[ captures ] ( parameters ) -> return_type {
    // body
}
```

Example:

```cpp
int x = 10;
auto f = [x]() { return x + 5; }; // captures x by value
std::cout << f(); // 15
```

Here, `x` is **captured by value**, meaning a *copy* of `x` is stored inside the lambda.

---

## 🔹 The Problem Without `mutable`

By default, when you capture variables **by value**, the lambda’s `operator()` is **const**.
That means you cannot modify the captured variables inside the lambda body.

Example:

```cpp
int x = 10;
auto f = [x]() {
    x++; // ❌ Error! because operator() is const
    return x;
};
```

Compiler error (conceptually):

```
error: increment of member ‘<lambda>::x’ in read-only object
```

---

## 🔹 Enter `mutable`

If you add the `mutable` keyword to the lambda, it makes the call operator **non-const**, allowing modification of *captured-by-value* variables **inside** the lambda.

Example:

```cpp
int x = 10;
auto f = [x]() mutable {
    x++;            // ✅ allowed
    std::cout << x; // prints 11
};
f();
std::cout << x;     // prints 10 (original x unchanged)
```

Explanation:

* `x` inside the lambda is still a **copy** (because we captured by value),
* but now we’re allowed to **modify that copy** thanks to `mutable`.

The original `x` outside the lambda is not affected.

---

## 🔹 What if we capture by reference?

If you capture by reference `[&x]`, you can modify the original variable without `mutable`.

Example:

```cpp
int x = 10;
auto f = [&x]() {
    x++; // ✅ modifies the original x
};
f();
std::cout << x; // 11
```

Here `mutable` is irrelevant because you’re modifying `x` directly by reference.

---

## 🔹 Why does this matter in practice?

You use `mutable` when:

* You capture values by value,
* You want to modify **internal copies** (for example, to maintain internal state),
* But you don’t want to modify the original captured variables.

Example — counting how many times a lambda is called:

```cpp
auto counter = [count = 0]() mutable {
    return ++count;
};

std::cout << counter() << "\n"; // 1
std::cout << counter() << "\n"; // 2
std::cout << counter() << "\n"; // 3
```

Here:

* The lambda maintains an **internal counter** (`count`) even though the capture is by value.
* Without `mutable`, incrementing `count` would be illegal.

---

## 🔹 In C++23 context

C++23 didn’t fundamentally change the meaning of `mutable` in lambdas — its semantics are the same as in earlier standards (C++11 onward).
However, C++23 adds related lambda enhancements, such as:

* Deduction guides for lambdas with templated parameters,
* More flexible capture syntax,
* But `mutable` still means the same: it allows modifying captured-by-value copies.

---

## ✅ TL;DR Summary

| Capture Type          | Without `mutable` | With `mutable`         | Affects Original? |
| --------------------- | ----------------- | ---------------------- | ----------------- |
| `[x]` (by value)      | Cannot modify `x` | Can modify copy of `x` | ❌ No              |
| `[&x]` (by reference) | Can modify `x`    | Same                   | ✅ Yes             |

---

-------------------------------------------------------------------------------------------
## 🔹 Introduction: Default behavior of lambdas

In C++, when you define a **lambda expression**, you can “capture” variables from the surrounding scope to use inside the lambda.

Example:

```cpp
int x = 10;

auto f = [x]() {
    std::cout << x << '\n';
};

f(); // prints 10
```

Here:

* `[x]` means **capture `x` by value**.
* So the lambda internally stores a **copy** of `x`.

---

## 🔹 Important default: lambda’s `operator()` is **const**

Under the hood, a lambda is transformed into a **function object (functor)**, like this:

```cpp
struct Lambda {
    int x;
    void operator()() const {
        std::cout << x;
    }
};
```

Notice that `operator()` is marked as `const`.
This means you **cannot modify** any of the captured variables inside the lambda body because a `const` function cannot modify its data members.

---

## 🔹 Result: compilation error if you modify captures

If you try to modify a captured-by-value variable, you get a compile-time error:

```cpp
int x = 10;

auto f = [x]() {
    x++;   // ❌ Error! lambda's operator() is const
    std::cout << x;
};
```

Typical compiler message:

```
error: increment of member ‘Lambda::x’ in read-only object
```

---

## 🔹 The solution: `mutable`

If you add the `mutable` keyword to your lambda, it tells the compiler:

> “Please make this lambda’s `operator()` **non-const**, so I can modify the captured variables inside it.”

Internally, it becomes:

```cpp
struct Lambda {
    int x;
    void operator()() {   // ❌ const is removed
        x++;
        std::cout << x;
    }
};
```

---

## 🔹 Basic example

```cpp
int x = 10;

auto f = [x]() mutable {
    x++;                  // ✅ allowed, because of mutable
    std::cout << x << '\n';
};

f(); // prints 11
std::cout << x << '\n';  // prints 10 (original x unchanged)
```

🟢 Explanation:

* `x` inside the lambda is still a **copy**.
* You are modifying that **internal copy**, not the original variable.

---

## 🔹 Comparison: capture by reference

If you capture by reference (`[&x]`), `mutable` is not needed because you’re modifying the **original variable** directly:

```cpp
int x = 10;

auto f = [&x]() {
    x++;                  // ✅ OK
    std::cout << x << '\n';
};

f(); // prints 11
std::cout << x << '\n'; // prints 11 (original modified)
```

| Capture Type          | Needs `mutable`? | Modifies original variable? |
| --------------------- | ---------------- | --------------------------- |
| `[x]` (by value)      | ✅ Yes            | ❌ No                        |
| `[&x]` (by reference) | ❌ No             | ✅ Yes                       |

---

## 🔹 Practical Example 1 — Independent counter

A common use case for `mutable` is when you want a lambda to maintain its own internal state:

```cpp
auto counter = [count = 0]() mutable {
    return ++count;
};

std::cout << counter() << '\n'; // 1
std::cout << counter() << '\n'; // 2
std::cout << counter() << '\n'; // 3
```

Without `mutable`, this would fail to compile because `count++` modifies a captured variable.

---

## 🔹 Practical Example 2 — Inside a loop

You can use `mutable` when you create multiple lambdas that each need their own copy of a variable:

```cpp
std::vector<std::function<void()>> funcs;

for (int i = 0; i < 3; ++i) {
    funcs.push_back([i]() mutable {
        i *= 2;
        std::cout << i << '\n';
    });
}

for (auto& f : funcs) f();
```

Output:

```
0
2
4
```

Each lambda keeps **its own copy** of `i`, and `mutable` lets you modify it.

---

## 🔹 Practical Example 3 — In algorithms

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
int offset = 10;

std::for_each(v.begin(), v.end(), [offset](int& n) mutable {
    n += offset++;
});
```

Here, `offset` is captured by value and changed internally for each iteration,
but the original `offset` variable outside remains untouched.

---

## 🔹 Important note: `mutable` vs `const`

A lambda marked as `mutable` has a **non-const** `operator()`.
You **cannot** use both `const` and `mutable` together — they are opposites.

Example:

```cpp
[x]() mutable -> void { /* ... */ } // ✅ OK
[x]() const -> void { /* ... */ }   // ✅ OK
[x]() mutable const -> void { /* ❌ Invalid */ }
```

---

## 🔹 C++23 and `mutable`

In C++23, the meaning of `mutable` hasn’t changed — it still works exactly the same way.
You can also use it in **templated lambdas** or lambdas with **explicit object parameters**:

```cpp
auto make_multiplier = [](auto factor) {
    return [factor]() mutable {
        return factor *= 2;
    };
};

auto f = make_multiplier(3);
std::cout << f() << '\n'; // 6
std::cout << f() << '\n'; // 12
```

---

## ✅ Final Summary

| Concept                | Description                                                                         |
| ---------------------- | ----------------------------------------------------------------------------------- |
| **Definition**         | `mutable` allows modification of captured **by-value** variables inside the lambda. |
| **Reason**             | By default, a lambda’s `operator()` is `const`.                                     |
| **Effect**             | Only modifies the *copy* inside the lambda, not the original.                       |
| **Needed for `[&x]`?** | ❌ No                                                                                |
| **Introduced in**      | **C++11** (unchanged through C++23)                                                 |

---
