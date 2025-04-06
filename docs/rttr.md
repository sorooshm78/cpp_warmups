# RTTR (Run Time Type Reflection) in C++

## What is RTTR?

RTTR is a C++ library that provides runtime type reflection capabilities. Reflection is the ability of a program to examine and modify its own structure and behavior at runtime. While many languages like C# and Java have built-in reflection, C++ doesn't natively support it - that's where RTTR comes in.

## Why Use RTTR?

You might need RTTR for:

1. **Serialization/Deserialization**: Converting objects to/from JSON, XML, etc.
2. **Script Binding**: Exposing C++ classes to scripting languages
3. **GUI Editors**: Creating property editors for game objects
4. **Plugin Systems**: Dynamically loading and interacting with plugins
5. **Debugging Tools**: Inspecting object states at runtime

## Key Features

- Type registration and inspection
- Property and method reflection
- Object creation and destruction
- Enumeration support
- Cross-platform (Windows, Linux, macOS)

## Example Usage

Here's a simple example demonstrating RTTR:

```cpp
#include <rttr/registration>
#include <iostream>

using namespace rttr;

// Class we want to reflect
class Person {
public:
    Person() {}
    Person(const std::string& name) : m_name(name) {}
    
    void greet() const {
        std::cout << "Hello, I'm " << m_name << std::endl;
    }
    
    void setAge(int age) { m_age = age; }
    int getAge() const { return m_age; }
    
private:
    std::string m_name;
    int m_age = 0;
};

// Register the class with RTTR
RTTR_REGISTRATION
{
    registration::class_<Person>("Person")
        .constructor<>()
        .constructor<const std::string&>()
        .property("name", &Person::m_name)
        .property("age", &Person::getAge, &Person::setAge)
        .method("greet", &Person::greet);
}

int main() {
    // Create an instance
    type t = type::get<Person>();
    variant var_person = t.create("John Doe");
    
    // Access properties
    property prop_name = t.get_property("name");
    prop_name.set_value(var_person, "Alice Smith");
    
    property prop_age = t.get_property("age");
    prop_age.set_value(var_person, 30);
    
    // Call methods
    method meth_greet = t.get_method("greet");
    meth_greet.invoke(var_person);
    
    // Get the age
    variant age = prop_age.get_value(var_person);
    std::cout << "Age: " << age.to_int() << std::endl;
    
    // Destroy the instance
    t.destroy(var_person);
    
    return 0;
}
```

## How It Works

1. **Registration**: You register your classes, properties, and methods with RTTR using the registration macros.
2. **Runtime Inspection**: At runtime, you can query these registered types to:
   - Create/destroy instances
   - Get/set property values
   - Invoke methods
   - Check inheritance relationships

## Advanced Example: Iterating Over Properties

```cpp
void print_properties(const rttr::instance& obj) {
    type t = obj.get_derived_type();
    std::cout << "Properties of " << t.get_name() << ":\n";
    
    for (auto& prop : t.get_properties()) {
        variant prop_value = prop.get_value(obj);
        std::cout << "  " << prop.get_name() << " = " 
                  << prop_value.to_string() << "\n";
    }
}

// Usage:
Person person("Bob");
person.setAge(25);
print_properties(person);
```

## Limitations

1. **Performance Overhead**: Reflection has some runtime cost
2. **Build Time**: Registration code can increase compilation time
3. **Private Members**: Need to expose private members you want to reflect

## When to Use RTTR

Consider RTTR when you need dynamic type information at runtime and the benefits outweigh the overhead. For performance-critical code, you might want to avoid it or use it sparingly.

RTTR is particularly useful in frameworks, editors, and tools where flexibility and introspection are more important than raw performance.


-------------------------------------------------------------------------

The `rttr::type::get_by_name` function in RTTR is used to retrieve a type based on its name (as a string). This is useful when you want to dynamically query a type at runtime using its name, which might be provided by the user or determined at runtime rather than at compile time.

### Syntax:
```cpp
static rttr::type rttr::type::get_by_name(const std::string& name);
```

### Parameters:
- **name**: The name of the type (class, struct, or enum) as a string.

### Return Value:
- The function returns an `rttr::type` object representing the type with the specified name, or an invalid `rttr::type` object if the type cannot be found.

