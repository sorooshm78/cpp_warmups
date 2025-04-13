Below is a **detailed explanation** of **C++ lambda expressions**. We will start with the **basic syntax**, then delve into **captures**, **parameters**, **return types**, and **common use cases**, concluding with some **step-by-step examples**.

---

## 1. What is a Lambda Expression?

A **lambda expression** in C++ is essentially an **anonymous function**—a function with no name that you can define in place and treat like a regular object. This capability was introduced in C++11 to make code that deals with callbacks, functional programming, or short “one-off” functions more concise and convenient.

### Basic Form:

```cpp
[capture-list] (parameters) -> return_type {
    // function body
}
```

- **capture-list**: Specifies what outside variables (from the enclosing scope) the lambda can use.
- **(parameters)**: Parameter list, similar to a function’s parameter list.
- **-> return_type**: (Optional) The explicit return type. If omitted, it is deduced from the return statements inside the lambda.
- **{ body }**: The code that makes up the function’s body.

---

## 2. Capture List

In a lambda expression, the **capture list** indicates which external variables (from the enclosing scope) the lambda is allowed to access, and how it can access them. This is sometimes referred to as a “closure.”

Common capture forms:

1. **`[]`**: Capture nothing. The lambda has no access to local variables except those explicitly passed as parameters.
2. **`[=]`**: Capture **all** automatic (local) variables **by value**.
3. **`[&]`**: Capture **all** automatic (local) variables **by reference**.
4. **`[x, &y]`**: Capture `x` by value, capture `y` by reference, do not capture anything else.
5. **`[this]`**: Capture the `this` pointer by value. This is how you call member functions or access member variables within a lambda that is defined inside a class method.
6. **`[*this]`** (C++20): Capture the “current object” by value (which can copy all fields). This is a more advanced feature.

When you see something like:

```cpp
[this, &dp]() {
    // ...
}
```

It means:
- Capture `this` pointer by value (i.e., the pointer itself).
- Capture the local variable `dp` **by reference**.

In other words, inside the lambda body, you can write `this->someMethod()` or `dp.someField = ...`.

---

## 3. Parameters

Lambdas can have **parameters** just like normal functions. For instance:

```cpp
auto greet = [](const std::string &name) {
    std::cout << "Hello, " << name << "!\n";
};
```

- `[](const std::string &name)` is the **capture list** (`[]`) and the parameter list (`(const std::string &name)`).
- You could then call `greet("Alice");`

---

## 4. Return Type

In many cases, C++ can **deduce** the return type from the body of the lambda. For example:

```cpp
// Return type is automatically deduced as int
auto add = [](int a, int b) {
    return a + b;
};
```

If the body is more complicated, or if you want an explicit return type, you can specify it:

```cpp
// Force a return type of long long
auto addLong = [](int a, int b) -> long long {
    return static_cast<long long>(a) + b;
};
```

---

## 5. Mutable Lambdas

By default, a lambda that captures variables **by value** treats them as **const** inside the body. If you want to modify those captured values (in the lambda’s local copy), you can use the **mutable** keyword:

```cpp
int x = 10;
auto modifyX = [=]() mutable {
    x += 5;  // modifies the copy of x
    std::cout << "Inside lambda: x = " << x << "\n";
};

// The original x outside is unaffected.
modifyX();
std::cout << "Outside: x = " << x << "\n";
```

**Without `mutable`,** this would not compile because the captured-by-value variable `x` would be treated as if it were `const` inside the lambda.

---

## 6. Common Use Cases

1. **Callbacks / Function Pointers**: Lambdas are often used where you need a quick function for an algorithm or event callback. 
   
   ```cpp
   std::vector<int> v{3, 1, 4, 1, 5};
   std::sort(v.begin(), v.end(), [](int a, int b) {
       return a > b;  // sort descending
   });
   ```

2. **Thread Functions**: In multi-threaded code, we often pass a lambda to a `std::thread` constructor:

   ```cpp
   std::thread t([]() {
       std::cout << "Hello from a thread!\n";
   });
   t.join();
   ```

3. **As a short “helper” function** inside a larger function or method, especially if it won’t be used anywhere else.

---

## 7. Examples in Detail

