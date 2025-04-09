* ![what-is-the-purpose-of-the-ebp-frame-pointer-register](https://stackoverflow.com/questions/579262/what-is-the-purpose-of-the-ebp-frame-pointer-register)

---------------------------------------------------------------

The frame pointer is a reference pointer allowing a debugger to know where a local variable or an argument is, with a single constant offset. Although ESP's value changes over the course of execution, EBP remains the same making it possible to reach the same variable at the same offset (such as the first parameter always being at EBP+8 while ESP offsets can change significantly since you'll be pushing/popping things).

Why don't compilers throw away the frame pointer? Because with the frame pointer, the debugger can figure out where local variables and arguments are using the symbol table since they are guaranteed to be at a constant offset to EBP. Otherwise there isn't an easy way to figure where a local variable is at any point in code.

As Greg mentioned, it also helps stack unwinding for a debugger since EBP provides a reverse linked list of stack frames therefore letting the debugger figure out the size of a stack frame (local variables + arguments) of the function.

Most compilers provide an option to omit frame pointers although it makes debugging really hard. That option should never be used globally, even in release code. You don't know when you'll need to debug a user's crash.

---------------------------------------------------------------