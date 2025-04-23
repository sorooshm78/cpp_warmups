### `PROJECT_SOURCE_DIR` in CMake

`PROJECT_SOURCE_DIR` is a special CMake variable that refers to the **root directory** of your project’s source code — specifically the directory where the main `CMakeLists.txt` file is located.

### What is the Purpose of `PROJECT_SOURCE_DIR`?

This variable is automatically defined by CMake when you run the build system. It provides a way to refer to the root source directory of your project, regardless of where you build or where your `CMakeLists.txt` files are located. This helps to manage paths to files and directories relative to the source directory.

### Why is `PROJECT_SOURCE_DIR` Useful?

1. **Portability**: Using `PROJECT_SOURCE_DIR` makes your project portable because it allows you to specify paths relative to the root source directory, instead of hardcoding absolute paths that would differ between machines.
2. **Consistency**: It ensures that any file or resource you need to reference within the project is based on the project’s root source directory, preventing errors from relative path mismatches.
3. **Clarity**: It improves readability and maintainability of your CMake configuration because it makes it clear that the paths you're defining are relative to the root source directory.

### Where is `PROJECT_SOURCE_DIR` Set?

This variable is automatically set by CMake as soon as it processes the `CMakeLists.txt` file. It points to the directory that contains your main `CMakeLists.txt` file, which is considered the "root" of the project.

For example, if your project’s directory structure looks like this:

```
my_project/
├── CMakeLists.txt     # This is where PROJECT_SOURCE_DIR is based
├── src/
│   └── main.cpp
└── include/
    └── myheader.h
```

In this case:
- `PROJECT_SOURCE_DIR` will point to `my_project/`, the directory containing `CMakeLists.txt`.

### Example Usage of `PROJECT_SOURCE_DIR`

Let’s look at a practical example where we use `PROJECT_SOURCE_DIR` to include header files or source files in a project.

#### Example 1: Using `PROJECT_SOURCE_DIR` to Include Header Files

```cmake
# CMakeLists.txt at the root of the project

cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Specify where to find header files relative to the project source directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create an executable from the source files
add_executable(my_project ${PROJECT_SOURCE_DIR}/src/main.cpp)
```

In this example:
- `include_directories(${PROJECT_SOURCE_DIR}/include)` tells CMake to add the `include/` directory (relative to the root source directory) to the list of directories where it will look for header files.
- `add_executable(my_project ${PROJECT_SOURCE_DIR}/src/main.cpp)` specifies that the `main.cpp` source file, located in `src/`, should be used to create the executable.

Here, `PROJECT_SOURCE_DIR` ensures that the paths to `include/` and `src/` are always correct relative to the project's root directory.

#### Example 2: Using `PROJECT_SOURCE_DIR` for File Installation

You can also use `PROJECT_SOURCE_DIR` to reference files when setting up installation rules.

```cmake
# CMakeLists.txt at the root of the project

cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Install the header files from the include directory
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION /usr/local/include)

# Install the executable
install(TARGETS my_project DESTINATION /usr/local/bin)
```

In this example:
- `install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION /usr/local/include)` installs the contents of the `include/` directory (relative to the root source directory) into the `/usr/local/include` directory on the target system.
- `install(TARGETS my_project DESTINATION /usr/local/bin)` installs the compiled executable into `/usr/local/bin`.

### Example 3: Using `PROJECT_SOURCE_DIR` for Conditional Configuration

You might also want to conditionally add a file or directory based on the presence of a specific folder or file in your source directory.

```cmake
# CMakeLists.txt at the root of the project

cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Check if a config file exists in the source directory and include it
if(EXISTS ${PROJECT_SOURCE_DIR}/config/myconfig.h)
    message(STATUS "Found myconfig.h, including it.")
    include_directories(${PROJECT_SOURCE_DIR}/config)
else()
    message(STATUS "myconfig.h not found.")
endif()

add_executable(my_project ${PROJECT_SOURCE_DIR}/src/main.cpp)
```

In this example:
- We use `PROJECT_SOURCE_DIR` to check for the existence of `myconfig.h` in the `config/` directory relative to the project’s source directory.
- Based on whether the file exists, we conditionally include the directory.

### Key Points about `PROJECT_SOURCE_DIR`

1. **Path to the Project Root**: It points to the directory containing your main `CMakeLists.txt` file.
2. **Portability**: Makes your CMake scripts more portable and easier to maintain by avoiding hardcoded paths.
3. **Always Available**: It is automatically set by CMake, so you can always use it without manually defining it.

### Example Directory Structure

Here’s how `PROJECT_SOURCE_DIR` fits into a typical project structure:

```
my_project/
├── CMakeLists.txt         # Root CMake file
├── src/
│   └── main.cpp
├── include/
│   └── myheader.h
└── build/                 # Build directory (outside of project source)
```

- In this example, `PROJECT_SOURCE_DIR` will point to `my_project/`, and it can be used to reference files like `myheader.h` or `main.cpp` relative to the root of the source directory.

### Summary

- **`PROJECT_SOURCE_DIR`** is a special variable in CMake that refers to the root directory of your project’s source code, where the main `CMakeLists.txt` file is located.
- **Why use it?**
  - **Portability**: Ensures paths are relative to the project root, making your project easier to move or share across different systems.
  - **Clarity**: Makes CMake files easier to read and understand by clearly indicating the source directory.
  - **Consistency**: Helps avoid hardcoding absolute paths, ensuring consistent referencing of files and directories.
  
By using `PROJECT_SOURCE_DIR`, you make your build process more flexible and maintainable, especially as your project grows and you need to handle more complex directory structures.





---------------------------
---------------------------
---------------------------
---------------------------
---------------------------


