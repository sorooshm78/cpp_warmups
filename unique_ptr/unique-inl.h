template<typename T>
UniquePtr<T>::UniquePtr() noexcept
{
    this->pointer = nullptr;
}

template<typename T>
UniquePtr<T>::UniquePtr(T* pointer) noexcept
{
    this->pointer = pointer;
}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept
{
    pointer = other.pointer;
    other.pointer = nullptr;
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept
{
    if (this == &other)
        return *this;

    delete pointer;

    pointer = other.pointer;
    other.pointer = nullptr;

    return *this;
}

template<typename T>
UniquePtr<T>::~UniquePtr() noexcept
{
    delete pointer;
}

template<typename T>
T UniquePtr<T>::operator*() const noexcept
{
    return *pointer;
}

template<typename T>
T* UniquePtr<T>::operator->() const noexcept
{
    return pointer;
}

template<typename T>
bool UniquePtr<T>::operator!() const noexcept
{
    return pointer == nullptr;
}

template<typename T>
UniquePtr<T>::operator bool() const noexcept
{
    return pointer != nullptr;
}

template<typename T>
T* UniquePtr<T>::get() const noexcept
{
    return pointer;
}

template<typename T>
void UniquePtr<T>::reset() noexcept
{
    delete pointer;
    pointer = nullptr;
}

template<typename T>
T* UniquePtr<T>::release() noexcept
{
    T* temp = pointer;
    pointer = nullptr;
    return temp;
}
