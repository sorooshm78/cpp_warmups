template<typename T>
class UniquePtr {
private:
    T* pointer;

public:
    UniquePtr() noexcept;
    explicit UniquePtr(T* pointer) noexcept;
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) noexcept;
    UniquePtr& operator=(UniquePtr&& other) noexcept;
    ~UniquePtr() noexcept;

    T operator*() const noexcept;
    T* operator->() const noexcept;
    bool operator!() const noexcept;
    explicit operator bool() const noexcept;
    T* get() const noexcept;
    void reset() noexcept;
    T* release() noexcept;
};

#include "unique-inl.h"