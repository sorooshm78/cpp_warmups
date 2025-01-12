In C++, smart pointers are used to manage the lifetime of dynamically allocated objects, ensuring proper resource management and preventing memory leaks. The Standard Library provides several smart pointers, including `std::shared_ptr` and `std::weak_ptr`. Understanding when and why to use `std::weak_ptr` is crucial, especially when dealing with complex object relationships that could lead to issues like **circular references**.

## Why `std::weak_ptr` Is Needed

### The Problem with `std::shared_ptr` and Circular References

`std::shared_ptr` manages shared ownership of an object through reference counting. Each `shared_ptr` increases the reference count, and when the count drops to zero, the managed object is deleted. However, in certain scenarios, using `shared_ptr` can inadvertently create **circular references**, preventing the reference count from ever reaching zero and thus causing memory leaks.

**Circular Reference Example:**

Imagine two objects that reference each other using `std::shared_ptr`:

```cpp
#include <memory>

struct B; // Forward declaration

struct A {
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};

struct B {
    std::shared_ptr<A> a_ptr;
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    a->b_ptr = b;
    b->a_ptr = a;

    // Both a and b go out of scope here, but their reference counts never reach zero
}
```

In this example:

1. `a` holds a `shared_ptr` to `b`.
2. `b` holds a `shared_ptr` back to `a`.
3. When `main` ends, `a` and `b` go out of scope, decreasing their reference counts by one.
4. However, each object still holds a `shared_ptr` to the other, keeping their reference counts at least one.
5. As a result, neither `A` nor `B` is destroyed, leading to a memory leak.

### How `std::weak_ptr` Solves the Problem

`std::weak_ptr` provides a way to reference an object managed by `std::shared_ptr` without increasing its reference count. This is useful for breaking circular dependencies because it allows one of the references to be non-owning.

**Modified Example Using `std::weak_ptr`:**

```cpp
#include <memory>
#include <iostream>

struct B; // Forward declaration

struct A {
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};

struct B {
    std::weak_ptr<A> a_ptr; // Changed to weak_ptr
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    a->b_ptr = b;
    b->a_ptr = a; // Assigning shared_ptr to weak_ptr

    // Both a and b go out of scope here
}
```

In this modified example:

1. `A` still holds a `shared_ptr` to `B`.
2. `B` now holds a `weak_ptr` to `A`.
3. When `main` ends, `a` and `b` go out of scope:
   - The `shared_ptr` to `A` (`a`) is destroyed, decreasing `A`'s reference count.
   - Since `B` only holds a `weak_ptr` to `A`, it doesn't affect `A`'s reference count.
   - Similarly, the `shared_ptr` to `B` (`b`) is destroyed, decreasing `B`'s reference count.
4. Both `A` and `B` are properly destroyed, as their reference counts reach zero.

**Output:**
```
A destroyed
B destroyed
```

## When to Use `std::weak_ptr`

1. **Breaking Circular References:**
   As shown in the example, when two objects reference each other, use `weak_ptr` for at least one of the references to prevent a circular dependency.

2. **Observer Patterns:**
   When implementing observer patterns, observers can hold `weak_ptr` references to the subject to avoid prolonging the subject's lifetime unintentionally.

3. **Caching Mechanisms:**
   In caching, you might want to hold references to objects without preventing their deletion when they are no longer in use elsewhere.

## How to Use `std::weak_ptr`

To utilize a `weak_ptr`, you typically need to **lock** it to obtain a `shared_ptr` when you want to access the managed object. This ensures that the object is still alive.

**Example: Accessing an Object via `std::weak_ptr`:**

```cpp
#include <memory>
#include <iostream>

struct Resource {
    void say_hello() const { std::cout << "Hello!\n"; }
};

int main() {
    std::weak_ptr<Resource> weak_res;

    {
        auto shared_res = std::make_shared<Resource>();
        weak_res = shared_res; // weak_ptr now references the Resource

        if (auto locked_res = weak_res.lock()) { // Attempt to lock
            locked_res->say_hello(); // Safe to use
        } else {
            std::cout << "Resource no longer exists.\n";
        }
    }

    // At this point, shared_res is out of scope and Resource is destroyed

    if (auto locked_res = weak_res.lock()) {
        locked_res->say_hello();
    } else {
        std::cout << "Resource no longer exists.\n"; // This will execute
    }
}
```

