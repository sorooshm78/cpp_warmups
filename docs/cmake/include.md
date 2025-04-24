The `include()` command in **CMake** is used to **load and run another CMake script** during the configuration process. It’s like saying “run this other file here as if its content was written at this point.”

---

### 🔹 Why use `include()`?

You use `include()` to:
1. **Reuse code** – Avoid duplicating setup logic in multiple `CMakeLists.txt`.
2. **Organize CMake files** – Split large configurations into smaller, manageable chunks.
3. **Use modules** – Load built-in or custom CMake modules.
4. **Share logic between projects** – Like handling compiler flags, custom functions/macros, or setting up tools.

---

### 🔹 Syntax:
```cmake
include(<file|module> [OPTIONAL] [RESULT_VARIABLE <var>] [NO_POLICY_SCOPE])
```

---

### 🔹 Simple Example:

#### Directory structure:
```
MyProject/
│
├── CMakeLists.txt
└── cmake/
    └── MyHelpers.cmake
```

#### `cmake/MyHelpers.cmake`
```cmake
message(STATUS "Included MyHelpers.cmake")
function(print_custom_message)
    message(STATUS "Hello from custom function!")
endfunction()
```

#### `CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

# Include custom script
include(cmake/MyHelpers.cmake)

# Call function defined in the included file
print_custom_message()
```

**Result when you configure:**
```
-- Included MyHelpers.cmake
-- Hello from custom function!
```

---

### 🔹 When NOT to use `include()`:
- If you're trying to **add another CMake-based subproject**, use `add_subdirectory()` instead.
- If you're linking libraries or targets, use `find_package()` or `target_link_libraries()`.

---

### 🔹 Common Problems `include()` Solves:
- Organizing settings for toolchains, platform-specific flags, or packaging into reusable files.
- Keeping main `CMakeLists.txt` clean.
- Avoiding code duplication across multiple projects.

---

Want help structuring your own CMake project or splitting it into includes? Happy to help with a real example if you’ve got one.