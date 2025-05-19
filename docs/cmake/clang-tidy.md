Absolutely! Let’s walk through **how to integrate `clang-tidy` into a CMake project** — with **step-by-step explanation**, real examples, and why it’s helpful.

---

## 🎯 Goal: Automatically check code style and bugs with `clang-tidy` using CMake

---

## ✅ What is `clang-tidy`?

`clang-tidy` is a **static analysis tool** for C/C++ code. It catches:

* Code style violations
* Bugs and undefined behavior
* Performance issues
* Modernization suggestions (e.g., replace `for` loops with range-based loops)

---

## 🧱 Project Structure Example

```
my_project/
├── main.cpp
├── CMakeLists.txt
```

---

## 🛠️ Step-by-Step Setup for `clang-tidy` in CMake

---

### 🔹 Step 1: Basic `CMakeLists.txt`

Create a simple project to compile:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
add_executable(MyApp main.cpp)
```

---

### 🔹 Step 2: Install `clang-tidy` (if not installed)

Install via:

```bash
sudo apt install clang-tidy           # Ubuntu
brew install llvm                     # macOS
choco install llvm                    # Windows
```

You can test it's available:

```bash
clang-tidy --version
```

---

### 🔹 Step 3: Enable `clang-tidy` in CMake

Add this to your `CMakeLists.txt`:

```cmake
find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
    message(WARNING "clang-tidy not found!")
endif()
```

This tells CMake:
➡️ “Use `clang-tidy` to analyze C++ source files during build.”

---

### 🔹 Step 4: Configure and Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

During compilation, you’ll see messages like:

```
clang-tidy found: /usr/bin/clang-tidy
[1/2] Running clang-tidy for main.cpp
```

---

### 🔹 Step 5: Add a `.clang-tidy` Config File (Optional, Recommended)

Create a `.clang-tidy` file in your project root:

```yaml
Checks: 'modernize-*,readability-*,performance-*'
WarningsAsErrors: '*'
HeaderFilterRegex: '.*'
```

This controls what checks are run. Without it, `clang-tidy` will use default settings.

---

## 🧠 What's Going On?

By setting:

```cmake
set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
```

You're telling CMake to automatically run `clang-tidy` **as part of the build**, **on every `.cpp` file**, just like a compiler.

It **does not stop the build** on issues by default — it just prints warnings. If you want to treat them as errors, you can use:

```cmake
set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-warnings-as-errors=*")
```

---

## ✅ Optional: Add a Custom Target to Run Clang-Tidy Manually

If you **don’t** want to run `clang-tidy` during every build, you can use `add_custom_target` instead:

```cmake
add_custom_target(run-tidy
    COMMAND ${CLANG_TIDY_EXE} main.cpp -- -I${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-tidy manually"
)
```

Then run with:

```bash
cmake --build . --target run-tidy
```

---

## 📌 Summary

| Step                       | Description                             |
| -------------------------- | --------------------------------------- |
| 1. Add `clang-tidy`        | Install and test it's in your path      |
| 2. Modify `CMakeLists.txt` | Use `CMAKE_CXX_CLANG_TIDY` to enable it |
| 3. Configure project       | `cmake ..` and `cmake --build .`        |
| 4. Optional config         | Use `.clang-tidy` for custom checks     |
| 5. Optional manual run     | Use `add_custom_target(run-tidy ...)`   |

---

## ✅ Example CMakeLists.txt with Everything

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
    message(WARNING "clang-tidy not found!")
endif()

add_executable(MyApp main.cpp)
```

---