### Use Case:
- **Dynamic Type Resolution**: If you have a type name as a string (e.g., from user input or a configuration file), you can use `get_by_name` to retrieve the corresponding type and perform reflection on it.
- **Plugin Systems**: In plugin systems or dynamic loading scenarios, you might want to load types dynamically based on their names.

### Example:

Let's say we have the following class `Person` and we want to dynamically retrieve its type using `rttr::type::get_by_name`.

1. **Class Definition and Registration:**

   ```cpp
   #include <rttr/registration>
   #include <iostream>
   #include <string>

   class Person {
   public:
       Person() = default;
       Person(std::string name, int age) : name(name), age(age) {}

       const std::string& getName() const { return name; }
       int getAge() const { return age; }

   private:
       std::string name;
       int age;
   };

   RTTR_REGISTRATION {
       rttr::registration::class_<Person>("Person")
           .constructor<>()
           .constructor<std::string, int>()
           .property("name", &Person::getName)
           .property("age", &Person::getAge);
   }
   ```

2. **Using `get_by_name`:**

   In the main function, we will use `rttr::type::get_by_name` to retrieve the type of the `Person` class dynamically by its name and then reflect on its properties.

   ```cpp
   int main() {
       // Register a person object
       Person p("Alice", 25);

       // Retrieve type by name
       std::string type_name = "Person";
       rttr::type person_type = rttr::type::get_by_name(type_name);

       if (person_type.is_valid()) {
           std::cout << "Class name: " << person_type.get_name() << std::endl;

           // Print properties of the class
           for (auto& prop : person_type.get_properties()) {
               std::cout << "Property: " << prop.get_name() << ", Value: " << prop.get_value(p).to_string() << std::endl;
           }
       } else {
           std::cout << "Type not found: " << type_name << std::endl;
       }

       return 0;
   }
   ```

### Explanation:

1. **Class Registration:**
   - We register the `Person` class with RTTR using the `RTTR_REGISTRATION` macro. This allows RTTR to reflect on the `Person` class and its properties (`name` and `age`).
   
2. **Dynamic Type Retrieval:**
   - We specify the type name (`"Person"`) as a string.
   - We use `rttr::type::get_by_name("Person")` to retrieve the `rttr::type` object corresponding to the `Person` class.
   - If the type is valid (i.e., RTTR successfully found the type), we print the class name and properties.
   - We use `prop.get_value(p)` to get the value of each property for the `p` object.

3. **Output:**
   ```plaintext
   Class name: Person
   Property: name, Value: Alice
   Property: age, Value: 25
   ```

### Use Cases for `get_by_name`:

- **Dynamic Object Creation:** If you have the name of a class and want to create objects or interact with them dynamically, `get_by_name` can be used to retrieve the type and invoke methods.
  
- **Plugin Systems:** You can register types dynamically via name strings in plugin-based systems where new types can be loaded at runtime.
  
- **Scripting or User-Driven Systems:** In scenarios where the user specifies the type name at runtime (e.g., through scripting or a configuration file), `get_by_name` allows you to resolve the type and interact with it.

### Considerations:
- The type name provided to `get_by_name` must match the exact name used during the type registration process (including case sensitivity).
- If the type name is incorrect or the type hasn’t been registered, `get_by_name` will return an invalid `rttr::type`, and you should check the validity using `is_valid()`.

In conclusion, `rttr::type::get_by_name` is a powerful feature that enables dynamic type resolution in C++ using the RTTR library. It allows you to retrieve types based on their names at runtime, making it useful in various flexible and dynamic applications such as plugin systems, serialization, and scripting interfaces.

---------------------------------------------------------------------

## get_method and get_parameter_infos
In RTTR (Run-Time Type Reflection), `get_method` and `get_parameter_infos` are used to inspect and interact with the methods of a class dynamically at runtime. These functions allow you to retrieve information about the methods of a class, including the parameters of each method, and then call those methods or extract details about their signatures.

### `get_method`
The `get_method` function is used to retrieve a method by its name from a class type. This allows you to inspect and invoke methods dynamically.

#### Syntax:
```cpp
rttr::method rttr::type::get_method(const std::string& name) const;
```

#### Parameters:
- **name**: The name of the method you want to retrieve.

#### Return Value:
- Returns an `rttr::method` object representing the method with the specified name. If the method is not found, it returns an invalid `rttr::method`.

