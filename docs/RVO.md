# RVO (**Return Value Optimization**)
Great question! The optimization I’m referring to is called **Return Value Optimization (RVO)** — a key compiler technique in C++ to make returning objects efficient.

---

### What is Return Value Optimization (RVO)?

Normally, when you return an object from a function by value, you might expect:

1. The object is created inside the function.
2. The object is copied or moved when returned.
3. The returned object is assigned to the variable outside the function.

**RVO** is an optimization where the compiler **elides (omits) those intermediate copy or move operations entirely** and constructs the returned object directly in the memory where it’s going to be used.

---

### Why is RVO beneficial?

- It **removes unnecessary copies or moves** of objects.
- Improves performance by reducing memory allocations and data transfers.
- Makes returning objects by value as efficient as possible, sometimes as fast as returning a pointer.

---

### Example:

Without RVO, imagine:

```cpp
Moo foo() {
    Moo moo{"hello"};  // created inside foo()
    return moo;        // returns a copy or moves moo
}
```

- `moo` is created inside `foo`.
- Then copied or moved when returned.

With RVO enabled:

- The compiler constructs `moo` **directly in the caller’s storage**.
- No copy or move constructor is called at all.

---

### How does `std::move` affect RVO?

If you explicitly write:

```cpp
return std::move(moo);
```

- You tell the compiler “I want to move,” which disables some forms of RVO.
- The compiler must perform a move (calling move constructor), preventing elision.
- So, **you lose the chance of the even better optimization**.

---

### Summary:

- **RVO**: Compiler optimization that omits copy/move on return by directly constructing the object where it will live.
- Use **plain `return moo;`** to enable RVO.
- Avoid `return std::move(moo);` because it can prevent RVO, making your code less efficient.

---