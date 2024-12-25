## Rule of Three
Rule of three
If a class requires a user-defined destructor, a user-defined copy constructor, or a user-defined copy assignment operator, it almost certainly requires all three.

Because C++ copies and copy-assigns objects of user-defined types in various situations (passing/returning by value, manipulating a container, etc), these special member functions will be called, if accessible, and if they are not user-defined, they are implicitly-defined by the compiler.

The implicitly-defined special member functions should not be used if the class manages a resource whose handle does not destroy the resource themselves (raw pointer, POSIX file descriptor, etc), whose destructor does nothing and copy constructor/assignment operator only copies the value of the handle, without duplicating the underlying resource.

```c++
#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>
 
class rule_of_three
{
    char* cstring; // raw pointer used as a handle to a
                   // dynamically-allocated memory block
 
public:
    rule_of_three(const char* s, std::size_t n)
        : cstring(new char[n + 1]) // allocate
    {
        std::memcpy(cstring, s, n); // populate
        cstring[n] = '\0';          // tail 0
    }
 
    explicit rule_of_three(const char* s = "")
        : rule_of_three(s, std::strlen(s))
    {
    }
 
    ~rule_of_three() // I. destructor
    {
        delete[] cstring; // deallocate
    }
 
    rule_of_three(const rule_of_three& other) // II. copy constructor
        : rule_of_three(other.cstring)
    {
    }
 
    rule_of_three& operator=(const rule_of_three& other) // III. copy assignment
    {
        if (this == &other)
            return *this;
 
        rule_of_three temp(other); // use the copy constructor
        std::swap(cstring, temp.cstring); // exchange the underlying resource
 
        return *this;
    }
 
    const char* c_str() const // accessor
    {
        return cstring;
    }
};
 
int main()
{
    rule_of_three o1{"abc"};
    std::cout << o1.c_str() << ' ';
    auto o2{o1}; // II. uses copy constructor
    std::cout << o2.c_str() << ' ';
    rule_of_three o3{"def"};
    std::cout << o3.c_str() << ' ';
    o3 = o2; // III. uses copy assignment
    std::cout << o3.c_str() << '\n';
}
```
Output:
```
abc abc def abc
```
Classes that manage non-copyable resources through copyable handles may have to declare copy assignment and copy constructor private and not provide their definitions(until C++11)define copy assignment and copy constructor as `= delete`(since C++11). This is another application of the rule of three: deleting one and leaving the other to be implicitly-defined typically incorrect.

---

## Rule of Zero
Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership (which follows from the Single Responsibility Principle). Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators[1].

```c++
class rule_of_zero
{
    std::string cppstring;
public:
    // redundant, implicitly defined is better
    // rule_of_zero(const std::string& arg) : cppstring(arg) {}
}
```


---

## Rule of Five
Because the presence of a user-defined (include `= default` or = `delete declared`) destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the move constructor and the move assignment operator, any class for which move semantics are desirable, has to declare all five special member functions:

```c++
#include <cstddef>
#include <cstring>
#include <utility>
 
class rule_of_five
{
    char* cstring; // raw pointer used as a handle to a
                   // dynamically-allocated memory block
 
public:
    rule_of_five(const char* s, std::size_t n)
        : cstring(new char[n + 1]) // allocate
    {
        std::memcpy(cstring, s, n); // populate
        cstring[n] = '\0';          // tail 0
    }
 
    explicit rule_of_five(const char* s)
        : rule_of_five(s, std::strlen(s))
    {
    }
 
    ~rule_of_five() // I. destructor
    {
        delete[] cstring; // deallocate
    }
 
    rule_of_five(const rule_of_five& other) // II. copy constructor
        : rule_of_five(other.cstring)
    {
    }
 
    rule_of_five& operator=(const rule_of_five& other) // III. copy assignment
    {
        if (this == &other)
            return *this;
 
        rule_of_five temp(other); // use the copy constructor
        std::swap(cstring, temp.cstring); // exchange the underlying resource
 
        return *this;
    }
 
    rule_of_five(rule_of_five&& other) noexcept // IV. move constructor
        : cstring(std::exchange(other.cstring, nullptr))
    {
    }
 
    rule_of_five& operator=(rule_of_five&& other) noexcept // V. move assignment
    {
        rule_of_five temp(std::move(other));
        std::swap(cstring, temp.cstring);
        return *this;
    }
};
```

Unlike Rule of Three, failing to provide move constructor and move assignment is usually not an error, but it will result in a loss of performance.