### `get_parameter_infos`
Once you have obtained a `method` object using `get_method`, you can call `get_parameter_infos` on the method to get detailed information about the parameters that the method takes. This includes the parameter names, types, and their default values if available.

#### Syntax:
```cpp
std::vector<rttr::parameter_info> rttr::method::get_parameter_infos() const;
```

#### Return Value:
- Returns a `std::vector` of `rttr::parameter_info` objects, which provide details about each parameter of the method.

#### Example:

Let's go through a complete example to understand how `get_method` and `get_parameter_infos` work.

### Example Code:

```cpp
#include <rttr/registration>
#include <iostream>
#include <string>
#include <vector>

// Define a class with methods
class MyClass {
public:
    MyClass() = default;

    // Method with no parameters
    void greet() {
        std::cout << "Hello!" << std::endl;
    }

    // Method with parameters
    void setPersonInfo(std::string name, int age) {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }

    // Method with default parameters
    void displayInfo(std::string city = "Unknown", int year = 2023) {
        std::cout << "City: " << city << ", Year: " << year << std::endl;
    }
};

// Register the class for RTTR reflection
RTTR_REGISTRATION {
    rttr::registration::class_<MyClass>("MyClass")
        .constructor<>()
        .method("greet", &MyClass::greet)
        .method("setPersonInfo", &MyClass::setPersonInfo)
        .method("displayInfo", &MyClass::displayInfo);
}

int main() {
    // Retrieve the type of MyClass
    rttr::type t = rttr::type::get<MyClass>();

    // Get the "setPersonInfo" method by name
    rttr::method method = t.get_method("setPersonInfo");

    // Check if the method is valid
    if (method.is_valid()) {
        std::cout << "Method found: " << method.get_name() << std::endl;

        // Get parameter info for the "setPersonInfo" method
        std::vector<rttr::parameter_info> params = method.get_parameter_infos();

        // Print parameter details
        for (const auto& param : params) {
            std::cout << "Parameter: " << param.get_name() << ", Type: " << param.get_type().get_name() << std::endl;
        }

        // Invoke the method dynamically with parameters
        MyClass obj;
        method.invoke(obj, "John", 30);
    } else {
        std::cout << "Method not found." << std::endl;
    }

    // Get the "displayInfo" method by name
    method = t.get_method("displayInfo");

    if (method.is_valid()) {
        std::cout << "\nMethod found: " << method.get_name() << std::endl;

        // Get parameter info for "displayInfo" method
        std::vector<rttr::parameter_info> params = method.get_parameter_infos();
        for (const auto& param : params) {
            std::cout << "Parameter: " << param.get_name() << ", Type: " << param.get_type().get_name() << std::endl;
        }

        // Invoke the method with default parameters
        method.invoke(obj);
    } else {
        std::cout << "Method not found." << std::endl;
    }

    return 0;
}
```

### Explanation of the Code:

1. **Class Definition:**
   - We define a class `MyClass` with three methods:
     - `greet()`: No parameters.
     - `setPersonInfo(std::string, int)`: Takes two parameters (name and age).
     - `displayInfo(std::string, int)`: Has two parameters with default values (city and year).

2. **RTTR Registration:**
   - We register the class `MyClass` and its methods (`greet`, `setPersonInfo`, `displayInfo`) using the `RTTR_REGISTRATION` macro.

3. **Using `get_method`:**
   - We retrieve the `setPersonInfo` method by calling `get_method("setPersonInfo")` on the `rttr::type` object of `MyClass`.
   - If the method is valid, we print the method name and its parameters using `get_parameter_infos`.

4. **Inspecting Parameters with `get_parameter_infos`:**
   - For the `setPersonInfo` method, we use `method.get_parameter_infos()` to retrieve information about the parameters.
   - We then loop through the parameters and print their names and types.
   - We invoke the method dynamically with the arguments `"John"` and `30` using `method.invoke`.

5. **Output:**

   ```plaintext
   Method found: setPersonInfo
   Parameter: name, Type: string
   Parameter: age, Type: int
   Name: John, Age: 30

   Method found: displayInfo
   Parameter: city, Type: string
   Parameter: year, Type: int
   City: Unknown, Year: 2023
   ```

### Key Concepts:

- **`get_method`:** This retrieves a method by its name from a class type. You can use it to dynamically inspect or invoke methods.
  
