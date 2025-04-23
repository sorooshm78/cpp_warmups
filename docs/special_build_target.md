### What CMake calls *special* (or *standard*) build targets  

CMake automatically creates a handful of targets in every build tree in addition to the ones you declare with `add_executable()` / `add_library()` / `add_custom_target()`.  
They appear when you run `cmake --build` (or `make`, `ninja`, etc.) and you invoke them with the ordinary **–--target** switch:

```bash
cmake --build build-dir --target <name> [--config <cfg>]
```

Below are the ones you meet most often, what they do, and when they are available.

| Target name (Makefiles/Ninja) | Generator-specific names | Appears when… | What it does |
|-------------------------------|--------------------------|---------------|--------------|
| `all`                         | `ALL_BUILD` (VS/Xcode)   | always        | Default target – builds every target that is part of the project. |
| `clean`                       |                          | always        | Deletes all products created by the compiler/linker. citeturn10search1 |
| `install`<br/>`install/strip`<br/>`install/local` …      | `INSTALL` (VS/Xcode) | after at least one `install()` command | Runs the project’s install rules; variants strip symbols or limit installation to the top directory. citeturn5search1 |
| `package` / `package_source`  | `PACKAGE` / `PACKAGE_SOURCE` | after `include(CPack)` (or `cpack` variables) | Invokes CPack to generate binary or source packages. citeturn8search1 |
| `test` (Unix) / `RUN_TESTS` (VS/Xcode) |            | after `enable_testing()` | Runs CTest with the arguments in `CMAKE_CTEST_ARGUMENTS`. citeturn6search0 |
| `edit_cache` & `rebuild_cache`| `ZERO_CHECK` (VS/Xcode)  | always        | Opens an interactive cache editor or forces CMake to re-generate if `CMakeLists.txt` changed. `ZERO_CHECK` depends on this check automatically in IDEs. citeturn9search0 |
| `list_install_components`     |                          | after components are defined with `install(... COMPONENT <name>)` | Prints the component list. |
| `help`                        |                          | Makefile generator only | Prints every target with a short hint. |

> **Reserved names:** these built-in targets live outside normal target space; CMake will **not** let you reference them with `add_dependencies()` or `add_custom_target()` – attempting to do so triggers an error. citeturn12search0  

---

### A minimal hands-on example

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.24)
project(SpecialTargetsDemo LANGUAGES CXX)

add_executable(hello src/hello.cpp)
install(TARGETS hello RUNTIME DESTINATION bin)

enable_testing()
add_test(NAME run_hello COMMAND hello)
include(CPack)           # enable package/package_source targets
```

```cpp
// src/hello.cpp
#include <iostream>
int main() { std::cout << "Hello\n"; }
```

1. Configure & build the default target (`all`):

   ```bash
   cmake -S . -B build
   cmake --build build            # same as --target all
   ```

2. Remove build artefacts:

   ```bash
   cmake --build build --target clean
   ```

3. Install the project into `stage/`:

   ```bash
   cmake --install build --prefix "$(pwd)/stage"   # shorthand for --target install
   ```

4. Run the unit tests:

   ```bash
   cmake --build build --target test               # or RUN_TESTS in an IDE
   ```

5. Produce a self-extracting installer or .zip:

   ```bash
   cmake --build build --target package            # binary package
   cmake --build build --target package_source     # source archive
   ```

---

### Creating *your own* special-purpose target

Special targets are handy to group maintenance tasks.  
Unlike CMake’s reserved ones, you create them with `add_custom_target()`:

```cmake
file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS *.cpp *.h)
add_custom_target(format
    COMMAND clang-format -i ${ALL_SOURCES}
    COMMENT "Re-formatting every source file")
```

* `format` does **not** build automatically (`ALL` is omitted).  
* Invoke it exactly like the built-ins:

```bash
cmake --build build --target format
```

If you **do** want such a helper to run whenever someone builds `all`, add the `ALL` keyword:

```cmake
add_custom_target(lint ALL COMMAND python run_clang_tidy.py)
```

---

#### Key take-aways

* Special/built-in targets give you common lifecycle actions (`clean`, `install`, `test`, `package`, …) for free.  
* They are invoked through your build tool exactly like ordinary targets.  
* Their names are **reserved** – you can’t attach dependencies to them inside `CMakeLists.txt`.  
* Custom targets you create with `add_custom_target()` let you extend the idea for project-specific chores.

Feel free to let me know which generator or workflow you use if you need deeper examples (e.g. Visual Studio, Xcode, Ninja, …).







-----------------------------
-----------------------------
-----------------------------
-----------------------------
-----------------------------





# Special Build Targets in CMake

CMake provides several special build targets that serve specific purposes in your build system. These targets don't produce output files themselves but control aspects of the build process. Here are the main special build targets with examples:

## 1. `all` (Default Target)

This is the default target that gets built when you run `make` without specifying a target. It builds all executable and library targets in your project.

```cmake
add_executable(app1 src1.cpp)
add_executable(app2 src2.cpp)
# 'all' will build both app1 and app2 by default
```

## 2. `clean`

Removes all built files (object files, executables, etc.):

```cmake
# Automatically available, no need to define
# Usage: make clean
```

## 3. `test`

Runs the tests defined in your project (requires CTest):

```cmake
enable_testing()
add_test(NAME my_test COMMAND test_executable)

# Usage: make test
```

## 4. `install`

Installs the built targets and files to the specified installation directory:

```cmake
install(TARGETS mylib DESTINATION lib)
install(FILES myheader.h DESTINATION include)

# Usage: make install
```

## 5. `package`

Creates a distributable package (requires CPack):

```cmake
include(CPack)
set(CPACK_GENERATOR "ZIP")

# Usage: make package
```

## 6. `edit_cache`

Allows editing of CMake cache variables:

```cmake
# Automatically available
# Usage: make edit_cache
```

## 7. `rebuild_cache`

Rebuilds the CMake cache:

```cmake
# Automatically available
# Usage: make rebuild_cache
```

## 8. Custom Special Targets

You can create your own special targets using `add_custom_target()`:

### Example 1: Documentation Target

```cmake
add_custom_target(docs
    COMMAND doxygen Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating documentation"
)

# Usage: make docs
```

### Example 2: Formatting Target

```cmake
find_program(CLANG_FORMAT "clang-format")
add_custom_target(format
    COMMAND ${CLANG_FORMAT} -i ${SOURCES}
    COMMENT "Formatting source code"
)

# Usage: make format
```

### Example 3: Combined Build and Test

```cmake
add_custom_target(check
    DEPENDS all test
    COMMENT "Build all and run tests"
)

# Usage: make check
```

## Important Notes:

1. These targets are case-sensitive in some build systems (like Makefiles)
2. Some targets like `clean` and `all` are always available
3. Targets like `test` and `package` require enabling specific modules (`CTest`, `CPack`)
4. You can see available targets with `make help` or `cmake --build . --target help`

These special targets help organize your build process and provide standard interfaces for common build operations.