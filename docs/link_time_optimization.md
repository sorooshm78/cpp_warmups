![LinkTimeOptimization](https://llvm.org/docs/LinkTimeOptimization.html)

--------------------------
--------------------------
--------------------------

In C++, **LTO** stands for **Link Time Optimization**. It’s a compiler feature that allows the compiler to perform optimization across all the files of a program during the linking stage, rather than just optimizing each file separately during compilation.

### What does that mean?

Normally, when you compile a C++ program, each source file (`.cpp`) is compiled into object code (`.o` or `.obj`) independently. The compiler only sees the code in that one file, so it can’t optimize across different files. Then, at the link stage, these object files are combined into the final executable.

With **LTO**, the compiler delays some optimizations until link time, when it has access to the whole program’s code. This lets it do better optimizations such as:

- **Inlining functions** defined in other files.
- Eliminating unused functions or variables across files.
- Better constant propagation and dead code elimination across modules.
- More aggressive optimizations that work only if you see all code together.

### How to enable LTO?

With popular compilers like GCC or Clang, you enable LTO by passing flags like:

```bash
g++ -flto -O2 file1.cpp file2.cpp -o myprogram
```

Both compilation and linking stages must use the `-flto` flag.

### Summary

- **LTO:** Link Time Optimization
- **Purpose:** Optimize across multiple translation units during linking
- **Benefit:** Generates faster and smaller executables by better cross-file optimization
- **Usage:** Enable with `-flto` flag in compiler/linker

------------------
------------------
------------------

To enable **LTO (Link Time Optimization)** in a CMake project, you generally need to add the appropriate compiler and linker flags, or use the built-in CMake support available in newer versions (3.9+).

### Simple way (CMake 3.9+)

Since CMake 3.9, you can use the target property `INTERPROCEDURAL_OPTIMIZATION` like this:

```cmake
cmake_minimum_required(VERSION 3.9)
project(MyProject)

add_executable(myapp main.cpp)

# Enable LTO for this target
set_property(TARGET myapp PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
```

This tells CMake to enable the compiler and linker flags for LTO automatically for that target.

---

### Manual method (for earlier CMake versions or more control)

You can add `-flto` flags manually for GCC/Clang:

```cmake
add_executable(myapp main.cpp)

target_compile_options(myapp PRIVATE -flto)
target_link_options(myapp PRIVATE -flto)
```

Or, if `target_link_options()` is not available (pre-CMake 3.13), you can do:

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
```

---

### Notes

- LTO can increase compile time because optimization is delayed until linking.
- Make sure all your libraries and dependencies are built with LTO enabled if you want consistent optimization.
- Some compilers/platforms might require additional flags or settings.

---
