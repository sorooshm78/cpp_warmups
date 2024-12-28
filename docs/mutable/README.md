In C++, the concepts of **mutable** and **non-mutable** primarily relate to whether an object's state can be modified after its creation. Understanding these concepts is crucial for effective memory management, const-correctness, and ensuring data integrity within your programs.

### **1. Mutable Members in C++**

The `mutable` keyword in C++ is used to allow a member of a class to be modified even if it is part of an object that is otherwise immutable (i.e., declared as `const`). This is particularly useful in scenarios where you need to perform operations like caching, reference counting, or lazy evaluation without changing the logical state of the object.

**Key Points:**
- **Usage:** Apply `mutable` to class member variables.
- **Effect:** Allows modification of the mutable member within `const` member functions.
- **Purpose:** Enables specific members to be changeable without altering the object's external state.

**Example:**

```cpp
#include <iostream>

class Example {
public:
    Example(int val) : value(val), cache(0) {}

    // A const member function
    int getValue() const {
        // Even though the function is const, we can modify 'cache' because it's mutable
        cache = value * 2;
        return value;
    }

    // Another const member function
    int getCache() const {
        return cache;
    }

private:
    int value;
    mutable int cache; // 'cache' can be modified in const functions
};

int main() {
    const Example ex(10);
    
    std::cout << "Value: " << ex.getValue() << std::endl; // Modifies 'cache'
    std::cout << "Cache: " << ex.getCache() << std::endl; // Accesses 'cache'

    return 0;
}
```

**Output:**
```
Value: 10
Cache: 20
```

**Explanation:**
- The `Example` class has two member variables: `value` (non-mutable) and `cache` (mutable).
- The `getValue` function is `const`, meaning it cannot modify any member variables except those declared as `mutable`.
- Despite `ex` being a `const` object, calling `getValue` modifies the `cache` member.

### **2. Non-Mutable (Immutable) Members in C++**

By default, member variables in a class are **non-mutable**, meaning they cannot be modified within `const` member functions. Additionally, objects can be declared as `const`, making all their member variables immutable within their scope.

**Key Points:**
- **Non-mutable Members:** Regular class members that cannot be modified in `const` contexts.
- **Const Objects:** When an object is declared as `const`, none of its non-mutable members can be altered.
- **Purpose:** Ensures data integrity by preventing unintended modifications.

**Example:**

```cpp
#include <iostream>

class ImmutableExample {
public:
    ImmutableExample(int val) : value(val) {}

    // A const member function
    int getValue() const {
        // Attempting to modify 'value' here would cause a compilation error
        // value = value * 2; // Error!
        return value;
    }

private:
    int value; // Non-mutable member
};

int main() {
    const ImmutableExample ex(10);
    
    std::cout << "Value: " << ex.getValue() << std::endl;

    // Attempting to modify 'value' directly would also cause an error
    // ex.value = 20; // Error!

    return 0;
}
```

**Output:**
```
Value: 10
```

**Explanation:**
- The `ImmutableExample` class has a single member variable `value`, which is non-mutable.
- The `getValue` function is `const` and cannot modify `value`.
- The object `ex` is declared as `const`, preventing any modification to its members outside of mutable exceptions.

### **3. Comparing Mutable and Non-Mutable Members**

| Feature               | Mutable Members                                    | Non-Mutable Members                             |
|-----------------------|----------------------------------------------------|-------------------------------------------------|
| **Declaration**       | `mutable` keyword before the member variable        | No `mutable` keyword                            |
| **Modification**     | Can be modified in `const` member functions         | Cannot be modified in `const` member functions   |
| **Use Cases**         | Caching, lazy evaluation, reference counting        | Representing the core state of the object        |
| **Access**            | Accessible and modifiable as usual in non-const contexts | Accessible and modifiable as usual in non-const contexts |

**Example Combining Both:**

```cpp
#include <iostream>

class MixedExample {
public:
    MixedExample(int val) : value(val), cache(0) {}

    int getValue() const {
        cache = value * 2; // Allowed because 'cache' is mutable
        return value;
    }

    int getCache() const {
        return cache;
    }

    void setValue(int val) {
        value = val; // Allowed in non-const member function
    }

private:
    int value;           // Non-mutable
    mutable int cache;   // Mutable
};

int main() {
    MixedExample ex(10);
    const MixedExample& constEx = ex;

    std::cout << "Initial Value: " << constEx.getValue() << std::endl; // Modifies 'cache'
    std::cout << "Cache after getValue: " << constEx.getCache() << std::endl;

    // ex.setValue(20); // Allowed: non-const object

    // constEx.setValue(20); // Error: const reference cannot call non-const functions

    return 0;
}
```

**Output:**
```
Initial Value: 10
Cache after getValue: 20
```

### **4. When to Use `mutable`**

Use the `mutable` keyword when you have members that:

- Do not affect the logical state of the object.
- Are used for caching or storing temporary results.
- Manage resources like reference counts or logging mechanisms.

**Caution:** Overusing `mutable` can lead to code that is harder to understand and maintain. It breaks the `const`-correctness guarantees, so it should be used sparingly and only when necessary.

### **Summary**

- **Mutable Members:** Declared with the `mutable` keyword, allowing them to be modified even in `const` objects or `const` member functions.
- **Non-Mutable Members:** Regular class members that cannot be modified in `const` contexts, ensuring the object's state remains unchanged.
- **Const Objects:** Objects declared with the `const` keyword, making all their non-mutable members immutable within their scope.

Understanding the distinction between mutable and non-mutable members helps in designing classes that are both efficient and maintainable, ensuring that immutability contracts are respected where necessary while allowing flexibility for specific use cases.