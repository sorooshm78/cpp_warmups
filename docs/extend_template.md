In C++, the concept of `Extent` is related to the size of arrays or containers at compile time, especially in the context of templates and template metaprogramming. It typically refers to the number of elements that an array or a container can hold, often associated with fixed-size arrays.

### `std::size_t Extent = std::dynamic_extent` in Templates

When you see a template parameter like this:

```cpp
template <typename T, std::size_t Extent = std::dynamic_extent>
```

It is used when defining containers or arrays that might have a size known at compile time or at runtime. `std::dynamic_extent` is a special constant provided by the C++ standard library that signifies that the extent (size) of the container or array is not known at compile time (i.e., it's dynamic).

#### Explanation:

* **`T`**: The type of the elements in the container (e.g., `int`, `double`, etc.).
* **`Extent`**: This is the size of the array or container. The default value `std::dynamic_extent` means that the size is not fixed and may vary at runtime. If you use a fixed-size array, `Extent` will be a constant value (like `5` for an array of size 5).

#### Why Use `Extent`?

Using `Extent` allows for greater flexibility in template functions and classes. If the size of the container is known at compile time, you can use `Extent` to make decisions based on it. If the size is dynamic, you can use `std::dynamic_extent` to handle it at runtime.

For example, the following template is useful for working with arrays or containers where the size might be specified at compile time or runtime:

```cpp
#include <iostream>
#include <array>

template <typename T, std::size_t Extent = std::dynamic_extent>
void print_array(const T (&arr)[Extent]) {
    for (std::size_t i = 0; i < Extent; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    print_array(arr);  // Output: 1 2 3 4 5
}
```

In this case, `Extent` is deduced automatically by the compiler when the size of `arr` is known at compile time.

### `template <typename T, std::size_t TExtent, typename U, std::size_t UExtent>`

This kind of template function declaration:

```cpp
template <typename T, std::size_t TExtent, typename U, std::size_t UExtent>
```

is typically used in situations where you are dealing with two different arrays or containers of potentially different sizes. `T` and `U` are the types of the elements in the respective containers, while `TExtent` and `UExtent` represent their respective sizes.

#### Example:

Suppose you want to implement a function that takes two fixed-size arrays and prints their elements:

```cpp
template <typename T, std::size_t TExtent, typename U, std::size_t UExtent>
void compare_arrays(const T (&arr1)[TExtent], const U (&arr2)[UExtent]) {
    for (std::size_t i = 0; i < std::min(TExtent, UExtent); ++i) {
        std::cout << arr1[i] << " == " << arr2[i] << std::endl;
    }
}

int main() {
    int arr1[] = {1, 2, 3};
    double arr2[] = {1.0, 2.0, 3.0};
    
    compare_arrays(arr1, arr2);
}
```

Here, `TExtent` and `UExtent` are the sizes of `arr1` and `arr2`, respectively. This allows the function to work with arrays of different sizes, performing operations up to the minimum size of both arrays.

### Why Use These Patterns?

1. **Flexibility**: The use of `Extent` and `std::dynamic_extent` allows for flexible templates that can handle both static (fixed-size) and dynamic (variable-size) containers/arrays. If the size of the array is fixed at compile time, you can use the specific size. If the size is dynamic, you can use `std::dynamic_extent` to allow for runtime-sized arrays.

2. **Type Safety**: By using `Extent` as a template parameter, you ensure type safety. The compiler can enforce that the correct size is passed in for static arrays or containers.

3. **Avoiding Dynamic Allocation When Unnecessary**: Using `Extent` helps avoid dynamic allocation when the array size is known at compile time, allowing the program to use the stack instead of heap memory, which can improve performance.

4. **Compile-Time Optimization**: The use of `std::size_t` and compile-time size parameters like `TExtent` allows the compiler to optimize code for fixed-size arrays and perform certain optimizations at compile time that would not be possible with purely dynamic containers.

### Conclusion

To summarize:

* **`std::size_t Extent = std::dynamic_extent`** allows for template functions and classes to handle both fixed-size and dynamically-sized arrays.
* **`template <typename T, std::size_t TExtent, typename U, std::size_t UExtent>`** is used to handle different sizes of two arrays or containers, allowing operations on arrays of potentially different sizes.
* These techniques help in writing flexible, type-safe, and optimized C++ code that works with both static and dynamic data sizes.

-----
-----
-----

در C++، استفاده از پارامتر `Extent` در قالب‌های (`template`) یکی از مفاهیم مهم برای کار با داده‌ها و آرایه‌ها است. به‌ویژه زمانی که شما با `std::span` یا ساختارهای مشابهی که نیاز به تعیین اندازه داده‌ها دارند کار می‌کنید، این ویژگی مفید واقع می‌شود.

### مفهوم `Extent`

`Extent` در C++ نشان‌دهنده اندازه ثابت یا داینامیک یک آرایه یا بازه داده‌ها است که در قالب‌ها (templates) به کار می‌رود. این پارامتر به تابع یا کلاس اجازه می‌دهد تا با داده‌هایی که اندازه آنها مشخص یا داینامیک است، به شکلی ایمن و انعطاف‌پذیر کار کند.

* **نوع ثابت**: زمانی که اندازه داده‌ها در زمان کامپایل مشخص باشد.
* **نوع داینامیک**: زمانی که اندازه داده‌ها در زمان اجرا مشخص شود (مانند `std::dynamic_extent`).

### 1. **`std::size_t Extent`**

زمانی که از `Extent` در قالب‌ها استفاده می‌کنیم، منظور اندازه ثابت یا داینامیک یک داده است. این مقدار می‌تواند یک عدد ثابت باشد (مثلاً ۵) یا از نوع `std::dynamic_extent` باشد که نشان‌دهنده اندازه متغیر در زمان اجرا است.

### 2. **`Extent` در `std::span`**

`std::span` یک نمای (view) از یک بخش داده است و در آن `Extent` برای مشخص کردن تعداد عناصری که در `span` موجود است، استفاده می‌شود. وقتی شما یک `std::span` می‌سازید، باید مشخص کنید که آیا اندازه آن ثابت است (در زمان کامپایل) یا داینامیک (در زمان اجرا).

### کاربردهای `Extent`

* **اندازه ثابت**: زمانی که شما از داده‌هایی با اندازه ثابت استفاده می‌کنید.
* **اندازه داینامیک**: زمانی که شما نمی‌دانید در زمان کامپایل که اندازه دقیق داده‌ها چقدر است و می‌خواهید در زمان اجرا این اندازه را تعیین کنید.

### مثال‌های عملی از `Extent`

#### 1. **اندازه ثابت (در زمان کامپایل)**

در این حالت، اندازه `span` به طور ثابت مشخص می‌شود و در زمان کامپایل ثابت است.

```cpp
#include <iostream>
#include <span>
#include <array>

template <typename T, std::size_t Extent>
void printSpan(std::span<T, Extent> sp) {
    for (auto& elem : sp) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::span<int, 5> sp(arr);  // Extent = 5

    printSpan(sp);  // خروجی: 1 2 3 4 5 
}
```

در این مثال:

* `std::span<int, 5>` یک `span` از نوع `int` با اندازه ثابت ۵ است.
* `Extent` در اینجا برابر با ۵ است که در زمان کامپایل مشخص می‌شود.
* `printSpan` به‌طور دقیق با اندازه ثابت ۵ کار می‌کند.

#### 2. **اندازه داینامیک (در زمان اجرا)**

در این حالت، شما نمی‌دانید که اندازه `span` دقیقاً چقدر است و می‌خواهید اندازه آن در زمان اجرا تعیین شود. در اینجا از `std::dynamic_extent` استفاده می‌شود.

```cpp
#include <iostream>
#include <span>
#include <vector>

template <typename T>
void printSpan(std::span<T> sp) {
    for (auto& elem : sp) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};  // اندازه متغیر
    std::span<int> sp(vec);  // استفاده از std::dynamic_extent

    printSpan(sp);  // خروجی: 1 2 3 4 5
}
```

در این مثال:

* اندازه `std::span<int>` داینامیک است (چون از `std::dynamic_extent` استفاده شده).
* اندازه `span` از `vec` در زمان اجرا مشخص می‌شود.

### چرا `Extent` به کار می‌آید؟

استفاده از `Extent` در قالب‌ها (templates) این امکان را می‌دهد که:

1. **انعطاف‌پذیری در اندازه‌ها**: شما می‌توانید با انواع مختلف داده‌ها با اندازه‌های ثابت و داینامیک کار کنید.
2. **افزایش ایمنی کد**: زمانی که اندازه‌ها مشخص باشند، می‌توان از خطاهای احتمالی جلوگیری کرد (مثلاً دسترسی به خارج از محدوده آرایه‌ها).
3. **عملکرد بهینه‌تر**: در صورتی که اندازه داده‌ها ثابت باشد، می‌توانید از ویژگی‌های بهینه‌سازی کامپایلر استفاده کنید.
4. **سادگی کد**: با استفاده از `Extent` می‌توان کد را ساده‌تر و قابل‌توسعه‌تر نوشت، زیرا پارامتر اندازه به‌طور خودکار از `std::span` استخراج می‌شود.

### مثال پیچیده‌تر با `Extent`:

فرض کنید می‌خواهید یک تابع بنویسید که دو آرایه یا `span` از انواع مختلف داده‌ها با اندازه‌های متفاوت را به هم کپی کند:

```cpp
#include <iostream>
#include <span>
#include <array>
#include <cstring>

template <typename T, std::size_t TExtent, typename U, std::size_t UExtent>
void memcpySpan(std::span<T, TExtent> destination, std::span<U, UExtent> source) {
    if (destination.size() >= source.size()) {
        std::memcpy(destination.data(), source.data(), source.size_bytes());
    }
}

int main() {
    std::array<int, 4> arr1 = {1, 2, 3, 4};
    std::array<float, 4> arr2;

    std::span<int, 4> span1(arr1.data(), 4);
    std::span<float, 4> span2(arr2.data(), 4);

    memcpySpan(span2, span1);  // کپی داده‌ها از arr1 به arr2

    for (auto& val : arr2) {
        std::cout << val << " ";  // خروجی: 1 2 3 4
    }

    return 0;
}
```

در اینجا:

* `TExtent = 4` و `UExtent = 4` است، که نشان‌دهنده اندازه ثابت `span` ها است.
* `memcpySpan` داده‌ها را از `arr1` که یک `span<int, 4>` است به `arr2` که یک `span<float, 4>` است کپی می‌کند.

### نتیجه‌گیری

`Extent` به شما این امکان را می‌دهد که:

* با اندازه‌های ثابت یا داینامیک داده‌ها به‌طور انعطاف‌پذیر کار کنید.
* از `std::span` یا سایر ساختارهای مشابه استفاده کنید که نیاز به مشخص کردن اندازه دارند.
* ایمنی حافظه را بالا ببرید و خطاهای دسترسی به خارج از محدوده داده‌ها را کاهش دهید.
