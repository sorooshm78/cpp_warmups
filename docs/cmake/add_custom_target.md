
---

## 🔹 What is `add_custom_target`?

`add_custom_target(<name> [ALL] [command1 [args1...]] ...)`

It defines a new **custom build target** that runs commands when invoked, but **does not produce a binary** like `add_executable()` or `add_library()` would.

---

## 🔹 Why use `add_custom_target`?

You use it when you want to:
- Run a script or tool as part of the build.
- Generate files (docs, code, assets) manually.
- Add custom checks, formatters, or linters.
- Hook into build system without creating a "real" program.

It **solves the problem** of automating any kind of non-C++ build task **within the CMake build process**.

---

## 🔹 Basic Syntax

```cmake
add_custom_target(
  <name>
  COMMAND <your-command>
  [BYPRODUCTS <files-generated>]
  [WORKING_DIRECTORY <dir>]
  [COMMENT "Message to show"]
  [VERBATIM]
  [ALL]  # Run by default with 'make'
)
```

---

## 🔹 Simple Example: Running a Python script

Imagine you have a script that generates JSON config before the program runs.

### 📁 Project Layout
```
MyProject/
├── CMakeLists.txt
├── generate_config.py
```

### 📄 `generate_config.py`
```python
# Just writes a sample config file
with open("config.json", "w") as f:
    f.write('{"key": "value"}')
```

### 📄 `CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.15)
project(MyCustomTargetExample)

# Add a custom target to generate config
add_custom_target(GenerateConfig
  COMMAND ${CMAKE_COMMAND} -E echo "Generating config..."
  COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_SOURCE_DIR}/generate_config.py
  BYPRODUCTS ${CMAKE_BINARY_DIR}/config.json
  COMMENT "Running Python script to generate config.json"
  VERBATIM
)

# Optional: make it run before main build
add_executable(MyApp main.cpp)
add_dependencies(MyApp GenerateConfig)
```

### ✅ What this does:
- Creates a target called `GenerateConfig`.
- When you run: `cmake --build . --target GenerateConfig`, it runs the script.
- If you add `add_dependencies()`, it runs **before building `MyApp`**.

---

## 🔹 Common Use Cases

| Task | How `add_custom_target` helps |
|------|-------------------------------|
| Code generation | Runs a generator script |
| Documentation | Runs Doxygen/Sphinx |
| Formatting | Runs `clang-format`, `black`, etc. |
| Unit test reports | Runs coverage tools |
| Asset bundling | Converts assets/images before packaging |

---

## 🔹 Bonus: Make it run by default

Add `ALL`:
```cmake
add_custom_target(MyTask ALL
  COMMAND echo "This always runs when you build"
)
```

---

## 🔹 Summary

| Feature | Purpose |
|--------|---------|
| `add_custom_target()` | Adds a build step with no output file |
| `COMMAND` | What to run |
| `BYPRODUCTS` | What the command creates (helps CMake track dependencies) |
| `add_dependencies()` | Makes other targets depend on it |
| `ALL` | Run every time you call `make` or `cmake --build .` |

---
