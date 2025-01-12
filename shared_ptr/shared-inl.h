template <typename T>
SharedPtr<T>::SharedPtr() noexcept
{
    this->pointer = nullptr;
    count = new std::atomic<int>(0);
}

template <typename T>
SharedPtr<T>::SharedPtr(T* const pointer) noexcept
{
    this->pointer = pointer;
    count = new std::atomic<int>(1);
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr& other) noexcept
{
    pointer = other.pointer;
    count = other.count;
    count->fetch_add(1, std::memory_order_relaxed);
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr& other) noexcept
{
    if (this == &other)
        return *this;

    pointer = other.pointer;
    count = other.count;
    count->fetch_add(1, std::memory_order_relaxed);
    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept
{
    count = other.count;
    pointer = other.pointer;

    other.pointer = nullptr;
    other.count = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept
{
    if (this == &other)
        return *this;

    if (count && count->fetch_sub(1, std::memory_order_acq_rel) == 0)
    {
        delete pointer;
        delete count;
    }

    pointer = other.pointer;
    count = other.count;

    other.pointer = nullptr;
    other.count = nullptr;

    return *this;
}

template <typename T>
SharedPtr<T>::~SharedPtr() noexcept
{
    if (count && count->fetch_sub(1, std::memory_order_acq_rel) == 0)
    {
        delete pointer;
        delete count;
    }
}

template <typename T>
T& SharedPtr<T>::operator*() const noexcept
{
    return *pointer;
}

template <typename T>
T* SharedPtr<T>::operator->() const noexcept
{
    return pointer;
}

template <typename T>
bool SharedPtr<T>::operator!() const noexcept
{
    return pointer == nullptr;
}

template <typename T>
SharedPtr<T>::operator bool() const noexcept
{
    return pointer != nullptr;
}

template <typename T>
T* SharedPtr<T>::get() const noexcept
{
    return pointer;
}

template <typename T>
void SharedPtr<T>::reset() noexcept
{
    if (count && count->fetch_sub(1, std::memory_order_acq_rel) == 0)
    {
        delete pointer;
        delete count;
    }
    pointer = nullptr;
    count = nullptr;
}

template <typename T>
int SharedPtr<T>::use_count() const noexcept
{
    return count ? count->load(std::memory_order_relaxed) : 0;
}