### Example 1: Simple Capture by Value

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 5, 3, 2, 4};
    int threshold = 3;

    // Capture threshold by value
    // We'll remove all values <= threshold
    numbers.erase(std::remove_if(numbers.begin(), numbers.end(),
        [threshold](int x) { 
            return x <= threshold; 
        }), 
    numbers.end());

    // Print what's left
    for (int n : numbers) {
        std::cout << n << " ";  // 5 4
    }
    return 0;
}
```

#### Explanation
- The lambda `[threshold](int x) { return x <= threshold; }` captures `threshold` by value and checks if `x` is <= `threshold`. 
- `std::remove_if` uses this predicate to find elements to remove. 
- We then erase those elements from the vector.

### Example 2: Capture by Reference

```cpp
#include <iostream>

int main() {
    int counter = 0;

    // We want to modify 'counter' inside the lambda
    auto increment = [&counter]() {
        counter++;
        std::cout << "Counter is now: " << counter << "\n";
    };

    increment(); // Counter is now: 1
    increment(); // Counter is now: 2
    std::cout << "Final counter: " << counter << "\n"; // 2
    return 0;
}
```

#### Explanation
- The lambda `[&counter]()` means we capture the variable `counter` from the outside scope **by reference** so we can modify it inside the lambda.
- Each call to `increment()` increments the original `counter`.

### Example 3: Using Lambdas with `std::thread`

```cpp
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Lambda that prints a message after sleeping 1 second
    std::thread t([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Hello from thread!\n";
    });

    std::cout << "Hello from main!\n";
    t.join();
    return 0;
}
```

#### Explanation
- We create a `std::thread` directly by passing a lambda that prints a message.
- The lambda has an empty capture list `[]`. It doesn’t need any outside variables.
- We call `t.join()` to wait for that thread to finish.

### Example 4: Capture `this` in a Class Method

Imagine you have a class with a method that wants to create a thread that uses its member function:

```cpp
#include <iostream>
#include <thread>

class Worker {
public:
    void doWork() {
        std::thread t([this]() {
            // Inside the lambda, we can call this->helper()
            helper();
        });

        t.join();
    }

private:
    void helper() {
        std::cout << "Helper method called from lambda.\n";
    }
};

int main() {
    Worker w;
    w.doWork();  // "Helper method called from lambda."
    return 0;
}
```

#### Explanation
- `[this]` captures the `this` pointer, allowing the lambda to call `helper()`.
- If you did not capture `this`, you couldn’t access member methods or variables from inside the lambda.

---

## 8. Summary

- A **lambda expression** is an **anonymous function** you can define inline.
- The **capture list** indicates which outside variables (and the `this` pointer) you allow inside the lambda.
- You can capture by **value** or **reference**, or mix and match.
- Lambdas have **parameters** and a **body** just like normal functions; the return type can be automatic or explicitly specified.
- They are widely used for callbacks, functional-style algorithms, and **wherever a quick, inline function is helpful**.

**Lambdas** are extremely powerful and bring functional-programming-style convenience to C++.

-------------------------------------------------------------

Below is a **detailed look** at **C++ lambda capture lists**, explaining what each capture style does and showing **practical examples**. Understanding how variables from the enclosing scope are made available inside a lambda is crucial for writing clear, correct code.

---

## 1. What is the Capture List?

When you write a lambda expression:

```cpp
[capture-list] (parameters) -> return_type {
    // body
}
```

the **capture-list** is the mechanism that determines **which** variables from the surrounding (enclosing) scope are accessible inside the lambda and **how** they are accessed (by value or by reference).

---

## 2. Common Capture Styles

### 2.1. Capture Nothing: `[]`

When you write an **empty capture list**:

```cpp
[]() {
    // cannot access local variables from outside this lambda
}
```

- **No** local variables are captured from the enclosing scope.  
- You can only use global variables, static variables, or anything you explicitly pass in via the lambda’s parameter list.

**Example**:
```cpp
int main() {
    int x = 42;
    auto noCapture = []() {
        // Can't use 'x' here, because it's not captured
        std::cout << "Hello from noCapture!\n";
    };

    noCapture();  // works, but can't access 'x'
    return 0;
}
```

---

### 2.2. Capture All Used Variables by Value: `[=]`

When you write `[=]`, you tell the compiler, "If I refer to any automatic variable (from the enclosing scope) inside this lambda, capture it **by value**."

**By value** means a **copy** of that variable’s value is made at the time the lambda is created (or sometimes at the time it is invoked, depending on the compiler). Inside the lambda, you’re effectively using that copy.

**Example**:
```cpp
#include <iostream>