**Output:**
```
Hello!
Resource no longer exists.
```

**Explanation:**

1. A `shared_ptr<Resource>` named `shared_res` is created and managed.
2. A `weak_ptr<Resource>` named `weak_res` is assigned to reference the same `Resource`.
3. Inside the first block:
   - `weak_res.lock()` successfully obtains a `shared_ptr` (`locked_res`) to the `Resource`.
   - `say_hello()` is called on the `Resource`.
4. After the block:
   - `shared_res` goes out of scope, and the `Resource` is destroyed.
5. Outside the block:
   - Attempting to `lock()` `weak_res` fails because the `Resource` no longer exists.
   - The program outputs that the resource no longer exists.

## Key Points to Remember

- **`std::shared_ptr`:** Maintains shared ownership with reference counting. Use when multiple owners need to share access to an object.
  
- **`std::weak_ptr`:** Provides a non-owning reference to an object managed by `shared_ptr`. It does not affect the reference count and is useful for:
  - Breaking circular dependencies.
  - Implementing observer patterns.
  - Accessing objects conditionally without extending their lifetime.

- **Locking `weak_ptr`:** Always check if the managed object still exists by locking the `weak_ptr` before accessing it. This ensures safe access and avoids dereferencing dangling pointers.

By understanding and appropriately using `std::weak_ptr`, you can design more robust and memory-efficient C++ applications, avoiding common pitfalls associated with dynamic memory management.

---

Or

---
In C++, `std::shared_ptr` and `std::weak_ptr` are smart pointers provided by the Standard Library to manage dynamic memory more safely and efficiently. Understanding when and why to use `std::weak_ptr` involves grasping how `std::shared_ptr` works, particularly with reference counting and preventing memory leaks caused by circular references.

### Understanding `std::shared_ptr`

`std::shared_ptr` is a smart pointer that retains shared ownership of an object through a pointer. Multiple `std::shared_ptr` instances can own the same object, and the object is destroyed when the last `std::shared_ptr` owning it is destroyed or reset.

**Reference Counting:**
Each `std::shared_ptr` maintains a reference count. When a new `std::shared_ptr` is created from an existing one, the count increases. When a `std::shared_ptr` is destroyed or reset, the count decreases. When the count reaches zero, the managed object is deleted.

### The Problem: Circular References

While `std::shared_ptr` handles memory management effectively, it can lead to issues like **circular references**, where two or more objects reference each other using `std::shared_ptr`. This mutual ownership prevents the reference counts from ever reaching zero, causing memory leaks because the destructors are never called.

**Example of Circular Reference Without `std::weak_ptr`:**

Consider two classes, `Parent` and `Child`, where each holds a `std::shared_ptr` to the other.

```cpp
#include <iostream>
#include <memory>

class Child; // Forward declaration

class Parent {
public:
    std::shared_ptr<Child> child;
    Parent() { std::cout << "Parent created\n"; }
    ~Parent() { std::cout << "Parent destroyed\n"; }
};

class Child {
public:
    std::shared_ptr<Parent> parent;
    Child() { std::cout << "Child created\n"; }
    ~Child() { std::cout << "Child destroyed\n"; }
};

int main() {
    {
        std::shared_ptr<Parent> parent = std::make_shared<Parent>();
        std::shared_ptr<Child> child = std::make_shared<Child>();
        parent->child = child;
        child->parent = parent;
    }
    std::cout << "End of main\n";
    return 0;
}
```

**Output:**
```
Parent created
Child created
End of main
```

