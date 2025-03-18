# links
* [google-sanitizers](https://github.com/google/sanitizers/wiki/addresssanitizer)
* [AddressSanitizerAlgorithm](https://github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm)
* [Address Sanitizer in C++](https://medium.com/@saadurr/address-sanitizer-in-c-7a6e5017cbb2)

### **AddressSanitizer (ASan)**
AddressSanitizer (ASan) is a runtime memory error detector for C and C++ programs. It helps identify memory corruption bugs, such as:

- **Out-of-bounds accesses** (heap, stack, and global variables)
- **Use-after-free** (dangling pointer dereferences)
- **Use-after-return** (accessing memory from a returned function)
- **Use-after-scope** (accessing variables that have gone out of scope)
- **Double-free** and **wild frees**
- **Memory leaks** (via LeakSanitizer, integrated into ASan)

ASan is available in **Clang**, **GCC**, and **MSVC** (partial support).

---

## **How AddressSanitizer Works**
### **1. Shadow Memory**
ASan uses **shadow memory** to track memory allocations and detect illegal memory access. It:
- Allocates extra **red zones** around memory regions.
- Stores metadata in **shadow memory**.
- Checks every memory access against shadow memory.

### **2. Instrumentation**
- At compile time, ASan replaces memory allocation/deallocation functions (e.g., `malloc`, `free`).
- It inserts **checks** before every memory access to verify its validity.

### **3. Runtime Detection**
- If an invalid access occurs, ASan **aborts execution** and provides a detailed error report.

---

## **Example: Detecting Out-of-Bounds Access**
### **Buggy Code (Out-of-Bounds Read)**
```cpp
#include <iostream>

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    std::cout << arr[10] << std::endl; // Out-of-bounds access!
    return 0;
}
```

### **Compiling with AddressSanitizer**
To enable ASan, compile with:
```sh
g++ -fsanitize=address -g -o asan_example asan_example.cpp
```

### **Output when Running**
```
==12345==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7ffe58c4a5c8 at pc 0x000000401234
READ of size 4 at 0x7ffe58c4a5c8 thread T0
    #0 0x401234 in main asan_example.cpp:5
    #1 0x7f8aab1d3b97 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b97)
    #2 0x4011aa in _start (/asan_example+0x4011aa)

Address 0x7ffe58c4a5c8 is located in stack of thread T0 at offset 40 in frame
    #0 0x401000 in main asan_example.cpp:3
```
### **Analysis of Output**
- **"stack-buffer-overflow"** → Out-of-bounds stack memory access.
- **Address 0x7ffe58c4a5c8** → Invalid access location.
- **Backtrace shows the exact line (`asan_example.cpp:5`)**.

---

## **Example: Detecting Use-After-Free**
### **Buggy Code (Use-After-Free)**
```cpp
#include <iostream>
#include <cstdlib>

int main() {
    int* ptr = (int*)malloc(sizeof(int) * 5);
    free(ptr);
    std::cout << ptr[2] << std::endl; // Use-after-free!
    return 0;
}
```

### **Output when Running with ASan**
```
==12345==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000000010 at pc 0x000000401234
READ of size 4 at 0x603000000010 thread T0
    #0 0x401234 in main asan_example.cpp:6
    #1 0x7f8aab1d3b97 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b97)
    #2 0x4011aa in _start (/asan_example+0x4011aa)

0x603000000010 is located 0 bytes inside of 20-byte region [0x603000000010,0x603000000024)
freed by thread T0 here:
    #0 0x4e5b2a in free
    #1 0x401123 in main asan_example.cpp:5
```
### **Analysis**
- **"heap-use-after-free"** → Invalid memory access after freeing.
- **Line in the program (`asan_example.cpp:6`)** → Problematic memory access.

---

## **AddressSanitizer Features**
### **1. Leak Detection**
Enable LeakSanitizer:
```sh
g++ -fsanitize=address -fsanitize=leak -g -o asan_example asan_example.cpp
```
If a memory leak exists, it will report:
```
==12345==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 20 bytes in main()
```

### **2. Detecting Use-After-Return**
```cpp
#include <iostream>

int* getPointer() {
    int x = 42;
    return &x; // Returning address of local variable (bad practice)
}

int main() {
    int* ptr = getPointer();
    std::cout << *ptr << std::endl; // Use-after-return!
    return 0;
}
```
Output:
```
==12345==ERROR: AddressSanitizer: stack-use-after-return
```

### **3. Runtime Options**
Use environment variables for runtime control:
```sh
export ASAN_OPTIONS=detect_leaks=1:log_path=asan_log
./asan_example
```

---

## **Conclusion**
- **AddressSanitizer is lightweight**, fast, and detects many memory issues.
- It provides **detailed error messages** with stack traces.
- It is **easy to use**: just add `-fsanitize=address` to the compiler flags.


--------------------------------------------------------------------------------------------------------------------

### **AddressSanitizer (ASan)**

**AddressSanitizer** is a memory error detector tool developed by Google. It is designed to catch various types of memory-related bugs in C and C++ programs, such as:

1. **Out-of-bounds access** (reading/writing outside the bounds of an array or buffer).
2. **Use-after-free** (accessing memory after it has been freed).
3. **Double-free** (freeing the same memory block twice).
4. **Memory leaks** (not freeing allocated memory before the program exits).

ASan works by instrumenting the code at compile time, adding checks around memory accesses. It uses a combination of shadow memory and runtime checks to detect errors.

---

### **How AddressSanitizer Works**

1. **Shadow Memory**:
   - ASan uses a "shadow memory" technique to track the state of each byte of memory in the program.
   - Every byte of application memory is mapped to a corresponding byte in shadow memory.
   - Shadow memory indicates whether a byte is valid, invalid, or partially valid.

2. **Instrumentation**:
   - During compilation, ASan inserts checks before every memory access (read/write).
   - These checks consult the shadow memory to determine if the access is valid.

3. **Runtime Library**:
   - ASan includes a runtime library that replaces standard memory allocation functions (e.g., `malloc`, `free`).
   - This library tracks memory allocations and deallocations, and it detects errors like use-after-free or double-free.

4. **Error Reporting**:
   - When an error is detected, ASan prints a detailed report, including:
     - The type of error.
     - The location in the code where the error occurred.
     - A stack trace showing how the program reached the error.

---

### **Example of Using AddressSanitizer**

Let’s look at a simple example of a C program with a memory error and how ASan detects it.

#### **Example Code**
```c
#include <stdlib.h>

int main() {
    int *array = (int *)malloc(10 * sizeof(int)); // Allocate an array of 10 integers
    array[10] = 42; // Out-of-bounds access (valid indices are 0-9)
    free(array);
    return 0;
}
```

This program has an out-of-bounds access: `array[10]` is invalid because the array only has indices 0 through 9.

---

#### **Compiling with AddressSanitizer**

To use ASan, you need to compile your program with the `-fsanitize=address` flag. For example:

```bash
gcc -fsanitize=address -o example example.c
```

---

#### **Running the Program**

When you run the program, ASan will detect the out-of-bounds access and print a detailed error report:

```bash
./example
```

---

#### **ASan Output**

The output might look like this:

```
=================================================================
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x6020000000f8 at pc 0x0000004006b4 bp 0x7ffd4a3b2d20 sp 0x7ffd4a3b2d18
WRITE of size 4 at 0x6020000000f8 thread T0
    #0 0x4006b3 in main example.c:5
    #1 0x7f8c9b8b2b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #2 0x400599 in _start (/path/to/example+0x400599)

0x6020000000f8 is located 0 bytes to the right of 40-byte region [0x6020000000d0,0x6020000000f8)
allocated by thread T0 here:
    #0 0x7f8c9bb5cb50 in malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x400657 in main example.c:4
    #2 0x7f8c9b8b2b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)

SUMMARY: AddressSanitizer: heap-buffer-overflow example.c:5 in main
Shadow bytes around the buggy address:
  0x0c047fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff8000: fa fa 00 00 00 00 00 00 fa fa 00 00 00 00 00 00
=>0x0c047fff8010: fa fa 00 00 00 00 00 00[fa]fa fa fa fa fa fa fa
  0x0c047fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8060: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:     fa
  Freed heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  Container overflow:    fc
  Array cookie:          ac
  Intra object redzone:  bb
  ASan internal:         fe
==12345==ABORTING
```

---

#### **Explanation of the Output**

1. **Error Type**:
   - `heap-buffer-overflow`: The program tried to access memory outside the bounds of a heap-allocated buffer.

2. **Location**:
   - The error occurred in `main` at `example.c:5`.

3. **Memory Region**:
   - The invalid access was at `0x6020000000f8`, which is just outside the allocated region `[0x6020000000d0, 0x6020000000f8)`.

4. **Stack Trace**:
   - The stack trace shows the sequence of function calls leading to the error.

5. **Shadow Bytes**:
   - The shadow memory map shows the state of memory around the buggy address.

---

### **Advantages of AddressSanitizer**

- **High Detection Rate**: Catches a wide range of memory errors.
- **Low Overhead**: Typically slows down the program by 2x-3x, which is much faster than tools like Valgrind.
- **Detailed Reports**: Provides clear and actionable error messages.

---

### **Limitations**

- **Performance Overhead**: While faster than some tools, ASan still introduces runtime overhead.
- **Memory Usage**: ASan increases memory usage due to shadow memory.
- **Platform Support**: Primarily designed for Linux and macOS; Windows support is limited.

---

### **Conclusion**

AddressSanitizer is a powerful tool for detecting memory errors in C and C++ programs. By compiling your code with ASan and running your program, you can quickly identify and fix memory-related bugs, improving the reliability and security of your software.

--------------------------------------------------------------------------------------------------------------------

## How AddressSanitizer Works
### **How AddressSanitizer Works (Detailed Explanation)**

AddressSanitizer (ASan) is a runtime memory error detector that uses a combination of **compile-time instrumentation** and **runtime memory management** to detect memory-related bugs. It works by tracking memory allocations, deallocations, and accesses, and it uses a technique called **shadow memory** to identify invalid memory operations.

Let’s break down how ASan works step by step:

---

### **1. Shadow Memory**

Shadow memory is the core mechanism behind ASan. It is a dedicated memory region that tracks the state of every byte of the application's memory. Here's how it works:

- **Mapping**: Every 8 bytes of application memory are mapped to 1 byte of shadow memory.
- **Shadow Byte Values**:
  - `0`: All 8 bytes are valid (addressable).
  - `1-7`: Only the first `N` bytes are valid (partially addressable).
  - Negative values: The memory is invalid (e.g., freed, redzone, or unallocated).

For example:
- If the shadow byte is `0`, all 8 corresponding application bytes are valid.
- If the shadow byte is `3`, only the first 3 bytes are valid.
- If the shadow byte is `0xFA` (a special value), the memory is in a redzone (invalid).

---

### **2. Instrumentation**

During compilation, ASan instruments the code to insert checks before every memory access (read/write). These checks consult the shadow memory to determine if the access is valid. For example:

- Before accessing `array[i]`, ASan checks if `array + i` is valid by looking up the corresponding shadow byte.
- If the shadow byte indicates the memory is invalid, ASan reports an error.

---

### **3. Runtime Library**

ASan replaces standard memory allocation functions (e.g., `malloc`, `free`) with its own versions. These functions:

- **Allocate Extra Memory**: When you allocate memory (e.g., `malloc(10)`), ASan allocates extra memory for redzones (guard regions) and shadow memory.
- **Track Allocations**: ASan keeps track of all allocated and freed memory blocks.
- **Detect Errors**: When memory is accessed or freed, ASan checks if the operation is valid.

---

### **4. Redzones**

ASan adds **redzones** (guard regions) around allocated memory blocks. These are invalid memory regions that trigger an error if accessed. For example:

- If you allocate 10 bytes, ASan might allocate 16 bytes and mark the extra 6 bytes as redzones.
- Accessing these redzones will cause ASan to report an error.

---

### **5. Error Reporting**

When ASan detects an error, it prints a detailed report, including:
- The type of error (e.g., heap-buffer-overflow, use-after-free).
- The memory address where the error occurred.
- A stack trace showing where the error happened and how the program reached that point.
- Information about the memory region (e.g., allocated size, freed status).

---

### **Example of AddressSanitizer in Action**

Let’s walk through an example to see how ASan works in practice.

#### **Example Code**
```c
#include <stdlib.h>

int main() {
    int *array = (int *)malloc(10 * sizeof(int)); // Allocate an array of 10 integers
    array[10] = 42; // Out-of-bounds access (valid indices are 0-9)
    free(array);
    return 0;
}
```

This program has an out-of-bounds access: `array[10]` is invalid because the array only has indices 0 through 9.

---

#### **Step-by-Step Execution with ASan**

1. **Compilation**:
   - Compile the program with ASan enabled:
     ```bash
     gcc -fsanitize=address -o example example.c
     ```

2. **Memory Allocation**:
   - When `malloc(10 * sizeof(int))` is called, ASan allocates extra memory for redzones and shadow memory.
   - For example, ASan might allocate 48 bytes instead of 40:
     - 40 bytes for the array.
     - 8 bytes for redzones (guard regions).

3. **Shadow Memory Setup**:
   - ASan marks the 40 bytes of the array as valid (shadow byte `0`).
   - It marks the redzones as invalid (shadow byte `0xFA`).

4. **Memory Access**:
   - When `array[10]` is accessed, ASan checks the shadow memory for the address `array + 10 * sizeof(int)`.
   - The shadow byte for this address corresponds to a redzone, which is invalid.

5. **Error Detection**:
   - ASan detects that the access is invalid and prints a detailed error report.

---

#### **ASan Output**

When you run the program, ASan produces output like this:

```
=================================================================
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x6020000000f8 at pc 0x0000004006b4 bp 0x7ffd4a3b2d20 sp 0x7ffd4a3b2d18
WRITE of size 4 at 0x6020000000f8 thread T0
    #0 0x4006b3 in main example.c:5
    #1 0x7f8c9b8b2b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)
    #2 0x400599 in _start (/path/to/example+0x400599)

0x6020000000f8 is located 0 bytes to the right of 40-byte region [0x6020000000d0,0x6020000000f8)
allocated by thread T0 here:
    #0 0x7f8c9bb5cb50 in malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb50)
    #1 0x400657 in main example.c:4
    #2 0x7f8c9b8b2b96 in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x21b96)

SUMMARY: AddressSanitizer: heap-buffer-overflow example.c:5 in main
Shadow bytes around the buggy address:
  0x0c047fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c047fff8000: fa fa 00 00 00 00 00 00 fa fa 00 00 00 00 00 00
=>0x0c047fff8010: fa fa 00 00 00 00 00 00[fa]fa fa fa fa fa fa fa
  0x0c047fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8060: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:     fa
  Freed heap region:     fd
  Stack left redzone:    f1
  Stack mid redzone:     f2
  Stack right redzone:   f3
  Stack after return:    f5
  Stack use after scope: f8
  Global redzone:        f9
  Global init order:     f6
  Poisoned by user:      f7
  Container overflow:    fc
  Array cookie:          ac
  Intra object redzone:  bb
  ASan internal:         fe
==12345==ABORTING
```

---

#### **Key Points in the Output**

1. **Error Type**: `heap-buffer-overflow` (out-of-bounds access).
2. **Location**: The error occurred in `main` at `example.c:5`.
3. **Memory Address**: `0x6020000000f8` is just outside the allocated region `[0x6020000000d0, 0x6020000000f8)`.
4. **Stack Trace**: Shows the sequence of function calls leading to the error.
5. **Shadow Memory**: The shadow byte `[fa]` indicates a redzone (invalid memory).

---

### **Conclusion**

AddressSanitizer is a powerful tool for detecting memory errors in C and C++ programs. By using shadow memory, compile-time instrumentation, and runtime checks, it can catch a wide range of memory-related bugs with minimal performance overhead. The detailed error reports make it easy to identify and fix issues in your code.


--------------------------------------------------------------------------------------------------------------------

## Address Sanitizer in C++
AddressSanitizer (ASan) is a debugging tool that is used to detect memory bugs in C++ programs. It works by inserting special instrumentation into the program’s compiled binary code, which allows it to monitor memory accesses and detect when the program is attempting to access memory that is not valid or has been freed.

ASan can be used to detect a wide range of memory errors, including out-of-bounds accesses, use-after-free errors, and memory leaks. It can help to catch these errors early in the development process, which can save time and effort by allowing developers to fix problems before they become more difficult to track down.

To use ASan, developers can compile their C++ programs with the appropriate flags, which will enable the instrumentation and enable ASan to monitor the program’s memory accesses. When a memory error is detected, ASan will print a diagnostic message to the console, which can help developers to understand what went wrong and how to fix the problem.

### Using AddressSanitizer
To use AddressSanitizer (ASan) with C++, you will need to compile your program with the appropriate flags. This will enable ASan’s instrumentation, which will monitor your program’s memory accesses and detect any errors.

Here are the steps to use ASan with C++:

* Install the ASan runtime library: ASan is typically distributed as a runtime library, which means that you will need to install it on your system before you can use it. The installation process will depend on your operating system and package manager.
* Compile your program with the ASan flags: To enable ASan’s instrumentation, you will need to pass the appropriate flags to your compiler when you compile your program. For example, if you are using GCC or Clang, you can use the -fsanitize=address flag to enable ASan.
* Run your program: Once your program is compiled with ASan, you can run it as you would any other program. If ASan detects a memory error, it will print a diagnostic message to the console, which will include information about the error and the location in your code where it occurred.

* Fix any errors: If ASan detects a memory error, you will need to identify and fix the problem in your code. This may involve adding additional error checking, using more robust data structures, or following best practices for memory management.

### Example of using ASan on Linux
Here is an example of how to use AddressSanitizer (ASan) with C++ on a Linux system:
```
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  // Allocate some memory
  int* array = new int[10];
  // Use the memory
  for (int i = 0; i < 10; i++) {
    array[i] = i;
  }
  // Free the memory
  delete[] array;
  // Try to use the freed memory
  array[5] = 10;  // This will trigger an ASan error
  return 0;
}
```

To compile this program with ASan, you can use the `-fsanitize=address` flag with GCC or Clang:

```
g++ -fsanitize=address example.cpp -o example
```

When you run the program, ASan will detect the error when the program tries to access the freed memory and will print a diagnostic message to the console:

```
ASan:SIGSEGV
==============
...

ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010 at pc 0x00000040105e bp 0x7ffeea29a6f0 sp 0x7ffeea29a6e8
WRITE of size 4 at 0x602000000010 thread T0
    #0 0x40105d in main example.cpp:14
    #1 0x7f9d9f9b4f45 in __libc_start_main /build/glibc-OTsEL5/glibc-2.27/csu/../csu/libc-start.c:310
    #2 0x401019 in _start (/home/user/example+0x401019)

Address 0x602000000010 is located in stack of thread T0 at offset 16 in frame
    #0 0x400f44 in main example.cpp:8

This frame has 1 object(s):
[32, 40) 'array'
HINT: this may be a false positive if your program uses some custom stack unwind mechanism or swapcontext
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: heap-use-after-free (/home/user/example+0x40105d) in main
Shadow bytes around the buggy address:
  0x0c3a7fff9fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c3a7fff9fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c3a7fff9fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c3a7fff9fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c3a7fff9ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c3a7fffa000: fa fa fa 00 fd fd fd 00[fd]fd fd 00 fd fd fd 00
  0x0c3a7ff
```

Here is a breakdown of what each part of the output means:

* `ASan:SIGSEGV:` This indicates that ASan has detected a memory error, specifically a segmentation fault (SIGSEGV) caused by an invalid memory access.
* `ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010:` This line describes the type of error that was detected and the address in memory where the error occurred. In this case, the error is a "heap-use-after-free" error, which means that the program is attempting to access memory that has already been freed.
* `pc 0x00000040105e:` This is the address of the instruction in the program's compiled binary code that caused the error.

### Using ASan with CMake
To use AddressSanitizer (ASan) with CMake, you will need to add the appropriate flags to your CMakeLists.txt file. This will enable ASan’s instrumentation when you compile your program.

1. First, check if ASan is available on your system:

```
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-fsanitize=address ASAN_AVAILABLE)
```

2. If ASan is available, you can add the appropriate flags to your 
CMakeLists.txt file:

```
if (ASAN_AVAILABLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()
```

3. Then, you can compile your program as usual using CMake. ASan’s instrumentation will be enabled and will detect any memory errors that occur during runtime.

### Summary
It is important to note that using ASan may slow down your program, as the instrumentation adds overhead to memory accesses. However, this overhead is typically small, and the benefits of using ASan to catch memory errors can far outweigh the performance cost.
