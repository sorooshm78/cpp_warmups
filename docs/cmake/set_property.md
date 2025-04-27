Sure! Let’s dive deep into **`set_property`** in CMake — what it is, how it works, when and why you use it.

---

## What is `set_property` in CMake?

`set_property` is a versatile CMake command for **setting properties** on various CMake entities.

### Syntax:

```cmake
set_property(<GLOBAL | DIRECTORY | TARGET | SOURCE | TEST | INSTALL | CACHE> PROPERTY <prop> <value>...)
```

- **Scope:** Specifies *where* the property is set.
- **Property:** The name of the property to set.
- **Value:** One or more values to assign to the property.

---

## What are properties in CMake?

Properties are **key-value pairs** that describe or control behavior of:

- CMake itself (global-level)
- Directories
- Targets (executable, library)
- Source files
- Tests
- Cache entries
- Install rules

They affect how CMake configures and generates build systems.

---

## Major scopes you can set properties on

| Scope       | Description                                         |
|-------------|---------------------------------------------------|
| **GLOBAL**  | Global CMake state — affects entire project       |
| **DIRECTORY** | Specific directory and its children               |
| **TARGET**  | Targets (executable, library)                      |
| **SOURCE**  | Individual source files                             |
| **TEST**    | CTest test commands                                |
| **INSTALL** | Installation rules                                 |
| **CACHE**   | Cached variables in `CMakeCache.txt`              |

---

## Examples of `set_property` usage

### 1. Global property

```cmake
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
```

*Enables using folders to group targets in IDEs globally.*

---

### 2. Directory property

```cmake
set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "temp.txt")
```

*Adds file to clean list in the current directory.*

---

### 3. Target property

```cmake
add_executable(MyApp main.cpp)

set_property(TARGET MyApp PROPERTY CXX_STANDARD 17)
```

*Sets C++ standard for the target `MyApp`.*

---

### 4. Source file property

```cmake
set_property(SOURCE foo.cpp PROPERTY COMPILE_FLAGS "-Wall")
```

*Adds extra compile flags for an individual file.*

---

### 5. Cache property (rarely used)

```cmake
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
```

*Defines possible values for a cache variable.*

---

## Why use `set_property`?

### 1. More fine-grained and flexible control

- Some properties can only be set via `set_property`.
- Allows controlling behavior on specific scopes (global, target, file).

### 2. Works well with generator expressions and lists

- You can set lists of values.
- Some properties can be *added* or *removed* dynamically.

### 3. Better organization

- Centralizes property setting rather than mixing with `set()` commands.
- Some properties have *getters* and *setters* (`get_property` vs `set_property`).

---

## What problems does it solve?

- **Configuring build system features** (e.g., enabling warnings globally or per target).
- **Customizing build behavior per file** (e.g., special compile flags).
- **Setting executable, compiler, linker options** in a structured way.
- **Controlling IDE integration features** (folder grouping, project naming).
- **Adding hooks or wrappers** for build steps (`RULE_LAUNCH_COMPILE` for timing/wrapping compiler).

---

## Quick contrast with `set()`

- `set()` is primarily for **variable assignment**.
- `set_property()` is for **assigning meta-information/properties on objects/entities** in CMake.
  
For example:

```cmake
set(MYVAR 1)               # sets variable MYVAR
set_property(TARGET MyApp PROPERTY CXX_STANDARD 17)  # sets a formal property on target
```

---

## Summary table

| Command         | Purpose                | Typical Use                                         |
|-----------------|------------------------|----------------------------------------------------|
| `set()`         | Set variables          | Basic variable assignment                           |
| `set_property()`| Set properties         | Configure CMake objects, behavior, and build system|

---

## When to use `set_property`?

- When you want to **change CMake or build system behavior** via predefined properties.
- When **manipulating targets, sources, directories, or global CMake settings**.
- When working with **IDE features, compilation rules, or installation rules**.
- When you want a **structured, object-oriented configuration approach**.

---

---------------
---------------
---------------
---------------
---------------

Certainly! Here’s a **full, simple example** of using `set_property` in CMake, along with how to **get** that property using `get_property`.

---

# Basic Workflow of set_property and get_property

- `set_property` — assigns a property to a CMake entity (target, directory, global, etc.)
- `get_property` — reads (retrieves) a property value from that entity

---

# Example: Setting and Getting a Target Property

Suppose you have a target (an executable or library) and want to:

- Set **C++ standard** to 17 on the target.
- Get the C++ standard property later and print it.

---

## Step 1: Define a target

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

