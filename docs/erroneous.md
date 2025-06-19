![](https://www.sandordargo.com/blog/2025/02/05/cpp26-erroneous-behaviour)

If you pick a random talk at a C++ conference these days, there is a fair chance that the speaker will mention safety at least a couple of times. It’s probably fine like that. The committee and the community must think about improving both the safety situation and the reputation of C++.

If you follow what’s going on in this space, you are probably aware that people have different perspectives on safety. I think almost everybody finds it important, but they would solve the problem in their own way.

A big source of issues is certain manifestations of undefined behaviour. It affects both the safety and the stability of software. I remember that a few years ago when I was working on some services which had to support a 10x growth, one of the important points was to eliminate undefined behaviour as much as possible. One main point for us was to remove uninitialized variables which often lead to crashing services.

Thanks to P2795R5 by Thomas Köppe, uninitialized reads won’t be undefined behaviour anymore - starting from C++26. Instead, they will get a new behaviour called “erroneous behaviour”.

The great advantage of erroneous behaviour is that it will work just by recompiling existing code. It will diagnose where you forgot to initialize variables. You don’t have to systematically go through your code and let’s say declare everything as auto to make sure that every variable has an initialized value. Which you probably wouldn’t do anyway.

But what is this new behaviour that on C++ Reference is even listed on the page of undefined behaviour? It’s well-defined, yet incorrect behaviour that compilers are recommended to diagnose. Is recommended enough?! Well, with the growing focus on safety, you can rest assured that an implementation that wouldn’t diagnose erroneous behaviour would be soon out of the game.

Some compilers can already identify uninitialized reads - what nowadays falls under undefined behaviour. For example, clang and gcc with -ftrivial-auto-var-init=zero have already offered default initialization of variables with automatic storage duration. This means that the technique to identify these variables is already there. The only thing that makes this approach not practical is that you will not know which variables you failed to initialize.

Instead of default initialization, with erroneous behaviour, an uninitialized object will be initialized to an implementation-specific value. Reading such a value is a conceptual error that is recommended and encouraged to be diagnosed by the compiler. That might happen through warnings, run-time errors, etc.

```
void foo() {
  int d;  // d has an erroneous value
  bar(d); // that's erroneous behaviour!
}
```

So looking at the above example, ideally int d; should be already diagnosed at compile-time as a warning. If it’s ignored, at some point, bar(d); will have an effect during program execution, but it should be well-defined, unlike undefined behaviour where anything can happen.

It’s worth noting that undefined behaviour and having erroneous values is not possible in constant expressions. In other words, constexpr protects from it.

Initializing an object to anything has a cost. What if you really want to avoid it and initialize the object later? Will you be able to still do it without getting the diagnostics? Sure! You just have to be deliberate about that. You cannot just leave values uninitialized by accident, you must mark them with C++26’s new attribute, [[indeterminiate]].

```
void foo() {
  int d [[indeterminate]];  // d has an indeterminate value
  bar(d); // that's undefined behaviour!
}
```

We must notice in the example, that d doesn’t have an erroneous value anymore. Now its value is simply indeterminate. On the other hand, if we later use that variable still without initialization, it’s undefined behaviour!

Above, we’ve only talked about variables with automatic storage duration. That’s not the only way to have uninitialized variables. Moreover, probably it’s not even the main way, think about dynamic storage duration, think about pointers! Also, if any member is left uninitialized, the parent object’s value will be considered either indeterminate or erroneous.

```
struct S {
  S() {}
  int num;
  std::string text;
};

int main() {
  [[indeterminate]] S s1; // indeterminate value
  std::cout << s1.num << '\n' // this is UB as s1.num is indeterminate
  S s2;
  std::cout << s2.num << '\n' // this is still UB, s2.num is an erroneous value
}
```

Not only variables variables but function parameters can also be marked [[indeterminate]].
```
struct S {
  S() {}
  int num;
  std::string text;
};

void foo(S s1 [[indeterminate]], S s2)
{
    bar(s1.num); // undefined behavior
    bar(s2.num); // erroneous behavior
}
```
At the point of writing (January 2025), no compiler provides support for erroneous behaviour.

## Conclusion
C++26 introduces erroneous behaviour in order to give well-defined, but incorrect behaviour for reading uninitialized values. Soon, compilers will be recommended to diagnose every occurrence of reads of uninitialized variables and function parameters.

Also, if something is not initialized at a given moment on purpose, you can mark it with the [[indeterminate]] attribute following the don’t pay for what you don’t need principle.

This new behaviour is a nice step forward in terms of C++’s safety.

--------------------
--------------------
--------------------
--------------------
--------------------
### 🧠 What is Erroneous Behavior in C++?

**Erroneous behavior** generally means your C++ program is doing something **unexpected**, **wrong**, or **undefined**, such as:

* Accessing out-of-bounds memory
* Using uninitialized variables
* Dereferencing a null pointer
* Double-freeing memory
* Dividing by zero
* Violating type rules
* Causing undefined behavior (UB)

---

### ⚠️ Example of Erroneous Behavior

```cpp
#include <iostream>

int main() {
    int x;  // uninitialized variable
    std::cout << "x = " << x << std::endl;  // undefined behavior
    return 0;
}
```

📌 **Problem**: Variable `x` is **declared but not initialized**. Reading its value is undefined behavior — the value is garbage, and different compilers or runs may show different results.

---

### ⚡ Real Example: Out-of-Bounds Access

```cpp
#include <iostream>

int main() {
    int arr[3] = {1, 2, 3};
    std::cout << arr[5] << std::endl;  // ERROR: out-of-bounds access
    return 0;
}
```

📌 **Problem**: You are reading memory past the array — may work "sometimes" (by luck) but is **not guaranteed**. It can cause:

* Crashes
* Security vulnerabilities
* Silent data corruption

---

### ❓ Why You Need to Handle Erroneous Behavior

1. ✅ **Reliability**: Prevent crashes and unpredictable behavior.
2. 🔐 **Security**: Prevent buffer overflows or memory exploits.
3. 🔍 **Debugging**: Easier to debug if you catch issues early.
4. 📦 **Portability**: Code behaves the same across compilers/platforms.

---

### ✅ How to Avoid or Solve It

#### 1. **Initialize variables**

```cpp
int x = 0;  // always initialize
```

#### 2. **Bounds checking**

```cpp
if (index >= 0 && index < size) {
    std::cout << arr[index];
}
```

#### 3. **Use modern C++ containers**

```cpp
#include <vector>
std::vector<int> vec = {1, 2, 3};
vec.at(5);  // throws std::out_of_range exception instead of undefined behavior
```

#### 4. **Smart pointers instead of raw pointers**

```cpp
#include <memory>
std::shared_ptr<int> ptr = std::make_shared<int>(5);
// no manual delete, less chance of dangling pointer
```

#### 5. **Static analysis and tools**

Use tools like:

* **Valgrind**
* **AddressSanitizer (`-fsanitize=address`)**
* **Clang-Tidy / cppcheck**

---

### 💡 Summary

| Concept               | What It Solves                       |
| --------------------- | ------------------------------------ |
| Initialize Variables  | Prevents undefined reads             |
| Bounds Checking       | Prevents memory corruption           |
| Use `std::vector::at` | Detects out-of-bounds at runtime     |
| Smart Pointers        | Prevents memory leaks & invalid free |
| Compiler Sanitizers   | Helps catch bugs early               |

--------
--------
--------
--------


اگر این روزها به‌طور تصادفی در یکی از سخنرانی‌های یک کنفرانس C++ شرکت کنید، احتمال زیادی وجود دارد که سخنران حداقل چند بار به موضوع **ایمنی (Safety)** اشاره کند. این احتمالاً چیز بدی نیست. کمیته استاندارد و جامعه برنامه‌نویسان باید به بهبود وضعیت ایمنی و همچنین اعتبار زبان C++ فکر کنند.

اگر اخبار این حوزه را دنبال کرده باشید، احتمالاً می‌دانید که افراد دیدگاه‌های متفاوتی نسبت به ایمنی دارند. تقریباً همه آن را مهم می‌دانند، اما هرکسی می‌خواهد به روش خودش این مشکل را حل کند.

یکی از منابع اصلی مشکلات، **بروز رفتار تعریف‌نشده (Undefined Behavior)** است. این نوع رفتارها هم ایمنی و هم پایداری نرم‌افزار را تحت تأثیر قرار می‌دهند. یادم می‌آید چند سال پیش که روی سرویس‌هایی کار می‌کردم که باید رشد ۱۰ برابری را پشتیبانی می‌کردند، یکی از نکات کلیدی این بود که تا جای ممکن رفتارهای تعریف‌نشده را حذف کنیم. یکی از موارد مهم برای ما حذف **متغیرهای مقداردهی‌نشده** بود که اغلب باعث کرش کردن سرویس‌ها می‌شدند.

خوشبختانه با پیشنهاد **P2795R5** توسط *Thomas Köppe*، خواندن متغیرهای مقداردهی‌نشده دیگر رفتار تعریف‌نشده نخواهد بود — از نسخه‌ی **++C26** به بعد. به جای آن، این نوع رفتارها حالا با یک نوع جدید به نام **رفتار نادرست (Erroneous Behavior)** جایگزین می‌شوند.

مزیت بزرگ این رفتار جدید این است که فقط با **کامپایل مجدد کد موجود** کار می‌کند. یعنی خودش مشخص می‌کند کجا فراموش کرده‌اید متغیر را مقداردهی کنید. دیگر لازم نیست به‌طور سیستماتیک همه کدها را بررسی کرده و مثلاً همه چیز را با `auto` تعریف کنید تا مطمئن شوید همه متغیرها مقدار اولیه دارند — کاری که احتمالاً هیچ‌وقت انجامش نمی‌دادید.

اما این رفتار جدید که در سایت مرجع C++ حتی در کنار رفتارهای تعریف‌نشده لیست شده، دقیقاً چیست؟ این رفتاری است **به‌درستی تعریف‌شده ولی نادرست** که توصیه می‌شود کامپایلرها آن را تشخیص دهند. آیا فقط توصیه شدن کافی است؟ با توجه به تمرکز روزافزون روی ایمنی، می‌توان مطمئن بود که هر کامپایلری که نتواند این رفتار نادرست را تشخیص دهد، خیلی زود کنار گذاشته خواهد شد.

برخی از کامپایلرها همین حالا هم قادرند متغیرهای مقداردهی‌نشده را تشخیص دهند — چیزی که در حال حاضر هنوز رفتار تعریف‌نشده محسوب می‌شود. برای مثال، `clang` و `gcc` با گزینه‌ی `-ftrivial-auto-var-init=zero` متغیرهای خودکار را به‌صورت پیش‌فرض با صفر مقداردهی می‌کنند. یعنی تکنیک شناسایی این متغیرها همین حالا هم وجود دارد. چیزی که باعث شده این روش عملی نباشد این است که نمی‌فهمید کدام متغیر را مقداردهی نکرده‌اید.

به جای مقداردهی پیش‌فرض، در **رفتار نادرست (Erroneous Behavior)**، شیء مقداردهی‌نشده با یک مقدار **وابسته به پیاده‌سازی** مقداردهی می‌شود. خواندن چنین مقداری یک **خطای مفهومی** محسوب می‌شود که کامپایلرها توصیه می‌شود آن را با هشدار، خطای زمان اجرا، و... تشخیص دهند.

مثال:

```cpp
void foo() {
  int d;  // d دارای مقدار نادرست است
  bar(d); // این یک رفتار نادرست است!
}
```

در مثال بالا، بهتر است `int d;` در همان زمان کامپایل هشدار دهد. اگر هشدار نادیده گرفته شود، `bar(d);` در زمان اجرا اثری خواهد داشت، ولی برخلاف رفتار تعریف‌نشده، این رفتار **قابل پیش‌بینی** خواهد بود.

قابل ذکر است که **رفتار تعریف‌نشده** و **مقادیر نادرست** در **عبارات ثابت (constexpr)** ممکن نیستند. به‌عبارت دیگر، `constexpr` از این مشکلات جلوگیری می‌کند.

حالا سؤال پیش می‌آید که اگر نمی‌خواهید شیء را فوراً مقداردهی کنید، چه؟ آیا می‌توانید بدون دریافت اخطار، مقداردهی را به تعویق بیندازید؟ بله! فقط باید **به‌صورت صریح** این را اعلام کنید. نمی‌توانید به‌طور تصادفی مقداردهی را فراموش کنید. باید آن متغیر را با ویژگی جدید C++26 یعنی `[[indeterminate]]` علامت‌گذاری کنید:

```cpp
void foo() {
  int d [[indeterminate]];  // d دارای مقدار نامعین است
  bar(d); // این رفتار تعریف‌نشده است!
}
```

در این مثال، `d` دیگر مقدار نادرست ندارد. حالا مقدار آن صرفاً **نامعین (indeterminate)** است. ولی اگر بعداً بدون مقداردهی از آن استفاده شود، **رفتار تعریف‌نشده** خواهیم داشت.

تا اینجا فقط درباره‌ی متغیرهایی با **طول عمر خودکار (automatic storage duration)** صحبت کردیم. ولی این تنها راه داشتن متغیر مقداردهی‌نشده نیست. حتی شاید رایج‌ترین راه هم نباشد. به **طول عمر پویا (dynamic)** و **اشاره‌گرها** فکر کنید! همچنین اگر هر عضوی در یک شیء مقداردهی نشود، کل شیء دارای مقدار نادرست یا نامعین خواهد بود.

مثال:

```cpp
struct S {
  S() {}
  int num;
  std::string text;
};

int main() {
  [[indeterminate]] S s1; // مقدار نامعین
  std::cout << s1.num << '\n'; // رفتار تعریف‌نشده چون s1.num مقدار ندارد

  S s2;
  std::cout << s2.num << '\n'; // رفتار نادرست، چون مقدار دارد ولی ناصحیح است
}
```

نه‌تنها متغیرها، بلکه **پارامترهای تابع** هم می‌توانند `[[indeterminate]]` باشند:

```cpp
struct S {
  S() {}
  int num;
  std::string text;
};

void foo(S s1 [[indeterminate]], S s2)
{
    bar(s1.num); // رفتار تعریف‌نشده
    bar(s2.num); // رفتار نادرست
}
```

در زمان نوشتن این متن (ژانویه ۲۰۲۵)، هیچ کامپایلری از **رفتار نادرست (erroneous behavior)** پشتیبانی نمی‌کند.

### نتیجه‌گیری

++C26 مفهومی به نام **رفتار نادرست** را معرفی می‌کند تا خواندن مقادیر مقداردهی‌نشده را به رفتاری قابل پیش‌بینی ولی نادرست تبدیل کند. به‌زودی، از کامپایلرها انتظار می‌رود که هر مورد استفاده از متغیر یا پارامتر مقداردهی‌نشده را تشخیص دهند.

همچنین، اگر عمداً نمی‌خواهید متغیری را در لحظه‌ای خاص مقداردهی کنید، می‌توانید آن را با `[[indeterminate]]` علامت‌گذاری کنید — که مطابق اصل "برای چیزی که استفاده نمی‌کنی هزینه نده" است.

این ویژگی جدید، گامی مفید به سمت بهبود **ایمنی در زبان C++** است.


------
------
------


### خلاصه:

در نسخه آینده C++26، مفهومی جدید به نام **رفتار نادرست (Erroneous Behavior)** معرفی می‌شود. هدف آن افزایش **ایمنی زبان C++** است.

تا پیش از این، **خواندن متغیرهای مقداردهی‌نشده** باعث **رفتار تعریف‌نشده (Undefined Behavior)** می‌شد که می‌توانست مشکلات جدی و غیرقابل پیش‌بینی ایجاد کند. اما از C++26، چنین خواندنی به جای UB، رفتار نادرست خواهد بود:

* **رفتار نادرست** یعنی برنامه به شکلی نادرست ولی **قابل پیش‌بینی** اجرا می‌شود.
* **کامپایلرها توصیه می‌شوند** این موارد را شناسایی کرده و هشدار بدهند.

اگر بخواهید عمداً متغیری را بدون مقدار اولیه تعریف کنید، باید از ویژگی جدید `[[indeterminate]]` استفاده کنید.
در این صورت، اگر آن متغیر را بدون مقداردهی استفاده کنید، **رفتار همچنان تعریف‌نشده** خواهد بود.

این تغییر، گامی مهم برای بهبود ایمنی، قابل‌اعتماد بودن و اشکال‌زدایی آسان‌تر در کدهای ++C است.

---

اگر بخواهی خلاصه‌تر هم بگم:
**C++26 می‌خواهد خواندن متغیرهای مقداردهی‌نشده را از حالت خطرناک و غیرقابل‌پیش‌بینی (UB)، به حالت قابل‌پیش‌بینی ولی نادرست (Erroneous Behavior) تبدیل کند، تا ایمنی کد بیشتر شود.**
