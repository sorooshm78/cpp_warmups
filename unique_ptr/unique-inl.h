template<typename T>
UniquePtr<T>::UniquePtr() {
    this->pointer = nullptr;
}

template<typename T>
UniquePtr<T>::UniquePtr(T* pointer) {
    this->pointer = pointer;
}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) {
    pointer = other.pointer;
    other.pointer = nullptr;
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) {
    if (this == &other)
        return *this;

    delete pointer;

    pointer = other.pointer;
    other.pointer = nullptr;

    return *this;
}

template<typename T>
UniquePtr<T>::~UniquePtr() {
    delete pointer;
}

template<typename T>
T UniquePtr<T>::operator*() const{
    return *pointer;
}

template<typename T>
T* UniquePtr<T>::operator->() const{
    return pointer;
}

template<typename T>
bool UniquePtr<T>::operator!() const{
    return pointer == nullptr;
}

template<typename T>
UniquePtr<T>::operator bool() const{
    return pointer != nullptr;
}

template<typename T>
T* UniquePtr<T>::get() const{
    return pointer;
}

template<typename T>
void UniquePtr<T>::reset() {
    delete pointer;
    pointer = nullptr;
}

template<typename T>
T* UniquePtr<T>::release() {
    T* temp = pointer;
    pointer = nullptr;
    return temp;
}