add_executable(MyApp main.cpp)
```

---

## Step 2: Set a property on the target

Use `set_property(TARGET ...)` to set the C++ standard:

```cmake
set_property(TARGET MyApp PROPERTY CXX_STANDARD 17)
```

This tells CMake to compile `MyApp` with C++17.

---

## Step 3: Get the property value

You can retrieve (get) the property value and store it in a variable:

```cmake
get_property(cpp_std TARGET MyApp PROPERTY CXX_STANDARD)
message("MyApp C++ standard is: ${cpp_std}")
```

This will output something like:

```
MyApp C++ standard is: 17
```

---

# Full CMakeLists.txt example

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Add executable target
add_executable(MyApp main.cpp)

# Set the C++ standard property on target
set_property(TARGET MyApp PROPERTY CXX_STANDARD 17)

# Get and print the property
get_property(cpp_std TARGET MyApp PROPERTY CXX_STANDARD)
message("MyApp C++ standard is: ${cpp_std}")
```

---

# Explanation

- **`set_property`** assigns the property `CXX_STANDARD` with value `17` to `MyApp`.
- **`get_property`** retrieves the value of `CXX_STANDARD` for `MyApp`.
- `message()` prints the value during configuration.

---

# Extra: Setting and getting a GLOBAL property

Sometimes you want to set a global CMake property:

```cmake
set_property(GLOBAL PROPERTY GLOBAL_PROP_NAME "HelloGlobal")

get_property(global_value GLOBAL PROPERTY GLOBAL_PROP_NAME)
message("Global property value: ${global_value}")
```

---

# Summary

| Command           | Description                        |
|-------------------|----------------------------------|
| `set_property`    | Assign property to a CMake entity|
| `get_property`    | Retrieve property value           |

Both commands work with scopes like **TARGET, GLOBAL, DIRECTORY, SOURCE, TEST, INSTALL, CACHE**.

---

----------
----------
----------
----------

Certainly! Let’s fully unpack the CMake command you asked about:

```cmake
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
```

---

## What does it do?

This command **sets a global CMake property** named `RULE_LAUNCH_COMPILE` to the value:

```
${CMAKE_COMMAND} -E time
```

Let's break it down piece by piece.

---

## Background on `set_property` in CMake

- **`set_property(<scope> PROPERTY <property> <value>)`** sets a property in a specific scope.
- **`GLOBAL`** scope means this property affects the entire CMake process and all targets.
- Properties control various behaviors of CMake and the build system.

---

## What is the `RULE_LAUNCH_COMPILE` property?

- It’s a **special built-in property** in CMake.
- It **modifies the command CMake uses to invoke the compiler**.
- More precisely, it **prepends a command to every compile rule** (each time source files are compiled).
- This property is often used for:

  - Adding wrappers around the compiler invocation.
  - Measuring time.
  - Adding profiling, debugging, or custom tools in front of the compiler.

---

## What’s `${CMAKE_COMMAND}`?

- `${CMAKE_COMMAND}` is a **built-in CMake variable**.
- It holds the **full path to the `cmake` executable currently running**.
- Using `${CMAKE_COMMAND} -E` invokes CMake’s **command mode**, which supports various utility commands like `copy`, `time`, `echo`, etc.

---

## What is `cmake -E time`?

- `cmake -E time <command>` runs `<command>` and **prints the execution time** of that command.
- Like Unix `time` utility, but built into CMake, making it portable.
- Useful for **measuring duration** of build steps.

---

## So, effectively:

```cmake
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
```

Means:

- **Every compilation command (compiler invocation) will be wrapped by:**

  ```
  cmake -E time <original-compile-command>
  ```

- Instead of running, for example:

  ```
  gcc -c foo.cpp -o foo.o
  ```

- It effectively runs:

  ```
  cmake -E time gcc -c foo.cpp -o foo.o
  ```

- CMake will show timing information about how long each compile command takes.

---

## Why is this useful?

- **Performance profiling**: See which compile commands are slow.
- This helps **optimize build times** by identifying bottlenecks.
- Because `cmake -E time` is portable, this works cross-platform (Linux, Windows, macOS).
- You don’t need external `time` tools which may differ by platform.

---

## Important notes

- This **affects all targets** in the project because the property is set at GLOBAL scope.
- It only affects **compile commands**, not link or install.
- If you want to only time certain targets, you can set this property on a target scope instead.

---

## Summary

| Part                     | Meaning                                |
|--------------------------|---------------------------------------|
| `set_property`           | Command to set a CMake property        |
| `GLOBAL`                 | Property applies to the entire project|
| `PROPERTY RULE_LAUNCH_COMPILE` | Special property that wraps compile commands|
| `"${CMAKE_COMMAND} -E time"` | Wrapper command runs compiler and prints timing info |

---

## Example output when compiling with this set

```plaintext
[100%] Building CXX object foo.o
0.543 seconds for gcc ...  # printed by cmake -E time
```

---

