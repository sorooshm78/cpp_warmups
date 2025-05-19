Absolutely! Let's go **step by step** and explain **`add_custom_target()`** in CMake in detail — what it does, how to use it, and why it’s useful.

---

## ✅ What is `add_custom_target()` in CMake?

`add_custom_target()` lets you **define a new custom build step (a new "make" target)**. Unlike regular targets (like `add_executable()`), it doesn't produce an actual binary or file — it just runs commands you specify.

### 📦 Use Case:

You can use `add_custom_target()` to:

* Run formatting tools (like `clang-format`)
* Run static analysis
* Run tests
* Clean temporary files
* Auto-generate code or docs

---

## 📘 Syntax

```cmake
add_custom_target(<name> [ALL]
                  [COMMAND command1 [ARGS] [args1...]]
                  [COMMAND command2 ...]
                  [WORKING_DIRECTORY dir]
                  [COMMENT "message"]
                  [DEPENDS targets...])
```

### Key Parts:

* `<name>` – name of the target (you’ll run it with `make <name>`)
* `COMMAND` – what to run
* `COMMENT` – message shown when the target runs
* `DEPENDS` – if this depends on other targets/files

---

## 🧱 Step-by-Step Example: Creating a Format Target

Let’s say you want a target that runs `clang-format` on your files.

### 🔹 Step 1: CMake setup

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

add_executable(MyApp main.cpp)
```

### 🔹 Step 2: Add a `format` target using `add_custom_target()`

```cmake
find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        COMMENT "Formatting source files with clang-format"
    )
else()
    message(WARNING "clang-format not found!")
endif()
```

### 🔍 What this does:

* It **adds a new target** called `format`
* When you run `make format`, it runs:

  ```bash
  clang-format -i -style=file main.cpp
  ```
* `COMMENT` just prints a message during build

---

## 💡 Why use `add_custom_target()`?

### ✅ Pros:

* **Non-build actions** (formatting, code gen, tests)
* **Easy automation** of common dev tasks
* **Cross-platform** (CMake handles different OSes)

### 🛠️ Example Use Cases:

#### 🔹 Run all tests:

```cmake
add_custom_target(run_tests
    COMMAND ./run_all_tests
    COMMENT "Running all unit tests"
)
```

#### 🔹 Delete cache or temp files:

```cmake
add_custom_target(clean_temp
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_SOURCE_DIR}/temp
    COMMENT "Cleaning temporary files"
)
```

#### 🔹 Generate documentation:

```cmake
add_custom_target(doc
    COMMAND doxygen Doxyfile
    COMMENT "Generating documentation"
)
```

---

## 🔁 Combine With `DEPENDS`

Sometimes, you want to **trigger this target only after another is built**. Use `DEPENDS`:

```cmake
add_custom_target(build_and_format
    COMMAND ${CLANG_FORMAT_EXE} -i main.cpp
    DEPENDS MyApp
    COMMENT "Format after build"
)
```

Now `make build_and_format` will:

1. Build `MyApp`
2. Format `main.cpp`

---

## 🎓 Summary

| Feature               | What it does                                      |
| --------------------- | ------------------------------------------------- |
| `add_custom_target()` | Adds a named target to run custom commands        |
| `COMMAND`             | What gets executed                                |
| `COMMENT`             | Message shown in terminal                         |
| `DEPENDS`             | Run this only after listed targets/files are done |
| Useful for            | Formatting, testing, codegen, cleanup, automation |

---

---------------------
---------------------
---------------------
---------------------
---------------------

Great question! Let's break it down **simply and clearly**.

---

## 🔧 How to Use `add_custom_target()` in CMake — **Step by Step**

---

### ✅ Step 1: Basic Setup

You must already have a basic CMake project.

Let's say your project has this:

```bash
project/
├── CMakeLists.txt
├── main.cpp
```

`main.cpp`:

```cpp
#include <iostream>
int main() {
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
```

`CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyApp)
add_executable(MyApp main.cpp)
```

---

### ✅ Step 2: Add `add_custom_target()` to run something

Now we add a **custom target** to run `clang-format`, or even a simple `echo`.

#### Example 1: Hello target (basic demo)

Add this to your `CMakeLists.txt`:

```cmake
add_custom_target(say_hello
    COMMAND echo "Hello from custom target!"
    COMMENT "Running say_hello target"
)
```

Now from your build directory:

```bash
cmake ..
make say_hello
```

**Output:**

```bash
Running say_hello target
Hello from custom target!
```

---

### ✅ Step 3: Add a `clang-format` target using `add_custom_target()`

Assume you have `clang-format` installed.

#### Add this to `CMakeLists.txt`:

```cmake
# Find clang-format on your system
find_program(CLANG_FORMAT_EXE NAMES clang-format)

