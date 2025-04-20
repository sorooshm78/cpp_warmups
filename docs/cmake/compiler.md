To use **Clang** as the compiler in a CMake project, you need to tell CMake to use `clang` (for C) or `clang++` (for C++) **before** the first `project()` or `enable_language()` command is run in your `CMakeLists.txt` file.

### ✅ Method 1: Set environment variables (recommended for general use)
Before running CMake, you can set environment variables like so:

#### Linux/macOS
```bash
export CC=clang
export CXX=clang++
cmake -S . -B build
```

#### Windows (with PowerShell)
```powershell
$env:CC="clang"
$env:CXX="clang++"
cmake -S . -B build
```

Then proceed to build:
```bash
cmake --build build
```

---

### ✅ Method 2: Set the compiler in `CMakeLists.txt`
You **can** try setting the compiler in `CMakeLists.txt`, but it only works if it’s the **very first thing** (before `project()`), and even then, it's not guaranteed to override an existing cache:

```cmake
# CMakeLists.txt
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
project(MyProject)
```

⚠️ **Warning**: If you’ve already run CMake and generated a `CMakeCache.txt`, it won’t pick up the new compiler setting. You’ll need to delete your `build/` directory or clear the cache:

```bash
rm -rf build/
```