int main() {
    int a = 10, b = 20;
    auto sumByValue = [=]() {
        // 'a' and 'b' are captured by value (copied)
        return a + b;
    };

    a = 100; // changing 'a' after the lambda is created
    std::cout << "sumByValue: " << sumByValue() << "\n";
    // Output is still 30, because 'a' was captured as 10 originally.

    return 0;
}
```

**Key Points**:
- Changing `a` later **does not** affect the copy inside `sumByValue`.
- `[=]` does not automatically capture the `this` pointer in a member function. For that, you need `[this]` or `[=, this]` explicitly.

---

### 2.3. Capture All Used Variables by Reference: `[&]`

When you write `[&]`, you tell the compiler, "If I refer to any automatic variable inside this lambda, capture it by **reference**."

By reference means you will be accessing the **original** variable from the enclosing scope. If you modify it inside the lambda, you’re modifying the original.

**Example**:
```cpp
#include <iostream>

int main() {
    int counter = 0;

    auto increment = [&]() {
        // 'counter' is captured by reference
        counter++;
    };

    increment();
    increment();
    std::cout << "counter = " << counter << "\n"; // Outputs 2
    return 0;
}
```

**Key Points**:
- Any variable used inside the lambda is captured by reference, so changes affect the original.
- Must ensure the variable is still alive when the lambda is called (e.g., capturing local variables by reference, then returning the lambda from the function, can be dangerous if the original variables go out of scope).

---

### 2.4. Capture List with Specific Variables

You can **mix** capturing all variables by some default method, **except** for certain variables that you explicitly want to capture differently. For example:

- `[=, &x]`: Capture all used variables by **value**, but capture `x` by **reference**.
- `[&, x]`: Capture all used variables by **reference**, but capture `x` by **value**.

```cpp
int main() {
    int a = 10, b = 20;

    // All used variables will be by value, except 'b' which is by reference
    auto mixLambda = [=, &b]() {
        // 'a' is by value, 'b' is by reference
        b += 5;
        return a + b; // a was captured as 10, b was captured by reference
    };

    a = 100; // won't affect the copy in mixLambda
    b = 50;  // this will affect the reference in mixLambda

    std::cout << "mixLambda: " << mixLambda() << "\n";
    // Inside the lambda, 'b' was 50 at call time, plus 5 => 55. 
    // 'a' is still 10.
    // So the result is 10 + 55 = 65.

    // Also note that after mixLambda, b was incremented to 55
    std::cout << "b after lambda: " << b << "\n"; // 55

    return 0;
}
```

---

### 2.5. Capture by Value for Specific Variables

You can explicitly name which variables to capture **by value**, ignoring everything else from the enclosing scope. For example:

```cpp
int main() {
    int x = 1;
    int y = 2;
    auto lambdaVal = [x]() {
        // x is captured by value, can't access 'y'
        return x + 10;
    };

    std::cout << lambdaVal() << "\n"; // 11
    return 0;
}
```

- `[x]` means "capture only `x` by value." 
- If you try to use `y` inside `lambdaVal()`, it **won’t compile** because `y` isn’t captured.

---

### 2.6. Capture `this`: `[this]`

When you write `[this]` **inside a non-static class method**, you are telling the lambda, "I want to capture the `this` pointer so I can access **member variables** and **member methods**."

**Example**:
```cpp
#include <iostream>
#include <thread>

class Worker {
public:
    void doWork() {
        // Capture 'this' so we can call member functions in the lambda
        std::thread t([this]() {
            helper();
        });
        t.join();
    }

private:
    void helper() {
        std::cout << "Helper method called.\n";
    }
};

int main() {
    Worker w;
    w.doWork(); 
    // Output: "Helper method called."
    return 0;
}
```

- `[this]` effectively means `this` pointer is available inside the lambda body.
- Without `[this]`, you can’t access `helper()`.

---

### 2.7. C++20: `[=, *this]` or `[*this]`

In **C++20**, you can capture the **entire current object** by value using `[*this]`. This creates a **copy of the object** rather than just capturing the `this` pointer. That allows you to use all of the object’s data members by value even if the original object goes out of scope.

**Example** (simplified conceptual code):
```cpp
#include <iostream>
#include <string>

