# Lvalues and Rvalues
Lvalues and rvalues are fundamental to C++ expressions. Put simply, an lvalue is an object reference and an rvalue is a value. The difference between lvalues and rvalues plays a role in the writing and understanding of expressions.

An lvalue is an expression that yields an object reference, such as a variable name, an array subscript reference, a dereferenced pointer, or a function call that returns a reference. An lvalue always has a defined region of storage, so you can take its address.

An rvalue is an expression that is not an lvalue. Examples of rvalues include literals, the results of most operators, and function calls that return nonreferences. An rvalue does not necessarily have any storage associated with it.

Strictly speaking, a function is an lvalue, but the only uses for it are to use it in calling the function, or determining the function’s address. Most of the time, the term lvalue means object lvalue, and this book follows that convention.

C++ borrows the term lvalue from C, where only an lvalue can be used on the left side of an assignment statement. The term rvalue is a logical counterpart for an expression that can be used only on the righthand side of an assignment. For example:
```
#define rvalue 42
int lvalue;
lvalue = rvalue;
```
In C++, these simple rules are no longer true, but the names remain because they are close to the truth. The most significant departure from traditional C is that an lvalue in C++ might be const, in which case it cannot be ...

--------------------------------
--------------------------------
--------------------------------
--------------------------------
--------------------------------

