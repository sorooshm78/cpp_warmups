Sure! Let's walk through how to **add `clang-format` integration into a CMake-based C++ project** step by step. This means you'll be able to format your code using `clang-format` via CMake commands.

---

## 🛠️ What is `clang-format`?

[`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) is a tool to format C, C++, Java, JavaScript, and other code according to style guidelines. You use it to ensure your code is automatically formatted the same way across your project.

---

## 🎯 Goal

We want to:

1. Add a `.clang-format` config file.
2. Use a custom CMake target, e.g., `make format`, to run `clang-format` on your source files.
3. Keep everything simple and automated.

---

## ✅ Step-by-step Example

### 🔹 Step 1: Create a minimal C++ project

Let’s say your folder structure is:

```
my_project/
│
├── CMakeLists.txt
├── main.cpp
└── .clang-format
```

### 🔹 Step 2: Add `.clang-format` file

Create a `.clang-format` file in your root directory. Here's a simple one:

```yaml
BasedOnStyle: Google
IndentWidth: 4
ColumnLimit: 100
```

This tells `clang-format` to start from Google style and tweak indentation and column limit.

---

### 🔹 Step 3: Basic CMakeLists.txt

Here’s a simple `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyApp main.cpp)
```

---

### 🔹 Step 4: Add a `clang-format` CMake target

Now, add a custom target to run `clang-format`. Update your `CMakeLists.txt` like this:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyApp main.cpp)

# List of source files to format
set(FORMAT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Add a custom target for clang-format
find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE}
        -i
        -style=file
        ${FORMAT_SOURCES}
        COMMENT "Running clang-format on source files"
    )
else()
    message(WARNING "clang-format not found!")
endif()
```

### What this does:

* `find_program` looks for `clang-format` in your system.
* If found, it adds a custom target called `format`.
* Running `make format` will apply `clang-format` to listed source files (`main.cpp`).

---

### 🔹 Step 5: Build & Format

In your terminal:

```bash
mkdir build
cd build
cmake ..
make format
```

✅ `main.cpp` is now formatted according to your `.clang-format` file.

---

## 🧩 Optional: Format all `.cpp` and `.h` files

Instead of manually listing all source files, you can do:

```cmake
file(GLOB_RECURSE FORMAT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
```

Replace the earlier `set(FORMAT_SOURCES ...)` line with this one.

---

## ✅ Final Thoughts

You now have:

* A `.clang-format` config file.
* CMake automatically formatting your code with `make format`.
* A scalable setup for large projects.

