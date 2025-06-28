---

## 🔹 What does `template<int T>` mean in C++?

This is **template parameterization**, but instead of templating on a *type*, you are templating on a **non-type parameter**, specifically an `int`.

So:

✅ `template<typename T>` — you make T a *type* parameter.
✅ `template<int T>` — you make T an *integer constant* parameter.

---

## 🔹 How does it work?

When you write:

```cpp
template<int T>
struct MyStruct {
    void print() {
        std::cout << "T = " << T << std::endl;
    }
};
```

you are defining a **template struct** where `T` is an integer known at compile time.

You can instantiate it like this:

```cpp
MyStruct<5> obj; // T is 5
obj.print();     // prints: T = 5
```

In this example, `T` is baked into the type itself.
Every different integer value gives you a **different type**:

```cpp
MyStruct<1> a;
MyStruct<2> b;

// a and b are different types!
```

---

## 🔹 Why do we use this?

This technique is very useful for:

✅ Compile-time constants
✅ Array sizes
✅ Specialization for specific constants
✅ Template metaprogramming (e.g., recursive templates)

---

## 🔹 Example: Compile-time array

Here’s a more practical example:

```cpp
template<int Size>
class FixedArray {
    int data[Size];
public:
    void set(int index, int value) {
        data[index] = value;
    }
    int get(int index) {
        return data[index];
    }
};
```

Then:

```cpp
FixedArray<10> arr10;
FixedArray<20> arr20;

// arr10 and arr20 are different types,
// and their arrays have different sizes.
```

---

## ✅ In summary:

`template<int T>` means:

* You have a template that depends on an integer value.
* T is a **compile-time constant**.
* Each distinct T produces a distinct instantiation of the template.

