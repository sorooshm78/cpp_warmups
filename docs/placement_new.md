[Ref](https://www.geeksforgeeks.org/cpp/placement-new-operator-cpp/)

### What is **placement new** in C++?

**Placement new** is a special version of the `new` operator in C++ that constructs an object in a pre-allocated memory buffer. It allows you to separate memory allocation from object construction.

---

### Syntax

```cpp
new (pointer) Type(args);
```

Here:

* `pointer` is a memory location already allocated (like from `malloc`, stack buffer, or custom allocator).
* `Type` is the type of the object you want to construct.
* `args` are the constructor arguments.

---

### Why Use Placement New?

* **Custom memory management**: Useful in embedded systems or performance-critical applications.
* **Avoid frequent allocations**: E.g., using memory pools.
* **Construct objects in pre-allocated buffers**: For controlling memory layout or in-place object replacement.

---

### Example: Basic Usage of Placement New

```cpp
#include <iostream>
#include <new>     // For std::nothrow

class MyClass {
public:
    MyClass(int x) : value(x) {
        std::cout << "Constructor called with value: " << value << std::endl;
    }
    ~MyClass() {
        std::cout << "Destructor called for value: " << value << std::endl;
    }
private:
    int value;
};

int main() {
    // Step 1: Allocate raw memory (enough to hold MyClass)
    char buffer[sizeof(MyClass)];

    // Step 2: Use placement new to construct object in buffer
    MyClass* obj = new (buffer) MyClass(42);

    // Use the object
    // ...

    // Step 3: Manually call the destructor
    obj->~MyClass();

    // No need to free buffer, since it was stack allocated
    return 0;
}
```

---

### Key Points

1. **No memory allocation** is done by placement `new`; memory is assumed to already exist.
2. You **must manually call the destructor** if you use placement new.
3. Don't use `delete` on objects created with placement new — it only applies to regular `new`.

---

### When Not to Use Placement New

* If you don’t need fine-grained memory control, prefer standard `new/delete`.
* Misuse can lead to memory leaks or undefined behavior if you forget destructor calls or overwrite un-destroyed objects.

---

-------------
-------------
-------------
-------------


Sure! Let's break down **placement new** in C++ in detail.

---

## 🔹 What is "placement new" in C++?

In C++, the `new` operator is normally used to:

1. Allocate memory.
2. Call the constructor of the object in that memory.

### Example:

```cpp
MyClass* obj = new MyClass(); // allocates memory + calls constructor
```

However, **placement new** allows you to **construct an object in a pre-allocated memory buffer**. This means **you handle memory allocation manually**, and then call the constructor using **placement new**.

---

## 🔹 Syntax of Placement New

```cpp
void* buffer = malloc(sizeof(MyClass));      // Step 1: allocate memory manually
MyClass* obj = new(buffer) MyClass();        // Step 2: placement new (construct object)
```

Notice this part:

```cpp
new(buffer) MyClass()
```

It means: construct a `MyClass` object at the memory address pointed to by `buffer`.

---

## 🔹 Why use placement new?

You might need **placement new** in cases such as:

1. **Custom memory management**
   For example, in performance-critical systems (games, embedded systems), you might allocate a memory pool and construct objects in that pre-allocated memory.

2. **Avoiding repeated allocations**
   You want to allocate a block of memory once and construct objects in-place to reduce fragmentation and overhead.

3. **Constructing at a specific address**
   Sometimes you need to place an object at a specific memory address (e.g., in a shared memory segment, hardware-mapped address, etc.)

4. **Object pools / arena allocators**
   You may reuse memory many times for different objects — placement new constructs an object in a reused memory block.

---

## 🔹 Example: Using placement new

```cpp
#include <iostream>
#include <cstdlib> // for malloc, free

class MyClass {
public:
    MyClass(int x) {
        std::cout << "Constructor called with x = " << x << std::endl;
        this->x = x;
    }
    ~MyClass() {
        std::cout << "Destructor called for x = " << x << std::endl;
    }
private:
    int x;
};

int main() {
    // Allocate raw memory
    void* memory = malloc(sizeof(MyClass));

    // Use placement new to construct object
    MyClass* obj = new(memory) MyClass(42);

    // Manually call destructor
    obj->~MyClass();

    // Free raw memory
    free(memory);

    return 0;
}
```

### Output:

```
Constructor called with x = 42
Destructor called for x = 42
```

---

## 🔹 What problem does placement new solve?

It gives you **fine-grained control** over **object lifetime** and **memory allocation**, allowing:

* Pre-allocation for speed and determinism.
* Avoiding heap allocations (important in embedded systems).
* Using shared memory or memory-mapped files.
* Implementing allocators or custom containers efficiently.

---

## 🔹 Caution!

* If you use placement new, **you are responsible for calling the destructor manually**.
* Mixing placement new and normal `delete` is **undefined behavior**.
* You must ensure the buffer is properly aligned for the object you're constructing.

---

## 🔹 Summary Table

| Aspect            | Regular `new`          | Placement `new`            |
| ----------------- | ---------------------- | -------------------------- |
| Memory allocation | Yes                    | No (you allocate manually) |
| Calls constructor | Yes                    | Yes                        |
| Calls destructor  | With `delete`          | You must call manually     |
| Use case          | Simple heap allocation | Custom memory management   |
| Syntax            | `new MyClass()`        | `new(ptr) MyClass()`       |

---

Let me know if you want an example with arrays, custom allocators, or performance profiling.

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

# Placement new operator in C++
Last Updated : 23 Jul, 2025

Placement new is a variation new operator in C++. Normal new operator does two things : (1) Allocates memory (2) Constructs an object in allocated memory. 
Placement new allows us to separate above two things. In placement new, we can pass a preallocated memory and construct an object in the passed memory. 
 

new vs placement new
    Normal new allocates memory in heap and constructs objects there whereas using placement new, object construction can be done at known address.
    With normal new, it is not known that, at what address or memory location it's pointing to, whereas the address or memory location that it's pointing is known while using placement new.
    The deallocation is done using delete operation when allocation is done by new but there is no placement delete, but if it is needed one can write it with the help of destructor


Syntax: 
```
new (address) (type) initializer
As we can see, we can specify an address
where we want a new object of given type 
to be constructed.
```

## When to prefer using placement new?
As it allows to construct an object on memory that is already allocated, it is required for optimizations as it is faster not to re-allocate all the time. There may be cases when it is required to re-construct an object multiple times so, placement new operator might be more efficient in these cases. 
 
```
// C++ program to illustrate the placement new operator

#include<iostream>

using namespace std;


int main()

{

    // buffer on stack

    unsigned char buf[sizeof(int)*2] ;​
    
    // placement new in buf

    int *pInt = new (buf) int(3);


    int *qInt = new (buf + sizeof (int)) int(5);

    int *pBuf = (int*)(buf+0) ;

    int *qBuf = (int*) (buf + sizeof(int));

    cout << "Buff Addr             Int Addr" << endl;

    cout << pBuf <<"             " << pInt << endl;

    cout << qBuf <<"             " << qInt << endl;

    cout << "------------------------------" << endl;

    cout << "1st Int             2nd Int" << endl;

    cout << *pBuf << "                         "

         << *qBuf << endl;

    return 0;

}
```

Output: 
``` 
Buff Addr              Int Addr
0x69fed8               0x69fed8
0x69fedc               0x69fedc
------------------------------
1st Int                2nd Int
3                      5
```

The diagram below pictorially shows what is actually happening in above C++ program.
 
![](https://media.geeksforgeeks.org/wp-content/uploads/final1.jpg)

Below is a another simple implementation in C++ to illustrate the use of placement new in C++ :
 
```
// C++ program to illustrate the placement new operator

#include<iostream>

using namespace std;

int main()

{

    // initial value of X

    int X = 10;

    cout << "Before placement new :" << endl;

    cout << "X : " << X << endl;

    cout << "&X : " << &X << endl;​

    // Placement new changes the value of X to 100

    int *mem = new (&X) int(100);

    cout << "\nAfter placement new :" << endl;

    cout << "X : " << X << endl;

    cout << "mem : " << mem << endl;

    cout << "&X : " << &X << endl;

    return 0;

}
```

Output: 
```
Before placement new :
X : 10
&X : 0x69fee8

After placement new :
X : 100
mem : 0x69fee8
&X : 0x69fee8
```

Explanation: Here, it is clear that a new value of x is assigned at the address of x with the help of placement new operator. This is clear by the fact that the value of &X and mem is equal. 
The diagram below pictorially shows what is actually happening in above C++ program.
 
![](https://media.geeksforgeeks.org/wp-content/uploads/98.jpg)

## How to delete the memory allocated by placement new ?

The operator delete can only delete the storage created in heap, so when placement new is used delete operator cannot be used to delete the storage. In the case of memory allocation using placement new operator , since it is created in stack the compiler knows when to delete it and it will handle deallocation of the memory automatically. If required, one can write it with the help of destructor as shown below.
 
```
// C++ program to illustrate using destructor for

// deleting memory allocated by placement new

#include<iostream>

#include<cstdlib>

#include<cmath>

using namespace std;

class Complex
{
private:

    double re_, im_;
public:
    // Constructor
    Complex(double re = 0, double im = 0): re_(re), im_(im)
    {
        cout << "Constructor : (" << re_
             << ", " << im_ << ")" << endl;
    }

    // Destructor
    ~Complex()
    {
        cout << "Destructor : (" << re_ << ", "

             << im_ << ")" << endl;
    }

    double normal()
    {

        return sqrt(re_*re_ + im_*im_);

    }

    void print()

    {

        cout << "|" << re_ <<" +j" << im_

             << " | = " << normal() << endl;

    }

};

​

// Driver code

int main()

{

    // buffer on stack

    unsigned char buf[100];​

    Complex* pc = new Complex(4.2, 5.3);

    Complex* pd = new Complex[2];

    // using placement new

    Complex *pe = new (buf) Complex(2.6, 3.9);​

    // use objects

    pc -> print();

    pd[0].print();

    pd[1].print();

    pe->print();

    // Release objects

    // calls destructor and then release memory

    delete pc;

    // Calls the destructor for object pd[0]

    // and then release memory

    // and it does same for pd[1]

    delete [] pd;

    // No delete : Explicit call to Destructor.

    pe->~Complex();

    return 0;

}
```

Output: 
```
Constructor : (4.2, 5.3)
Constructor : (0, 0)
Constructor : (0, 0)
Constructor : (2.6, 3.9)
|4.2 +j5.3 | = 6.7624
|0 +j0 | = 0
|0 +j0 | = 0
|2.6 +j3.9 | = 4.68722
Destructor : (4.2, 5.3)
Destructor : (0, 0)
Destructor : (0, 0)
Destructor : (2.6, 3.9)
```

Explanation: Here the destructor is explicitly called because here it cannot be packaged within the delete operator because delete will need to release the memory which you do not have here and it cannot be implicit as it is a dynamic process which we want to manage yourself. 
 

## When will placement new operator show segmentation fault?


The placement new operator should be used with care. The address which is passed can be a reference or a pointer pointing to a valid memory location. It may show an error when the address passed is : 
    A pointer such as NULL pointer.
    A pointer that is not pointing to any location.
    It cannot be a void pointer unless it points to some location.


 
```
// C++ program to illustrate segmentation fault

// while using placement new operator

#include<iostream>

using namespace std;

int main()
{

    // Fine

    int i = 10;

    int *ipt = &i ;

    int *i1 = new(ipt) int(9) ;

    // Incorrect as ip may not

    // be a valid address

    int *ip;

    int *i2 = new(ip) int(4) ;

    // Fine

    void *vd = &i;

    int *i3 = new(vd) int(34) ;

    // Incorrect as x is not an address

    int x;

    int *i5 = new(x) int(3) ;

    return 0;
}
```

## Segmentation fault
Advantages of placement new operator over new operator

    The address of memory allocation is known before hand.
    Useful when building a memory pool, a garbage collector or simply when performance and exception safety are paramount.
    There's no danger of allocation failure since the memory has already been allocated, and constructing an object on a pre-allocated buffer takes less time.
    This feature becomes useful while working in an environment with limited resources.