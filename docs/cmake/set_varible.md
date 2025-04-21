Creating variables in **CMake** is straightforward. CMake uses the `set()` command to define variables. Let's walk through it with clear **examples**.

---

### 🔹 Basic Syntax

```cmake
set(<variable_name> <value>...)
```

- `<variable_name>`: The name of your variable.
- `<value>`: The value(s) you want to assign to it.

---

### 🧪 Example 1: Simple string variable

```cmake
set(MY_NAME "ChatGPT")
message(STATUS "My name is ${MY_NAME}")
```

**Output during CMake configuration:**
```
-- My name is ChatGPT
```

---

### 🧪 Example 2: Integer variable (used for comparison)

```cmake
set(VERSION_NUMBER 5)
if(VERSION_NUMBER GREATER 3)
    message(STATUS "Version is greater than 3")
endif()
```

---

### 🧪 Example 3: List variable

```cmake
set(MY_LIST a b c d)
message(STATUS "List: ${MY_LIST}")
```

Or split by semicolon:

```cmake
set(MY_LIST "apple;banana;cherry")
foreach(item IN LISTS MY_LIST)
    message(STATUS "Fruit: ${item}")
endforeach()
```

---

### 🧪 Example 4: Using variables to define include directories

```cmake
set(INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")
include_directories(${INCLUDE_DIRS})
```

---

### 🧪 Example 5: Cache variable (used to expose options in GUIs like `ccmake` or `cmake-gui`)

```cmake
set(MY_OPTION "ON" CACHE BOOL "Enable my custom option")
```

---

### 🧪 Example 6: Environment variable

You can read environment variables with:

```cmake
message(STATUS "PATH is $ENV{PATH}")
```

You can also set one for subprocesses:

```cmake
set(ENV{MY_ENV_VAR} "HelloFromCMake")
```

---


-------------------------------------
-------------------------------------
-------------------------------------
-------------------------------------
-------------------------------------


Sure! Let's break down **CMake variables** with examples and a mini-tutorial.

---

### 🧠 **What is a variable in CMake?**
In CMake, **variables** are used to store values like file paths, options, project names, build types, etc. You can then use them throughout your `CMakeLists.txt` script.

They are defined using:

```cmake
set(<variable_name> <value>)
```

And accessed using `${<variable_name>}`.

---

### 📘 **Basic Example 1: Simple Variable**

```cmake
set(MY_NAME "Alice")
message("My name is: ${MY_NAME}")
```

🧾 **Output:**
```
My name is: Alice
```

---

### 📘 **Example 2: Using Variables in Paths**

```cmake
set(SRC_DIR "src")
add_executable(MyApp ${SRC_DIR}/main.cpp)
```

🧾 **Explanation:**
- You define a source directory.
- Then you use that variable to refer to the file inside `src/main.cpp`.

---

### 📘 **Example 3: Conditional Variable Usage**

```cmake
set(USE_MATH_LIB ON)

if(USE_MATH_LIB)
    message("Math library will be used.")
else()
    message("Math library will NOT be used.")
endif()
```

🧾 **Output (if ON):**
```
Math library will be used.
```

---

### 📘 **Example 4: Cache Variables (for User Input)**

```cmake
set(BUILD_TESTS ON CACHE BOOL "Build tests")
```

🧾 **Explanation:**
This allows users to control the value using the command line:

```bash
cmake -DBUILD_TESTS=OFF .
```

So this is great for build configuration flags.

---

### ⚙️ **Useful Built-in Variables in CMake**

| Variable              | Description                                     |
|-----------------------|-------------------------------------------------|
| `CMAKE_SOURCE_DIR`    | Root dir of the source code                     |
| `CMAKE_BINARY_DIR`    | Directory where build files go                 |
| `CMAKE_C_COMPILER`    | Path to the C compiler                         |
| `CMAKE_BUILD_TYPE`    | Build type: Debug, Release, etc.               |
| `CMAKE_INSTALL_PREFIX`| Where `make install` will place files          |

🧪 **Example:**

```cmake
message("Build type: ${CMAKE_BUILD_TYPE}")
```

---

### 🎯 Summary

- Define variables with `set()`.
- Use them with `${}`.
- They can store strings, paths, options, and more.
- Cache variables allow user input.
- There are many built-in variables to control your build.

---

---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------

Great! Let’s dive into **CMake built-in variables** — what they are, how to use them, and see some real examples and expected output.

---

## 🧠 What Are Built-in Variables in CMake?

Built-in variables are **predefined by CMake** and help control how your project is built, compiled, installed, etc.

You can **read** and sometimes **set** these variables in your `CMakeLists.txt` file.

---

## 🔑 Common Built-in Variables with Examples

---

### 1. `CMAKE_SOURCE_DIR`

📌 **The top-level directory of your project (where your main `CMakeLists.txt` is).**

```cmake
message("Source directory: ${CMAKE_SOURCE_DIR}")
```

