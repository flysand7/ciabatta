
# assert.h - debug assertions

Macro definitions:

- [`assert(expr)`](#assert)
- [`static_assert`](#static-assert)

## [assert](#assert)

Assert a given condition is true, otherwise abort execution of a program.

<details><summary>Description</summary>

The macro checks whether `expr` is true, and if not, prints the
information about where the error occurs and then aborts execution in a way
equivalent to calling abort() function (See SIGABRT).

You can disable the assertion checks by defining NDEBUG macro.

</details>

<details><summary>Declaration</summary>

```c
#if defined(NDEBUG)
    #define assert(expr) ((void)0)
#else
    #define assert(expr) /* see description */
#endif
```
</details>

<details>
<summary>Example</summary>

```c
// Uncomment to disable assert
//#define NDEBUG
#include <assert.h>

int factorial(int n) {
    assert(n >= 0);
    if(n == 0) return 1;
    return n*factorial(n-1);
}

int main() {
    printf("Factorial of %d is %d\n", 10, factorial(10));
    printf("Factorial of %d is %d\n", -1, factorial(-1));
    return 0;
}
```

The first function would print 3628800, the second would trigger an assert.
Output:

```
Factorial of 10 is 3628800
Assertion failed: n >= 0
  Function: factorial
  File: test\test_assert.c
  Line: 4
Trace:
  4: factorial
  3: main
  2: mainCRTStartup
  1: BaseThreadInitThunk
  0: RtlUserThreadStart
```
</details>

## [static_assert](#static-assert)

Keyword macro that expands to C11 keyword `_Static_assert`.

<details>
<summary>Definition</summary>

```c
#define static_assert _Static_assert
```
</details>