**Explanation:**
- Both `Parent` and `Child` are created successfully.
- When the scope ends, the `parent` and `child` `std::shared_ptr`s go out of scope, decrementing their respective reference counts.
- However, `parent->child` and `child->parent` still hold references to each other, keeping the reference counts above zero.
- As a result, neither `Parent` nor `Child` is destroyed, leading to a memory leak.

### Solution: Using `std::weak_ptr` to Break the Cycle

To prevent circular references, `std::weak_ptr` can be used. Unlike `std::shared_ptr`, `std::weak_ptr` does not contribute to the reference count. It provides a non-owning ("weak") reference to an object that is managed by `std::shared_ptr`.

**Rewriting the Example with `std::weak_ptr`:**

Typically, one of the references in the cycle should be a `std::weak_ptr`. In the `Parent` and `Child` example, it's common for the `Child` to have a weak reference to the `Parent`.

```cpp
#include <iostream>
#include <memory>

class Child; // Forward declaration

class Parent {
public:
    std::shared_ptr<Child> child;
    Parent() { std::cout << "Parent created\n"; }
    ~Parent() { std::cout << "Parent destroyed\n"; }
};

class Child {
public:
    std::weak_ptr<Parent> parent; // Changed to weak_ptr
    Child() { std::cout << "Child created\n"; }
    ~Child() { std::cout << "Child destroyed\n"; }
};

int main() {
    {
        std::shared_ptr<Parent> parent = std::make_shared<Parent>();
        std::shared_ptr<Child> child = std::make_shared<Child>();
        parent->child = child;
        child->parent = parent; // Assigning shared_ptr to weak_ptr
    }
    std::cout << "End of main\n";
    return 0;
}
```

**Output:**
```
Parent created
Child created
Parent destroyed
Child destroyed
End of main
```

**Explanation:**
- `Parent` and `Child` are created as before.
- `parent->child` holds a `std::shared_ptr` to `Child`, increasing the `Child`'s reference count.
- `child->parent` holds a `std::weak_ptr` to `Parent`, **not** increasing the `Parent`'s reference count.
- When the scope ends, `parent` and `child` go out of scope:
  - `parent`'s reference count decreases, and since `child->parent` is a `std::weak_ptr`, it doesn't prevent `Parent` from being destroyed.
  - `Child`'s reference count also decreases because `parent->child` is a `std::shared_ptr` that goes out of scope.
- Both `Parent` and `Child` are properly destroyed, preventing the memory leak.

### When to Use `std::weak_ptr`

1. **Breaking Circular References:** As demonstrated, use `std::weak_ptr` to prevent reference cycles that can lead to memory leaks.
2. **Observer Patterns:** When implementing observer patterns where observers need to refer to subjects without affecting their lifetimes.
3. **Caching:** To hold references to cached objects without preventing their destruction when no longer needed elsewhere.

### Accessing the Managed Object with `std::weak_ptr`

Since `std::weak_ptr` doesn't own the object, you need to convert it to `std::shared_ptr` to access the managed object safely. This is done using the `lock()` method, which returns a `std::shared_ptr` if the object is still alive or an empty `std::shared_ptr` if the object has been destroyed.

**Example:**

```cpp
std::weak_ptr<Parent> weakParent = child->parent;
if (auto sharedParent = weakParent.lock()) {
    // Safe to use sharedParent
    std::cout << "Parent is still alive.\n";
} else {
    // The Parent object has been destroyed
    std::cout << "Parent has been destroyed.\n";
}
```

### Summary

- **`std::shared_ptr`** manages shared ownership of an object with reference counting.
- **`std::weak_ptr`** provides a non-owning reference to an object managed by `std::shared_ptr`, helping to prevent circular references and memory leaks.
- Use `std::weak_ptr` when you need to reference an object without affecting its lifetime, especially in scenarios where circular dependencies might occur.

By appropriately using `std::weak_ptr`, you can write more robust and memory-efficient C++ applications.


![](https://blog.kakaocdn.net/dn/dUsYtG/btrH5dCQBxq/SOnwOtYjMOwIZNHVAXENg0/img.png)