class Person {
public:
    Person(std::string n, int a) : name(n), age(a) {}
    auto createLambda() {
        // This captures the entire Person object by value
        return [*this]() {
            // We use the copy's name and age here
            std::cout << "Name: " << name << ", Age: " << age << "\n";
        };
    }
private:
    std::string name;
    int age;
};

int main() {
    auto p = Person("Alice", 30);
    auto f = p.createLambda();

    p = Person("Bob", 40); // reassign p
    // f still prints "Alice, 30" because it captured a copy

    f();
    return 0;
}
```

- `[ *this ]` means the lambda has its own copy of the entire `Person` object. Even if `p` changes afterwards, the lambda’s copy remains the same.

---

## 3. Summary of Capture List Options

1. **`[]`**: Capture nothing from the enclosing scope.
2. **`[=]`**: Capture all **used** variables by value.
3. **`[&]`**: Capture all **used** variables by reference.
4. **`[a, &b]`**: Capture `a` by value, `b` by reference, ignore everything else.
5. **`[=, &b]`**: Capture all used variables by value, but `b` by reference.
6. **`[&, x]`**: Capture all used variables by reference, but `x` by value.
7. **`[this]`**: Capture the `this` pointer, so you can use member variables and methods in a class context.
8. **`[*this]`** (C++20): Capture the **entire object** by value, effectively copying all members.

---

## 4. Best Practices

- **Capture only what you need**: If you only need `x`, do `[x]` instead of `[=]`.
- **Use references** with care**: Ensure the referenced variables outlive the lambda’s usage.
- **Use explicit captures** when clarity matters**: This helps readers see exactly which variables are used and how.
- **`[this]`** is usually best if you need to call member methods. But watch out for object lifetime issues if you store the lambda and call it later.

---

## 5. Putting It All Together

Here is a small program showing **multiple** lambdas with different capture strategies:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int x = 5;
    int y = 10;

    // 1) Capture nothing; can't use x or y inside
    auto noCapture = []() {
        std::cout << "No capture.\n";
    };
    noCapture();

    // 2) Capture x by value, but not y
    auto captureXValue = [x]() {
        std::cout << "x by value = " << x << "\n";
    };
    captureXValue();

    // 3) Capture all used variables by value
    auto captureAllValue = [=]() {
        std::cout << "x + y = " << (x + y) << " (by value)\n";
    };
    captureAllValue();

    // 4) Capture all used variables by reference
    auto captureAllRef = [&]() {
        x += 2; // modifies x in outer scope
        y += 2; // modifies y in outer scope
        std::cout << "x + y (modified by reference) = " << (x + y) << "\n";
    };
    captureAllRef();
    std::cout << "Outside after captureAllRef: x = " << x << ", y = " << y << "\n";

    // 5) Mixed captures: capture x by reference, y by value
    auto mixedCap = [&x, y]() {
        x += 3;  // modifies original x
        // y is captured by value, so this is a copy
        std::cout << "Inside mixedCap: x=" << x << ", y=" << y << "\n";
    };
    mixedCap();
    std::cout << "Outside after mixedCap: x = " << x << ", y = " << y << "\n";

    return 0;
}
```

**Output Explanation**:
1. `noCapture()` just prints a message without using `x` or `y`.
2. `captureXValue()` prints the old value of `x`.
3. `captureAllValue()` captures both `x` and `y` by value and prints their sum.
4. `captureAllRef()` modifies `x` and `y` in the outer scope by reference.
5. `mixedCap()` captures `x` by reference and `y` by value. It modifies the real `x`, but only sees a copy of `y`.

---

### Final Notes

- Lambda captures greatly simplify passing local variables into **callbacks**, **thread functions**, **STL algorithms**, etc.  
- By **default** (C++11, C++14), capturing by **value** (`[=]`) implies those captured variables act like **const** inside the lambda unless you add the `mutable` keyword. By reference, you can modify them directly.  
- Always be mindful of **object lifetimes** when capturing by reference—avoid dangling references.  
- With practice, you’ll quickly learn which capture mode is most suitable for each scenario.

**That’s it!** Now you have a thorough understanding and **several examples** of how to use **lambda captures** in C++.