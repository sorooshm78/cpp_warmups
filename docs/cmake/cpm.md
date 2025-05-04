Certainly! **CPM** stands for **CMake Package Manager**. It's a small CMake script that makes it easy to manage external dependencies inside your CMake projects without needing to manually download and configure them.

---

### What is CPM in CMake?

CPM simplifies fetching, building, and using CMake packages by automatically downloading the dependency if it's not found locally. It's somewhat like `fetchcontent`, but with a more user-friendly interface and built-in caching.

- No need to manually clone or install dependencies.
- Automatically downloads dependencies from GitHub or other URLs.
- Supports setting version and revision for reproducibility.
- Integrates smoothly with CMakeLists.

---

### How to Use CPM

To use CPM in your project, you start by including the CPM script itself. This is done by downloading CPM.cmake from the official repo or by including it inline.

Here’s a minimal example:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject)

# Include CPM.cmake (fetch CPM script directly from GitHub)
include(FetchContent)

FetchContent_Declare(
  CPM
  GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake.git
  GIT_TAG origin/master
)

FetchContent_MakeAvailable(CPM)

# Use CPM to fetch a dependency, e.g. spdlog

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  VERSION 1.11.0
)

add_executable(my_app main.cpp)

target_link_libraries(my_app PRIVATE spdlog::spdlog)
```

---

### Detailed Explanation:

- `include(FetchContent)` and `FetchContent_Declare/MakeAvailable` is used here to import CPM itself.
- `CPMAddPackage` is the key command to add dependencies.
  - `NAME` is an arbitrary name for the dependency.
  - `GITHUB_REPOSITORY` is the repo to download.
  - `VERSION` specifies version/tag/commit.
- If the package is already downloaded, CPM uses the cache, so it won’t download every time.
- `target_link_libraries` links your executable/library to the dependency target.

---

### Another Example: Using fmt library with CPM

```cmake
cmake_minimum_required(VERSION 3.14)
project(FmtExample)

include(FetchContent)

FetchContent_Declare(
  CPM
  GIT_REPOSITORY https://github.com/cpm-cmake/CPM.cmake.git
  GIT_TAG origin/master
)
FetchContent_MakeAvailable(CPM)

CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 9.1.0
)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE fmt::fmt)
```

---

### Summary
- CPM is a CMake module to easily fetch and manage dependencies.
- It downloads libraries automatically if missing.
- Uses `CPMAddPackage` for adding dependencies.
- Makes writing portable and simple CMake projects easier.

---