- **`get_parameter_infos`:** This retrieves a list of `parameter_info` objects, which contain details about the parameters of the method (e.g., name, type). This allows you to dynamically analyze the method's signature and pass appropriate arguments when invoking it.

- **Method Invocation:** Once you have the `method` object, you can invoke it dynamically on an object using the `invoke` function.

### Use Cases:

- **Dynamic Method Invocation:** If you need to invoke a method based on user input or dynamic conditions, `get_method` and `get_parameter_infos` provide a way to interact with methods without knowing their signatures at compile time.
- **Plugins or Scripting Systems:** You can use reflection to call methods from external plugins or script files without having to hard-code their names and signatures.
- **Serialization and Deserialization:** These functions can help automatically map method parameters when serializing or deserializing objects.

### Considerations:

- **Performance:** Reflection (and dynamic method invocation) introduces some overhead, so it’s not ideal for performance-critical parts of your application.
- **Method Name Matching:** The method name provided to `get_method` must exactly match the registered name, including case sensitivity.

In summary, `get_method` and `get_parameter_infos` in RTTR allow you to dynamically inspect methods and their parameters in C++ and invoke them without needing to know their details at compile time. This is useful in dynamic systems, such as plugins, scripting, and reflection-based frameworks.


--------------------------------------------------------------------------

Great question!

In **RTTR**, the method `get_wrapped_value()` is used on `rttr::variant` objects to **unwrap** the actual underlying value, especially when you're dealing with types that are wrapped—like pointers, `std::reference_wrapper`, or even smart pointers.

---

## 🔍 What is `get_wrapped_value()`?

When you reflect over properties or invoke methods, RTTR returns a `variant` object. Sometimes this variant doesn't hold the value directly, but a **wrapper** (like a reference or pointer). The method `get_wrapped_value()` lets you **unwrap** the actual object/value from that container.

---

## 🔧 Syntax:

```cpp
rttr::variant rttr::variant::get_wrapped_value() const;
```

---

## 📦 When is this useful?

Imagine you have a property that returns a reference or pointer to an object. If you want to access or manipulate the real value that’s inside, you'll need `get_wrapped_value()`.

---

## ✅ Example

### 1. **Class Definition**

```cpp
#include <rttr/registration>
#include <iostream>
#include <string>
#include <functional> // For std::reference_wrapper

class User {
public:
    std::string name = "Guest";
};

class Manager {
public:
    User& getUser() { return user; }

private:
    User user;
};

RTTR_REGISTRATION
{
    rttr::registration::class_<User>("User")
        .property("name", &User::name);

    rttr::registration::class_<Manager>("Manager")
        .method("getUser", &Manager::getUser);
}
```

---

### 2. **Using `get_wrapped_value()`**

```cpp
int main() {
    Manager mgr;

    // Get the type of Manager
    rttr::type mgr_type = rttr::type::get<Manager>();

    // Get the method
    rttr::method get_user_method = mgr_type.get_method("getUser");

    // Call the method, which returns a reference to User
    rttr::variant wrapped = get_user_method.invoke(mgr);

    // Check if the result is a wrapper
    if (wrapped.is_valid()) {
        std::cout << "Before unwrap: Type = " << wrapped.get_type().get_name() << std::endl;

        // Get the unwrapped value (i.e., the actual User object)
        rttr::variant unwrapped = wrapped.get_wrapped_value();

        std::cout << "After unwrap: Type = " << unwrapped.get_type().get_name() << std::endl;

        // Access the 'name' property of the unwrapped User
        rttr::type user_type = unwrapped.get_type();
        rttr::property name_prop = user_type.get_property("name");

        if (name_prop.is_valid()) {
            std::cout << "Original name: " << name_prop.get_value(unwrapped).to_string() << std::endl;

            // Change the name
            name_prop.set_value(unwrapped, "Alice");

            std::cout << "Updated name: " << name_prop.get_value(unwrapped).to_string() << std::endl;
        }
    }

    return 0;
}
```

---

### 🧾 Output:

```plaintext
Before unwrap: Type = User&
After unwrap: Type = User
Original name: Guest
Updated name: Alice
```

---

## 🔑 Key Takeaways:

| Concept                | Explanation                                      |
|------------------------|--------------------------------------------------|
| `get_wrapped_value()`  | Unwraps the value from a reference or pointer    |
| Useful when            | Method/property returns `User&`, `User*`, etc.  |
| Without unwrapping     | You can't reflect deeper on the actual object   |
| After unwrapping       | You can access and modify real object values    |

