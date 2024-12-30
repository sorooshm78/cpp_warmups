template <typename T>
SharedPtr<T>::SharedPtr()
{
    this->pointer = nullptr;
    count = new int(0);
}

template <typename T>
SharedPtr<T>::SharedPtr(T* const pointer)
{
    this->pointer = pointer;
    count = new int(1);
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr& other)
{
    pointer = other.pointer;
    count = other.count;
    (*count)++;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr& other)
{
    if (this == &other)
        return *this;

    pointer = other.pointer;
    count = other.count;
    (*count)++;
    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other)
{
    count = other.count;
    pointer = other.pointer;

    other.pointer = nullptr;
    other.count = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other)
{
    if (this == &other)
        return *this;

    if (count && --(*count) == 0)
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
SharedPtr<T>::~SharedPtr()
{
    if (count && --(*count) == 0)
    {
        delete pointer;
        delete count;
    }
}

template <typename T>
T& SharedPtr<T>::operator*() const
{
    return *pointer;
}

template <typename T>
T* SharedPtr<T>::operator->() const
{
    return pointer;
}

template <typename T>
bool SharedPtr<T>::operator!() const
{
    return pointer == nullptr;
}

template <typename T>
SharedPtr<T>::operator bool() const
{
    return pointer != nullptr;
}

template <typename T>
T* SharedPtr<T>::get() const
{
    return pointer;
}

template <typename T>
void SharedPtr<T>::reset()
{
    if (count && --(*count) == 0)
    {
        delete pointer;
        delete count;
    }
    pointer = nullptr;
    count = nullptr;
}

template <typename T>
int SharedPtr<T>::use_count() const
{
    return count ? *count : 0;
}