In CMake, if you have two separate CMake projects (or two separate `CMakeLists.txt` files), and you use `add_subdirectory()` or `PROJECT_SOURCE_DIR` in one project to refer to another project, the behavior depends on where the `CMakeLists.txt` files are located and how the `add_subdirectory()` command is used.

Let's break this down with an example to understand what happens when you call `add_subdirectory()` in one project and use `PROJECT_SOURCE_DIR` in another.

### Key Points:

- **`PROJECT_SOURCE_DIR`** always refers to the directory containing the root `CMakeLists.txt` of the current CMake project. If you call `add_subdirectory()` within a CMake project, `PROJECT_SOURCE_DIR` will still point to the original root of the CMake project, not the subdirectory.
- **`add_subdirectory()`** allows you to include another CMake project or another directory with its own `CMakeLists.txt`. This command effectively adds the directory as a subproject within the current project and processes its `CMakeLists.txt`.

### Scenario 1: `add_subdirectory()` in Project A Refers to Project B

Suppose we have the following directory structure:

```
root_project/
├── CMakeLists.txt     # Main project root
├── project_a/
│   ├── CMakeLists.txt # Subproject A
│   └── src/
├── project_b/
│   ├── CMakeLists.txt # Subproject B
│   └── src/
```

Let's say `root_project/CMakeLists.txt` includes `project_a/CMakeLists.txt` and inside `project_a/CMakeLists.txt`, we use `add_subdirectory()` to include `project_b`.

### Example CMake Files:

1. **`root_project/CMakeLists.txt`**:

```cmake
# Root project CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(RootProject)

# Add project_a as a subdirectory
add_subdirectory(project_a)

# Optionally, add more directories or dependencies here
```

2. **`project_a/CMakeLists.txt`**:

```cmake
# Project A's CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(ProjectA)

# Add project_b as a subdirectory of project_a
add_subdirectory(${CMAKE_SOURCE_DIR}/../project_b)

# Use PROJECT_SOURCE_DIR in project_a
message(STATUS "Project A's source dir: ${PROJECT_SOURCE_DIR}")

# Optionally, define targets in project A
add_executable(project_a_exec src/main.cpp)
```

3. **`project_b/CMakeLists.txt`**:

```cmake
# Project B's CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(ProjectB)

# Use PROJECT_SOURCE_DIR in project_b
message(STATUS "Project B's source dir: ${PROJECT_SOURCE_DIR}")

# Optionally, define targets in project B
add_executable(project_b_exec src/main.cpp)
```

### What Happens:

1. **For `PROJECT_SOURCE_DIR` in `root_project`:**
   - When you call CMake from `root_project`, `PROJECT_SOURCE_DIR` will point to `root_project` because it’s the root project’s `CMakeLists.txt`.
   - `PROJECT_SOURCE_DIR` is not affected by the subprojects or `add_subdirectory()`. It always refers to the directory containing the root `CMakeLists.txt` file (i.e., `root_project`).

2. **For `PROJECT_SOURCE_DIR` in `project_a`:**
   - In `project_a/CMakeLists.txt`, when `PROJECT_SOURCE_DIR` is used, it will point to `project_a`'s directory (because `PROJECT_SOURCE_DIR` always points to the directory containing the current `CMakeLists.txt` file).
   - Even though you added `project_b` as a subdirectory using `add_subdirectory(${CMAKE_SOURCE_DIR}/../project_b)`, `PROJECT_SOURCE_DIR` in `project_a` will still point to `project_a`.

3. **For `PROJECT_SOURCE_DIR` in `project_b`:**
   - In `project_b/CMakeLists.txt`, `PROJECT_SOURCE_DIR` will point to `project_b`'s directory (because it’s the directory containing the current `CMakeLists.txt` file for Project B).

### Key Observations:

- **`add_subdirectory()` does not change `PROJECT_SOURCE_DIR`**: Each `PROJECT_SOURCE_DIR` variable points to the directory where the `CMakeLists.txt` of the corresponding project is located. Even though `project_b` is added as a subdirectory of `project_a`, it does **not change** the behavior of `PROJECT_SOURCE_DIR` in either `project_a` or `project_b`.
  
- **Subprojects don’t overwrite the `PROJECT_SOURCE_DIR` variable**: This variable is specific to the current CMake project and does not change based on the inclusion of subdirectories.

### Scenario 2: Using `PROJECT_SOURCE_DIR` in a Subproject with `add_subdirectory`

If you want to explicitly reference `PROJECT_SOURCE_DIR` in a subproject or its parent, you can pass this information along via cache variables or other means. For example, if `project_a` needs to know the `PROJECT_SOURCE_DIR` of the root project, you could pass it using a cache variable like this:

```cmake
# root_project/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(RootProject)

# Pass the root source directory to project_a via CACHE
set(ROOT_PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR} CACHE PATH "Root project source directory")

add_subdirectory(project_a)
```

Then, in `project_a/CMakeLists.txt`:

```cmake
# project_a/CMakeLists.txt

message(STATUS "Root project source dir from parent: ${ROOT_PROJECT_SOURCE_DIR}")
```

This would allow `project_a` to explicitly know about the root project’s source directory.

### Summary

- **`PROJECT_SOURCE_DIR`** always points to the directory where the current `CMakeLists.txt` file is located. It is not affected by `add_subdirectory()` or subproject structure.
- **`add_subdirectory()`** does not alter `PROJECT_SOURCE_DIR` in any of the subdirectories. Each subdirectory has its own independent `PROJECT_SOURCE_DIR`, which points to the directory of that subproject’s `CMakeLists.txt`.
- If you need to reference the root project’s source directory inside subprojects, you can pass the root project’s path explicitly via CMake variables or cache entries.
