The operators ++ and -- can be used as both **prefix** and **postfix** operators.

## Prefix (++x and --x)
- **First** changes the value of `x` (increments/decrements), **then** returns the new value.
- Example: `y = ++x`
  ```c
  // Equivalent to: y = (x = x + 1)
  // If x = 5:
  // x becomes 6, then y = 6
  ```

## Postfix (x++ and x--)
- **First** returns the **old** value of `x`, **then** changes the value of `x`.
- Example: `y = x++`
  ```c
  // Equivalent to: y = (t = x, x = x + 1, t)
  // t is a temporary variable; if x = 5:
  // y = 5 (old value), x becomes 6
  ```

## Complete Example
```c
int x = 5, y, z;

y = ++x;  // x=6, y=6 (prefix)
z = x++;  // z=6, x=7 (postfix)

printf("%d %d %d", x, y, z);  // 7 6 6
```

## Quick Test
```c
int a = 0;
printf("%d\n", ++a);  // 1 (a becomes 1 first, then prints)
printf("%d\n", a++);  // 1 (prints 1 first, then a becomes 2)
printf("%d\n", a);    // 2
```

## Why Postfix Needs `t`?
The compiler creates a **temporary copy** of `x`'s old value (that's `t`), returns it, then modifies `x`. This temporary is only needed for postfix; prefix doesn't need it.

**Key Note**: Postfix always has **higher cost** (due to copy), so use prefix (`++x`) unless you specifically need the old value.