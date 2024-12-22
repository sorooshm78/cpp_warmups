template <typename T>
class SharedPtr
{
private:
    T* pointer;
    int* count;

public:
    SharedPtr();
    SharedPtr(T* pointer);
    SharedPtr(SharedPtr& other);
    SharedPtr& operator=(SharedPtr& other);
    SharedPtr(SharedPtr&& other);
    SharedPtr& operator=(SharedPtr&& other);
    ~SharedPtr();

    T& operator*() const;
    T* operator->() const;
    bool operator!() const;
    operator bool() const;
    int use_count() const ;
    T* get() const;
    void reset();
};

#include "shared-inl.h"
