### IIFE in C++: Immediately Invoked Function Expression

In **JavaScript**, an IIFE (Immediately Invoked Function Expression) is a function that runs **immediately after it's defined**. In **C++**, there is no exact IIFE syntax like in JavaScript, but you can achieve a **similar effect** using **lambda expressions** or unnamed functions.

---

### ✅ C++ Equivalent: IIFE with Lambda

In C++, you can define and immediately call a **lambda** like this:

```cpp
#include <iostream>

int main() {
    int result = []() {
        std::cout << "This is an IIFE in C++!" << std::endl;
        return 42;
    }();  // <-- Immediately invoked

    std::cout << "Result: " << result << std::endl;
    return 0;
}
```

### 🔍 Breakdown:

* `[]()` defines a lambda (an unnamed function).
* `{ ... }` is the function body.
* `()` at the end **invokes it immediately**.

---

### 📌 Why use IIFE in C++?

* To **initialize a variable** with complex logic:

```cpp
int x = []() {
    int a = 10;
    int b = 20;
    return a + b;
}();
```

* To **limit scope** of temporary variables.
* For **clean and isolated initialization** logic.
* In **global or static context**, where lazy or complex init is needed.

---

### ⚠️ Limitations

* Only possible with **C++11 or newer** (lambdas introduced in C++11).
* Mostly used for **initialization**, not general logic flow like in JavaScript.

---

### Summary

| Language   | IIFE Example             | Purpose                             |
| ---------- | ------------------------ | ----------------------------------- |
| JavaScript | `(function(){ ... })();` | Self-invoking function              |
| C++        | `[](){ ... }();`         | Self-invoking lambda for init/scope |