---

If you’re working with **references**, **pointers**, or **wrappers**, always check if you need `get_wrapped_value()` to get the actual instance for reflection.

Awesome! Let’s look at how `get_wrapped_value()` works when the method returns a **pointer** or a **smart pointer** (like `std::shared_ptr` or `std::unique_ptr`). RTTR handles these similarly — the returned `variant` holds the wrapper, and you need to **unwrap** it to reflect on the actual object.

---

## ✅ Example: Using `User*` (raw pointer)

### 🔧 Class Setup

```cpp
#include <rttr/registration>
#include <iostream>
#include <string>

class User {
public:
    std::string name = "Guest";
};

class Manager {
public:
    User* getUser() { return &user; }

private:
    User user;
};

RTTR_REGISTRATION {
    rttr::registration::class_<User>("User")
        .property("name", &User::name);

    rttr::registration::class_<Manager>("Manager")
        .method("getUser", &Manager::getUser);
}
```

---

### 🧠 Reflect and Unwrap Pointer

```cpp
int main() {
    Manager mgr;

    // Get type
    rttr::type mgr_type = rttr::type::get<Manager>();

    // Get method
    rttr::method get_user = mgr_type.get_method("getUser");

    // Invoke the method (returns User*)
    rttr::variant wrapped = get_user.invoke(mgr);

    if (wrapped.is_valid()) {
        std::cout << "Before unwrap: " << wrapped.get_type().get_name() << std::endl;

        // Unwrap the pointer to get actual User object
        rttr::variant unwrapped = wrapped.get_wrapped_value();

        std::cout << "After unwrap: " << unwrapped.get_type().get_name() << std::endl;

        // Reflect on the unwrapped object
        rttr::property name_prop = unwrapped.get_type().get_property("name");

        if (name_prop.is_valid()) {
            std::cout << "Original name: " << name_prop.get_value(unwrapped).to_string() << std::endl;

            name_prop.set_value(unwrapped, "Bob");

            std::cout << "Updated name: " << name_prop.get_value(unwrapped).to_string() << std::endl;
        }
    }
}
```

---

### 🧾 Output:

```plaintext
Before unwrap: User*
After unwrap: User
Original name: Guest
Updated name: Bob
```

---

## 🔁 Example: With `std::shared_ptr<User>`

### 🧱 Class Setup

```cpp
#include <memory> // For shared_ptr

class SmartManager {
public:
    std::shared_ptr<User> getSmartUser() {
        return std::make_shared<User>();
    }
};

RTTR_REGISTRATION {
    rttr::registration::class_<SmartManager>("SmartManager")
        .method("getSmartUser", &SmartManager::getSmartUser);
}
```

---

### 💡 Reflect and Unwrap Shared Pointer

```cpp
SmartManager smart_mgr;
rttr::type smart_mgr_type = rttr::type::get<SmartManager>();
rttr::method smart_method = smart_mgr_type.get_method("getSmartUser");

rttr::variant smart_wrapped = smart_method.invoke(smart_mgr);

if (smart_wrapped.is_valid()) {
    std::cout << "Before unwrap: " << smart_wrapped.get_type().get_name() << std::endl;

    rttr::variant smart_unwrapped = smart_wrapped.get_wrapped_value();

    std::cout << "After unwrap: " << smart_unwrapped.get_type().get_name() << std::endl;

    rttr::property name_prop = smart_unwrapped.get_type().get_property("name");
    if (name_prop.is_valid()) {
        name_prop.set_value(smart_unwrapped, "Charlie");
        std::cout << "SharedPtr name: " << name_prop.get_value(smart_unwrapped).to_string() << std::endl;
    }
}
```

---

### 📦 Output:

```plaintext
Before unwrap: std::shared_ptr<User>
After unwrap: User
SharedPtr name: Charlie
```

---

## 🧠 Summary Table

| Return Type        | `variant` Type Before Unwrap | Type After `get_wrapped_value()` |
|--------------------|------------------------------|----------------------------------|
| `User&`            | `User&`                      | `User`                           |
| `User*`            | `User*`                      | `User`                           |
| `std::shared_ptr<User>` | `std::shared_ptr<User>`      | `User`                           |
| `std::unique_ptr<User>` | `std::unique_ptr<User>`      | `User`                           |

---