🖨️ Output:
```
Source directory: /home/user/my_project
```

---

### 2. `CMAKE_BINARY_DIR`

📌 **Where the build files are generated (your build directory).**

```cmake
message("Binary directory: ${CMAKE_BINARY_DIR}")
```

🖨️ Output:
```
Binary directory: /home/user/my_project/build
```

---

### 3. `CMAKE_CURRENT_SOURCE_DIR`

📌 **The directory containing the current `CMakeLists.txt`.**

Useful in subdirectories.

```cmake
message("Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
```

🖨️ Output:
```
Current source dir: /home/user/my_project/src
```

---

### 4. `CMAKE_CURRENT_BINARY_DIR`

📌 **The build directory for the current source directory.**

```cmake
message("Current binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
```

🖨️ Output:
```
Current binary dir: /home/user/my_project/build/src
```

---

### 5. `CMAKE_INSTALL_PREFIX`

📌 **Where files will be installed by `make install`.**

Default: `/usr/local` on Linux.

```cmake
message("Install location: ${CMAKE_INSTALL_PREFIX}")
```

🖨️ Output:
```
Install location: /usr/local
```

You can change it:
```bash
cmake -DCMAKE_INSTALL_PREFIX=/opt/myapp ..
```

---

### 6. `CMAKE_CXX_STANDARD`

📌 **Specifies the C++ version (e.g., 11, 14, 17, 20, 23)**

```cmake
set(CMAKE_CXX_STANDARD 17)
message("Using C++ standard: ${CMAKE_CXX_STANDARD}")
```

🖨️ Output:
```
Using C++ standard: 17
```

---

### 7. `CMAKE_BUILD_TYPE`

📌 **Sets the build type:**
- `Debug`
- `Release`
- `RelWithDebInfo`
- `MinSizeRel`

```cmake
message("Build type: ${CMAKE_BUILD_TYPE}")
```

🖨️ Output:
```
Build type: Debug
```

Set it with:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

---

### 8. `PROJECT_NAME`

📌 **The name given in the `project()` command**

```cmake
project(MyApp)
message("Project name: ${PROJECT_NAME}")
```

🖨️ Output:
```
Project name: MyApp
```

---

## ✅ Summary Table

| Variable                  | Description                                     |
|---------------------------|-------------------------------------------------|
| `CMAKE_SOURCE_DIR`        | Top-level source dir                           |
| `CMAKE_BINARY_DIR`        | Top-level build dir                            |
| `CMAKE_CURRENT_SOURCE_DIR`| Current dir of `CMakeLists.txt`                |
| `CMAKE_CURRENT_BINARY_DIR`| Build dir for that source dir                  |
| `CMAKE_INSTALL_PREFIX`    | Install path                                   |
| `CMAKE_CXX_STANDARD`      | C++ version                                    |
| `CMAKE_BUILD_TYPE`        | Debug, Release, etc.                           |
| `PROJECT_NAME`            | Name of the project                            |

---


---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------






---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------

CMAKE_CXX_EXTENSIONS is a CMake variable that controls whether CMake enables compiler-specific C++ language extensions. It's a boolean variable that can be set to `ON` or `OFF`.

When `CMAKE_CXX_EXTENSIONS` is `ON` (the default), CMake allows the compiler to use its specific extensions to the C++ language. This often includes features that are not part of the standard C++ specification.  These extensions can be useful for taking advantage of specific hardware features, optimizing performance on a particular platform, or using language features that are not yet standardized. However, code relying on these extensions might not be portable to other compilers or platforms that do not support them.

When `CMAKE_CXX_EXTENSIONS` is `OFF`, CMake disables compiler extensions and enforces a stricter adherence to the C++ standard. This can improve code portability, as the compiler is less likely to accept non-standard constructs. However, it might limit the ability to use compiler-specific optimizations or features.

Here's a breakdown with an example:

**Scenario:** You have a piece of code that uses a compiler-specific extension (e.g., a GCC-specific attribute) and you want to ensure your code is portable or you want to enforce standard C++ compliance.

**Example CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

# By default, CMAKE_CXX_EXTENSIONS is ON. Let's turn it off.
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(my_executable main.cpp)
```

**Example C++ code (main.cpp):**

```cpp
#include <iostream>

// This attribute is a GCC-specific extension that tells the compiler
// that this function should never return.  If it does, something is wrong.
[[gnu::noreturn]]
void my_function() {
  std::cout << "This function is about to exit." << std::endl;
  exit(1); // Simulate a return by calling exit.  Without calling exit, the compiler may warn.
  //return;  // This line would be an error
}

