# Placement `new` C++. [[Ref]](https://medium.com/@ajay.patel305/placement-new-c-b7f1e2e2077a)
The placement new operator in C++ allows you to construct an object in a pre-allocated memory buffer. Unlike the regular new operator, which allocates memory and constructs an object, the placement new only constructs an object at a specified memory location. This can be particularly useful in scenarios where you need fine-grained control over memory allocation, such as in embedded systems or custom memory management schemes.
Press enter or click to view image in full size
Photo by Jefferson Santos on Unsplash

Syntax:
```
void* operator new(size_t size, void* ptr) noexcept;
```

Example Usage:
```
#include <iostream>
#include <new> // Required for placement new

class MyClass {
public:
    MyClass(int value) : value(value) {
        std::cout << "Constructor called, value = " << value << std::endl;
    }
    ~MyClass() {
        std::cout << "Destructor called, value = " << value << std::endl;
    }
private:
    int value;
};

int main() {
    // Pre-allocated memory buffer
    char buffer[sizeof(MyClass)];

    // Use placement new to construct MyClass object in the buffer
    MyClass* obj = new (buffer) MyClass(42);

    // Explicitly call the destructor
    obj->~MyClass();

    return 0;
}
```

Explanation

    Buffer Allocation: The array buffer provides the raw memory to store an object of type MyClass.
    Placement new: The line new (buffer) MyClass(42); constructs a MyClass object at the memory location pointed to by buffer.
    Destructor Call: The destructor is called explicitly with obj->~MyClass(); because the memory wasn't allocated with the regular new, so you must manually manage object destruction.

Key Points

    No Memory Allocation: Placement new doesn't allocate memory; it simply constructs the object at a specified location.
    Manual Destructor Call: You must manually invoke the destructor, as placement new doesn't deallocate memory.
    Custom Memory Management: Useful when you need to control where objects are placed in memory, such as in real-time systems, memory pools, or custom allocators.

Use cases:
1. Custom Memory Management
Press enter or click to view image in full size
Photo by Samsung Memory on Unsplash

In systems where memory is a critical resource, such as embedded systems or real-time applications, managing memory manually can lead to performance gains. The placement new allows you to construct objects in pre-allocated memory, avoiding the overhead of dynamic memory allocation and deallocation.
2. Object Pools

Object pools are a common pattern where a fixed pool of memory is allocated upfront, and objects are created and destroyed within this pool. The placement new operator is used to construct objects in specific slots of the pool, allowing for efficient reuse of memory.
Example:
```
class ObjectPool {
    char pool[100][sizeof(MyClass)];
    bool used[100] = { false };

public:
    MyClass* allocate(int index, int value) {
        if (!used[index]) {
            used[index] = true;
            return new (pool[index]) MyClass(value);
        }
        return nullptr; // Pool slot already in use
    }

    void deallocate(int index) {
        if (used[index]) {
            MyClass* obj = reinterpret_cast<MyClass*>(pool[index]);
            obj->~MyClass();
            used[index] = false;
        }
    }
};
```

3. Placement in Shared Memory
Press enter or click to view image in full size
Photo by Elaine Casap on Unsplash

When using shared memory (e.g., in inter-process communication), you often need to construct objects in a specific, shared memory segment. The placement new operator allows you to create objects directly in this shared memory.
4. Constructing Objects in Pre-Allocated Buffers

Sometimes, you might want to allocate a large block of memory once and use it to construct multiple objects over time. This can reduce fragmentation and improve cache performance, especially in performance-critical applications like game engines.
Get Never Ending Saga’s stories in your inbox

Join Medium for free to get updates from this writer.

Example:
```
class BufferManager {
    char* buffer;
    size_t bufferSize;
    size_t offset;

public:
    BufferManager(size_t size) : bufferSize(size), offset(0) {
        buffer = new char[size];
    }

    ~BufferManager() {
        delete[] buffer;
    }

    void* allocate(size_t size) {
        if (offset + size > bufferSize) return nullptr;
        void* ptr = buffer + offset;
        offset += size;
        return ptr;
    }
};

int main() {
    BufferManager manager(1024);
    void* mem = manager.allocate(sizeof(MyClass));
    MyClass* obj = new (mem) MyClass(123);
    // Use obj...
    obj->~MyClass(); // Don't forget to manually call the destructor!
}
```

5. Performance Optimization
Press enter or click to view image in full size
Photo by Growtika on Unsplash

For performance-critical applications, minimizing heap allocations by reusing pre-allocated memory can be beneficial. The placement new operator helps by allowing the construction of objects in such reusable memory.
Summary:

The placement new operator is a powerful tool for advanced memory management. It allows for the construction of objects in a specific memory location, which can lead to performance improvements and better control over memory usage in scenarios like custom memory allocators, object pools, shared memory, and memory-mapped files.