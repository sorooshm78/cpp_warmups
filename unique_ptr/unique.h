template<typename T>
class UniquePtr {
private:
    T* pointer;

public:
    UniquePtr();
    UniquePtr(T* pointer);
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other);
    UniquePtr& operator=(UniquePtr&& other);
    ~UniquePtr();

    T operator*() const;
    T* operator->() const;
    bool operator!() const;
    operator bool() const;
    T* get() const;
    void reset();
    T* release();
};

#include "unique-inl.h"