int main() {
  my_function();
  std::cout << "This line should not be printed." << std::endl;
  return 0;
}
```

**Explanation:**

1.  **`set(CMAKE_CXX_EXTENSIONS OFF)`:**  This line in the `CMakeLists.txt` file is crucial. It tells CMake to disable C++ extensions for the C++ compiler.

2.  **`[[gnu::noreturn]]`:** The C++ code uses the GCC-specific `[[gnu::noreturn]]` attribute.  This attribute tells the compiler that the function `my_function()` is not supposed to return.

**Behavior:**

*   **When `CMAKE_CXX_EXTENSIONS` is `ON` (or the line `set(CMAKE_CXX_EXTENSIONS OFF)` is removed):** The code will likely compile and run without errors or warnings (using GCC).  The compiler understands the `[[gnu::noreturn]]` attribute and acts accordingly.

*   **When `CMAKE_CXX_EXTENSIONS` is `OFF`:** When you try to build with `CMAKE_CXX_EXTENSIONS` set to `OFF` using GCC, the compiler will likely issue a warning or an error that it does not recognize `[[gnu::noreturn]]`. This is because the compiler is being forced to adhere to a stricter standard and the extension is not part of the standard.  The exact error message might depend on the compiler version and flags.

**Why is this useful?**

*   **Portability:**  If you intend to compile your code with multiple compilers (e.g., GCC, Clang, MSVC), setting `CMAKE_CXX_EXTENSIONS` to `OFF` can help you identify and remove compiler-specific code that would prevent your code from building on other platforms.

*   **Enforcing Standards:** If you want to ensure that your code adheres to a specific C++ standard (e.g., C++11, C++14, C++17), disabling extensions can help you catch non-standard code constructs.

*   **Debugging:** Sometimes, compiler extensions can mask underlying problems in your code. By disabling them, you might uncover hidden bugs.

**In Summary:**

`CMAKE_CXX_EXTENSIONS` is a powerful tool for managing compiler extensions in your CMake projects. By controlling whether extensions are enabled or disabled, you can influence the portability, standard compliance, and debugging of your code. Use `OFF` when you want stricter standard adherence or portability, and `ON` (the default) when you want to take advantage of compiler-specific features.


---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------
---------------------------------------


In **CMake**, the `option()` command is used to create **boolean configuration options** that users can enable or disable when configuring a project. These options are typically shown in GUI tools like **ccmake** or **cmake-gui**, or set via the command line with `-D`.

---

## 🔑 Syntax of `option()`
```cmake
option(<option_variable> "Help text describing the option" <initial_value>)
```

- `<option_variable>`: The name of the variable.
- `"Help text"`: Description shown in GUIs.
- `<initial_value>`: Default value, either `ON` or `OFF`.

---

## 🧪 Example 1: Simple option
```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MyApp)

option(USE_MATH "Use math functions" ON)

if(USE_MATH)
    message(STATUS "Math support enabled.")
else()
    message(STATUS "Math support disabled.")
endif()
```

### 🔧 Command to configure:
```bash
cmake -DUSE_MATH=OFF ..
```

### 🖥️ Output:
```
-- Math support disabled.
```

---

## 🧪 Example 2: Control optional source files
```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(FeatureApp)

option(ENABLE_LOGGING "Enable logging feature" OFF)

set(SOURCES main.cpp)

if(ENABLE_LOGGING)
    list(APPEND SOURCES logging.cpp)
endif()

add_executable(FeatureApp ${SOURCES})
```

### 🔧 Usage:
```bash
cmake -DENABLE_LOGGING=ON ..
```

### 🖥️ Effect:
- With `ENABLE_LOGGING=ON`, `logging.cpp` is compiled.
- With `ENABLE_LOGGING=OFF`, it's not.

---

## 🧪 Example 3: Multiple options
```cmake
cmake_minimum_required(VERSION 3.10)
project(ConfigurableApp)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(WITH_GUI "Enable GUI support" OFF)
option(WITH_NETWORKING "Enable networking" ON)

if(WITH_GUI)
    message(STATUS "GUI will be built.")
else()
    message(STATUS "GUI is disabled.")
endif()

if(WITH_NETWORKING)
    message(STATUS "Networking enabled.")
endif()
```

### 🔧 Configure:
```bash
cmake -DWITH_GUI=ON -DWITH_NETWORKING=OFF ..
```

### 🖥️ Output:
```
-- GUI will be built.
-- Networking disabled.
```

---

## 🛠️ Example 4: Use in `if()` conditions
```cmake
option(ENABLE_FEATURE "Enable a special feature" OFF)

if(ENABLE_FEATURE)
    add_definitions(-DFEATURE_ENABLED)
endif()
```

### 🔧 Use Case:
This will define `FEATURE_ENABLED` in your C++ code:
```cpp
#ifdef FEATURE_ENABLED
// Feature-specific code
#endif
```

---

## ✅ Tips

| Tip | Explanation |
|-----|-------------|
| `ON` / `OFF` | Case-insensitive (`on`, `ON`, `1`, `TRUE` are valid) |
| Default value | Used only the first time CMake runs. Cache remembers it. |
| Change value | Clear cache or use `-D<OPTION>=...` to override. |
| GUI tools | `option()` descriptions appear in cmake GUIs |

---
