* [what-is-the-purpose-of-the-ebp-frame-pointer-register](https://stackoverflow.com/questions/579262/what-is-the-purpose-of-the-ebp-frame-pointer-register)
* [Understanding Frame Pointers](https://people.cs.rutgers.edu/~pxk/419/notes/frames.html)
* [stack-memory](https://www.varonis.com/blog/stack-memory-3)
* [the-anatomy-of-a-stack-frame-a-guide](https://chessman7.substack.com/p/the-anatomy-of-a-stack-frame-a-guide)

---------------------------------------------------------------

The frame pointer is a reference pointer allowing a debugger to know where a local variable or an argument is, with a single constant offset. Although ESP's value changes over the course of execution, EBP remains the same making it possible to reach the same variable at the same offset (such as the first parameter always being at EBP+8 while ESP offsets can change significantly since you'll be pushing/popping things).

Why don't compilers throw away the frame pointer? Because with the frame pointer, the debugger can figure out where local variables and arguments are using the symbol table since they are guaranteed to be at a constant offset to EBP. Otherwise there isn't an easy way to figure where a local variable is at any point in code.

As Greg mentioned, it also helps stack unwinding for a debugger since EBP provides a reverse linked list of stack frames therefore letting the debugger figure out the size of a stack frame (local variables + arguments) of the function.

Most compilers provide an option to omit frame pointers although it makes debugging really hard. That option should never be used globally, even in release code. You don't know when you'll need to debug a user's crash.

---------------------------------------------------------------

## Understanding Frame Pointers
Each function has local memory associated with it to hold incoming parameters, local variables, and (in some cases) temporary variables. This region of memory is called a stack frame and is allocated on the process’ stack. A frame pointer (the ebp register on intel x86 architectures, rbp on 64-bit architectures) contains the base address of the function’s frame. The code to access local variables within a function is generated in terms of offsets to the frame pointer. The stack pointer (the esp register on intel x86 architectures or rsp on 64-bit architectures) may change during the execution of a function as values are pushed or popped off the stack, such as pushing parameters in preparation to calling another function. The frame pointer doesn’t change throughout the function.

Here’s what happens in a function call (there might be slight differences among languages/architectures):

* Push the current value of the frame pointer (ebp/rbp). This saves it so we can restore it later.

* Move the current stack pointer to the frame pointer. This defines the start of the frame.

* Subtract the space needed for the function’s data from the stack pointer. Remember that stacks grow from high memory to low memory. This puts the stack pointer past the space that will be used by the function so that anything pushed onto the stack now will not overwrite useful values.

* Now execute the code for the function. References to local variables will be negative offsets to the frame pointer (e.g., "movl $123, -8(%rbp)”).

* On exit from the function, copy the value from the frame pointer to the stack pointer (this clears up the space allocated to the stack frame for the function) and pop the old frame pointer. This is accomplished by the “leave” instruction.

* Return from the procedure via a “ret” instruction. This pops the return value from the stack and transfers execution to that address.

---------------------------------------------------------------

## What is Stack Memory?
Stack memory is often referred to as LIFO (last in, first out). Think of this as a bunch of building bricks stacked on top of one another, you can’t take from the middle as the stack will fall so the last brick placed on top has to first be removed. This is how the stack works.

In a previous article, I explained what the registers are in x64dbg and also some basic assembly instructions. This information is needed to understand how the stack operates. When new data is added to the stack, the malware will use the ‘PUSH’ command. To remove an item off the stack, the malware will use the ‘POP’ command. Data can also be popped off the stack and into a register.

The register ‘ESP’ is used to point to the next item on the stack and is referred to as the ‘stack pointer’.

EBP aka the ‘frame pointer’ serves as an unchanging reference point for data on the stack. This allows the program to work out how far away something in the stack is from this point. So if a variable is two ‘building blocks’ away then it is [EBP+8] as each ‘block’ in the stack is 4 bytes.

Each function in a program will generate its own stack frame to reference its own variables and parameters using this technique.

## Stack Memory Architecture
The following diagram should help illustrate how the stack is laid out like a set of building blocks:

![](https://www.varonis.com/hubfs/Imported_Blog_Media/stack-memory-architecture-1-1200x1253.png)

Lower memory addresses are on top and higher memory addresses are on the bottom.

Each function will generate its own stack frame, so the stack frame in the example above could be on top of another frame on the stack which is being used for another function.

EBP as mentioned earlier is stored as an unchanging reference point on the stack, this is done by moving the value of ESP (the stack pointer) into EBP. We do this as ESP will change as it always points to the top of the stack, storing it in EBP gives us an unchanging reference point in the stack and now the function can reference its variables and parameters on the stack from this location.

In this example parameters that have been passed to the function are stored in ‘[EBP]+8’, ‘[EBP]+12’ and ‘[EBP] +16’. So when we see ‘[EBP]+8’ that is the distance on the stack from EBP.

Variables will be stored after the function has begun executing so these will be stored higher up the stack but in lower address space, so in this example, this will be shown as ‘[EBP]-4’.

![](https://www.varonis.com/hubfs/Imported_Blog_Media/function-prologue-components-1-1200x615.png)

push ebp preserves ESP, the previous stack frame pointer, this is so it can be returned to at the end of the function. A stack frame is used to store local variables and each function will have its own stack frame in memory.

mov ebp, esp moves the current stack position into EBP which is the base of the stack. We now have a reference point that allows us to reference our local variables stored on the stack. The value of EBP now never changes.

sub esp, 10 grows the stack by 16 bytes (10 in hex) to allocate space on the stack for any variables we need to reference.

-----------------------------------------------------------

# The Role of EBP and ESP
* EBP (Base Pointer): Acts as the frame pointer, providing a constant reference point to the base of the current stack frame. This allows consistent access to function parameters and local variables using offsets from EBP throughout the function's lifetime.

* ESP (Stack Pointer): Points to the top of the stack. It's dynamic, constantly adjusting as data is pushed onto or popped off the stack. This manages the stack's growth and contraction during function operations.

## Function Call Breakdown
* Parameter Passing: When a function is called, parameters (e.g., c, b, a) are pushed onto the stack.

* Return Address: The return address, indicating where execution should resume after the function completes, is pushed next.

* Saving the Old EBP: The current EBP value is saved onto the stack, creating a crucial link to the previous stack frame. This is vital for debugging and call tracing.

* Establishing the New EBP: The ESP value is copied to EBP, establishing the new frame pointer for the current function.

* Local Variable Allocation: Space for local variables (e.g., xx, yy, zz, sum) is allocated below the new EBP. These variables are accessed using negative offsets from EBP (e.g., EBP - 4 for xx, EBP - 8 for yy, etc.).

## Function Return Process
* Freeing Local Variable Space: ESP is adjusted upwards to deallocate the space occupied by local variables.

* Retrieving the Return Address: The return address is popped off the stack, guiding the CPU back to the calling function.

* Restoring the Previous EBP: The saved EBP value is popped back into EBP, effectively dismantling the current frame and restoring the previous one.

![](https://payatu.com/wp-content/uploads/2018/01/func_stack_frame.jpg)

1. ![](https://people.cs.rutgers.edu/~pxk/419/notes/images/frames-1.png)
2. ![](https://people.cs.rutgers.edu/~pxk/419/notes/images/frames-2.png)
3. ![](https://people.cs.rutgers.edu/~pxk/419/notes/images/frames-3.png)
4. ![](https://people.cs.rutgers.edu/~pxk/419/notes/images/frames-4.png)