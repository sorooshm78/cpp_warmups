## [Understanding Volatile in C++](https://medium.com/@cankeceoglu/understanding-volatile-in-c-c3aa37db1042)

In C++ programming, the volatile keyword is used to indicate to the compiler that a variable’s value may be changed by something outside the program’s control, such as hardware, interrupt or concurrent thread. The volatile keyword prevents the compiler from optimizing the code in a way that the variable does not change unexpectedly.

## Key Concepts of volatile in C++

1. Prevents Optimization: Normally, the compiler optimizes the code by assuming if a variable is not modified, its value won’t change. With volatile, compiler knows that the value could be changed by external (hardware or thread), so it won’t cache the variable in a register or optimize away repeated access.
2. Use in Embedded Systems: In systems where programs have interface with hardware, the value can be change by external events (by hardware signals or interrupts) In such case, volatile ensures that every time the variable is accessed, it retrieves the actual value, not a cached version.
3. Concurrency and Multithreading: In multithreaded environments, one thread might modify a variable that another thread read it. Declaring the variable as volatile ensures that thread sees the most recent value of the variable.

### Volatile example;
```
volatile int flag = 0;

void interrupt_handler()
{
  flag = 1;
}

int main()
{

  while(flag == 0)
  {
    
  }

  return 0;
}
```
Without the volatile keyword, compiler would optimize the while loop and assume flag will never change.

## Advantages of Using Volatile

1- Ensures Correct Program Behaviour: When dealing with hardware or multithreads, volatile ensure the correct behaviour by forcing compiler to avoid optimizations.
2- Safe Interaction with External Events: When variables are changed by external sources, using volatile ensures that program has latest value.