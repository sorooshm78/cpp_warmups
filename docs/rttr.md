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