# Check if found
if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        COMMENT "Formatting code using clang-format"
    )
else()
    message(WARNING "clang-format not found!")
endif()
```

---

### ✅ Step 4: Run the format target

From terminal:

```bash
make format
```

It will format `main.cpp` using your `.clang-format` rules (if present), and print:

```
Formatting code using clang-format
```

---

## 🧠 Summary Table

| Component                     | Purpose                                                |
| ----------------------------- | ------------------------------------------------------ |
| `add_custom_target(name ...)` | Create a new "make" target (not linked to a file)      |
| `COMMAND`                     | Shell command to run                                   |
| `COMMENT`                     | Optional message shown during build                    |
| `DEPENDS`                     | List of other targets or files to depend on (optional) |

---

### 🔁 Optional: Use `file(GLOB)` to auto-include all `.cpp` files

```cmake
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${SOURCES}
    COMMENT "Formatting all .cpp files"
)
```

---

## 📌 Final Notes

* `add_custom_target()` is great for **extra developer commands**, like testing, linting, or code generation.
* It runs only when **explicitly invoked**, like `make format`, unless you use `ALL` to make it run always.

---

------------------------
------------------------
------------------------
------------------------
------------------------

Great question!

If you're using **CMake on platforms where `make` isn't used** (like **Windows with MSBuild**, or **Ninja**, or if you want to use **CMake itself directly**), you can still run custom targets like those defined with `add_custom_target()`.

Let’s go over **how to use custom targets without `make`**, depending on your **CMake generator**.

---

## ✅ 1. Using `cmake --build` Instead of `make`

No matter what build system you're using (Makefiles, Ninja, Visual Studio, etc.), **the cross-platform way** to build and run custom targets is:

```bash
cmake --build . --target <your_target>
```

So, if you have a target like:

```cmake
add_custom_target(format
    COMMAND clang-format -i main.cpp
    COMMENT "Formatting code"
)
```

You can run it from the **build directory** with:

```bash
cmake --build . --target format
```

### 💡 Tip:

This works on:

* Linux/macOS with Make/Ninja
* Windows with MSBuild or Visual Studio

---

## ✅ 2. Example Commands (Per Platform)

| Platform                | Command to run custom target                                                            |
| ----------------------- | --------------------------------------------------------------------------------------- |
| Linux/macOS (Make)      | `make format` or `cmake --build . --target format`                                      |
| Linux/macOS (Ninja)     | `ninja format` or `cmake --build . --target format`                                     |
| Windows (MSBuild)       | `cmake --build . --target format`                                                       |
| Windows (Visual Studio) | Build target `format` from the IDE or use `cmake --build . --target format` in terminal |

---

## ✅ 3. Set Generator (Optional)

When you run CMake, you can choose a generator (build system):

```bash
cmake -G "Ninja" ..
```

Then run:

```bash
cmake --build . --target format
```

---

## 🧠 Summary

* `add_custom_target()` creates a named build task like `format`, `clean`, `doc`, etc.
* Instead of `make format`, use this **universal CMake command**:

  ```bash
  cmake --build . --target format
  ```
* It works on **all platforms and build systems** supported by CMake.

---


