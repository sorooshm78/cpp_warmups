### `include` in CMake

In CMake, the `include` command is used to load CMake scripts, modules, or other files into the current CMake project. This command allows you to modularize your CMake configuration and reuse CMake code across different projects. You can use `include` to load CMake files that contain reusable functions, macros, or configuration settings.

### Why is `include` Needed?

The `include` command is essential in large and complex CMake-based projects. As projects grow, it becomes necessary to organize and modularize the CMake configuration. This can help with:

1. **Modularity**: You can split complex CMake configurations into multiple files, making the build system easier to maintain and understand.
2. **Reusability**: Common build logic (like compiler flags, search paths, and platform-specific settings) can be reused across multiple projects or multiple modules of the same project.
3. **Separation of Concerns**: Each CMake file can focus on a specific part of the project, such as setting up dependencies, defining targets, or handling platform-specific settings.

Without the `include` command, every configuration would need to be written inside the `CMakeLists.txt` files, leading to redundancy and less maintainable code.

### What Problem Does `include` Solve?

- **Reusability of CMake Logic**: It helps you reuse common CMake code across multiple projects or modules.
- **Organization**: Allows you to break down complex build systems into smaller, easier-to-manage files.
- **Maintainability**: When build settings, compiler flags, or macros need to change, you can modify the include file rather than multiple places in each `CMakeLists.txt`.

---

### Syntax of `include`

The basic syntax of the `include` command is as follows:

```cmake
include(<filename>)
```

- `<filename>`: The path to the CMake script or module that you want to include. This can be a relative or absolute path.

You can also specify options like `OPTIONAL` to avoid errors if the file is not found, or `NO_POLICY_SCOPE` to prevent the file from affecting the policies of the current CMake project.

---

### Example 1: Using `include` to Modularize CMake Configuration

Suppose you have a project that needs to configure different settings depending on the platform (e.g., Linux vs. Windows). Instead of repeating the logic in every `CMakeLists.txt`, you can modularize the configuration into separate files.

#### 1. **Create a Separate CMake File** for Platform-Specific Settings

Let's create a file called `platform_config.cmake` that contains platform-specific configurations.

```cmake
# platform_config.cmake

if(WIN32)
    message(STATUS "Configuring for Windows")
    set(MY_PLATFORM "Windows")
    set(SOME_WINDOWS_SPECIFIC_FLAG TRUE)
elseif(APPLE)
    message(STATUS "Configuring for macOS")
    set(MY_PLATFORM "macOS")
    set(SOME_MACOS_SPECIFIC_FLAG TRUE)
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    set(MY_PLATFORM "Linux")
    set(SOME_LINUX_SPECIFIC_FLAG TRUE)
endif()
```

#### 2. **Include the `platform_config.cmake` in Your Main CMake File**

Now, in your main `CMakeLists.txt`, use `include` to load the `platform_config.cmake` file.

```cmake
# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(MyProject)

# Include the platform-specific configuration
include(platform_config.cmake)

# Use the platform-specific variables in your project
message(STATUS "Current platform: ${MY_PLATFORM}")
```

#### 3. **Explanation**:

- **Modularization**: The `platform_config.cmake` file contains platform-specific configuration. Instead of having multiple `if` conditions in your main `CMakeLists.txt`, this is extracted into a separate file, making the main `CMakeLists.txt` cleaner and easier to read.
- **Reusability**: The `platform_config.cmake` file can be reused in other projects or CMake modules without duplicating the logic.
- **Maintainability**: If platform-specific configurations need to change, you only need to update `platform_config.cmake` instead of modifying every `CMakeLists.txt`.

#### 4. **Output** (depending on the platform):

- On Windows, the output will show `Configuring for Windows` and `Current platform: Windows`.
- On macOS, it will show `Configuring for macOS` and `Current platform: macOS`.
- On Linux, it will show `Configuring for Linux` and `Current platform: Linux`.

---

### Example 2: Including CMake Modules

In addition to including your own CMake scripts, you can use the `include` command to include CMake modules, such as the `FindBoost.cmake` module, which is used to find and configure the Boost library.

#### 1. **Use `include` to Find Boost**

```cmake
# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(MyProject)

# Include CMake's FindBoost module
include(FindBoost)

# Find Boost
find_package(Boost 1.70 REQUIRED)

add_executable(my_project main.cpp)

# Link Boost libraries to the project
target_link_libraries(my_project Boost::Boost)
```

#### 2. **Explanation**:

- `include(FindBoost)` includes the CMake module that provides logic for finding Boost. CMake automatically searches for this module in the standard CMake modules path.
- `find_package(Boost)` then uses the logic defined in the `FindBoost.cmake` module to locate Boost and set the necessary variables (like `Boost_INCLUDE_DIR` and `Boost_LIBRARIES`).
- This eliminates the need for manually specifying Boost's path, making your project easier to set up across different systems.

---

### Example 3: Using `include` with Macros or Functions

You can also define reusable functions or macros in separate files and include them in your main CMake configuration.

#### 1. **Define a Function in a Separate File**

```cmake
# macros.cmake

# Define a function to print the current platform
function(print_platform)
    if(WIN32)
        message(STATUS "Running on Windows")
    elseif(APPLE)
        message(STATUS "Running on macOS")
    elseif(UNIX)
        message(STATUS "Running on Linux")
    endif()
endfunction()
```

#### 2. **Include and Use the Function**

```cmake
# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(MyProject)

# Include the macros file
include(macros.cmake)

# Call the function defined in macros.cmake
print_platform()
```

#### 3. **Explanation**:

- The `macros.cmake` file contains a reusable function `print_platform` that checks the platform and prints a corresponding message.
- By using `include(macros.cmake)` in the main `CMakeLists.txt`, you can access and call `print_platform()` to print platform-specific messages.
- This modular approach keeps the CMake logic for platform-specific printing in one place and allows reuse in multiple projects.

---

### Summary: Why Use `include` in CMake?

1. **Modularity**: Break down complex CMake configurations into smaller, more manageable files.
2. **Reusability**: Reuse common CMake logic (macros, functions, platform-specific settings) across multiple projects or modules.
3. **Maintainability**: Keep `CMakeLists.txt` files clean and easy to manage, especially in large projects.
4. **Flexibility**: Easily load CMake modules or external scripts that provide additional functionality, such as finding dependencies or handling platform-specific configuration.

The `include` command is a powerful tool for organizing and modularizing your CMake configuration, making it easier to manage large, complex projects while ensuring that common logic is reusable and maintainable.