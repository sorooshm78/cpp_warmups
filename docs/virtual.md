## 🚧 Why Use Virtual Destructors?

In C++, if you delete a derived class object through a base class pointer, and the base class doesn't have a **virtual destructor**, the **derived class’s destructor won’t be called**. This leads to **resource leaks** or **incomplete destruction**.

---

### ✅ Example with **Virtual Destructor** (Correct Behavior)

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() {
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base {
public:
    ~Derived() {
        std::cout << "Derived destructor\n";
    }
};

int main() {
    Base* b = new Derived();
    delete b;  // Correct: Both Derived and Base destructors are called
}
```

#### 🔧 Output:

```
Derived destructor
Base destructor
```

* First `Derived` is destroyed, then `Base`.
* This is correct and safe.

---

### ❌ Example **Without Virtual Destructor** (Incorrect Behavior)

```cpp
#include <iostream>

class Base {
public:
    ~Base() {
        std::cout << "Base destructor\n";
    }
};

class Derived : public Base {
public:
    ~Derived() {
        std::cout << "Derived destructor\n";
    }
};

int main() {
    Base* b = new Derived();
    delete b;  // WRONG: Only Base destructor is called!
}
```

#### 🔧 Output:

```
Base destructor
```

* `Derived`’s destructor is **not called**.
* If `Derived` had dynamically allocated memory or resources, they would be **leaked**.
* This causes **undefined behavior**.

---

## 💡 Key Takeaway

| Scenario                                     | Should Base Destructor be `virtual`? |
| -------------------------------------------- | ------------------------------------ |
| You delete derived objects via base pointers | ✅ YES                                |
| No polymorphism or dynamic deletion          | ❌ NO (optional)                      |

---

## 🔄 Rule of Thumb

* Always make the destructor `virtual` if your class has **virtual functions** or is intended to be **used polymorphically** (i.e., with pointers or references to base class).
* If the base class will never be used this way, a non-virtual destructor is fine and may slightly improve performance.

---

## 🧠 Bonus Tip

Making a base class destructor `virtual` **also disables automatic move operations** (as mentioned earlier). So you might need to manually define them if needed:

```cpp
class Base {
public:
    virtual ~Base() = default;

    Base(Base&&) = default;
    Base& operator=(Base&&) = default;
};
```

---
