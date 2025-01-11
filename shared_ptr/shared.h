template <typename T>
class SharedPtr
{
private:
    T* pointer;
    int* count;

public:
    SharedPtr() noexcept;
    explicit SharedPtr(T* pointer) noexcept;
    SharedPtr(SharedPtr& other) noexcept;
    SharedPtr& operator=(SharedPtr& other) noexcept;
    SharedPtr(SharedPtr&& other) noexcept;
    SharedPtr& operator=(SharedPtr&& other) noexcept;
    ~SharedPtr() noexcept;

    T& operator*() const noexcept;
    T* operator->() const noexcept;
    bool operator!() const noexcept;
    explicit operator bool() const noexcept;
    int use_count() const noexcept;
    T* get() const noexcept;
    void reset() noexcept;
};

#include "shared-inl.h"
