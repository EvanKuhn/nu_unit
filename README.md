## nu_unit: simple C unit-testing framework

nu_unit is a simple unit-testing framework for C. It provides basic unit-testing
functionality and is small enough to learn in just a few minutes. It's a single
.h file, so it requires no compiling; just include it directly in your source code.

nu_unit supports:
- unit tests and test suites
- colorized output
- checks and asserts
- marking tests not-implemented
- helpful error messages including: filename, line, input expression and values
- basic statistic tracking and reporting
- control via command-line options

## Using nu_unit

An example will illustrate nu_unit best:

```c
#include "nu_unit.h"

// Define a basic unit-test, which is just a function
void test_something_simple() {
  nu_check(1 < 2);
}

// Show a few different error messages
void test_int_comparisons() {
  int five = 5;
  nu_check_int_eq(five, 5);
  nu_check_int_ne(five, 5); // Prints an error with input values
  nu_assert(2 < 1);         // Prints an error with original expression
  nu_check(2 < 1);          // Won't be run since the assert above failed
}

// This test hasn't been implemented yet
void test_not_implemented() {
  nu_not_implemented(); // We can write the actual test later
}

// Define a test-suite consisting of the two unit tests above
void example_test_suite() {
  nu_run_test(test_something_simple);
  nu_run_test(test_int_comparisons);
  nu_run_test(test_not_implemented);
}

// We need to initialize nu_unit before running our tests
nu_init();

int main(int argc, char **argv) {
  // Parse command-line options accepted by nu_unit
  nu_parse_cmdline(argc, argv);

  // Run test suites
  nu_run_suite(example_test_suite);
  // add more test suites here...

  // Print results and return
  nu_print_summary();
  nu_exit();
}
```

A test typically consists of calls to `nu_check()`, `nu_assert()`, or various
comparison macros like `nu_check_int_eq()`. Upon failure, these macros will
print an error message and increment the failure count. A `check` will allow the
function to continue, while an `assert` will cause it to return.

In nu_unit, all tests and test suites are functions of the form `void func()`.
Tests and suites are run via the `nu_run_test()` and `nu_run_suite()` macros,
respectively.

To run your tests, you'll first have to initialize nu_unit via `nu_init()`.
Then in your main:
- parse command-line args.
- run your tests/suites.
- print the summary info (number of checks, asserts, failures, etc).
- exit with success or failure.

The example program above will print:

```
suite: example_test_suite
  test: test_something_simple
  test: test_int_comparisons
    - small_example.c:12 nu_check_int_ne(five, 5) failed: (5 != 5) is false
    - small_example.c:13 nu_assert(2 < 1) failed
  test: test_not_implemented
    - small_example.c:19 test not implemented

3 checks, 1 asserts, 2 failures, 1 not implemented
FAILURE
```

Take a look at the `nu_unit_example.c` file for an example with multiple test
suites, utilizing all nu_unit functionality.

## Command Line Arguments

The `nu_parse_cmdline()` function adds basic command-line argument parsing,
allowing you to run a specific suite and control the output level.

Use `-h` to print usage info:

```
> ./example -h
USAGE:
  ./nu_unit_example [options]

OPTIONS:
  -l <level>   Output level. Accepts: 't', 's', 'test', 'suite'.
  -s <suite>   Test suite to run. By default, all suites are run.
  -c           Enable colorized output.
  -v           Print the nu_unit version and exit.
  -h           Show this usage info.
```

The `-l` option allows you to output test-level or suite-level data. Here we
print each test that's run:

```
> ./example -l t
suite: http_test_suite
test: test_http_get
test: test_http_post

suite: ftp_test_suite
test: test_ftp_get
test: test_ftp_post

0 checks, 4 asserts, 0 failures, 0 not implemented
SUCCESS
```

And here we just print the suite names:

```
> ./example -l s
suite: http_test_suite
suite: ftp_test_suite
0 checks, 4 asserts, 0 failures, 0 not implemented
SUCCESS
```

Errors will always be printed, regardless of the output level.

To run just a single suite, use the `-s` option:

```
> ./example -s http_test_suite
suite: http_test_suite
test: test_http_get
test: test_http_post

0 checks, 2 asserts, 0 failures, 0 not implemented
SUCCESS
```

## Reference

nu_unit is composed of a number of macros. Outside of your tests, use:

- `nu_init()`                - Initialize nu_unit. Call before entering your
                               `main()` function.
- `nu_parse_cmdline()`       - Parse command-line arguments to allow the user to
                               run a specific suite or alter the output level.
- `nu_run_test(func)`        - Run a test function. Print its name to stdout.
- `nu_run_suite(func)`       - Run a test-suite function. Print its name to stdout.
- `nu_print_summary()`       - Print the statistics collected during testing:
                               number of checks, asserts, failures, and tests not
                               implemented.
- `nu_exit()`                - Exit the program via the C `exit()` system call.
                               Use a return value of 1 if any checks or asserts
                               failed.

Inside your tests, use these macros:

- `nu_not_implemented()`     - Mark a test as not-implemented, and print a message
                               to stdout.
- `nu_fail(msg)`             - Print an error message and log a failure.
- `nu_abort(msg)`            - Print an error message, log a failure, and return
                               from the calling function.
- `nu_check(expr)`           - Check that some expression is true. If not, print
                               an error message and record a failure
- `nu_assert(expr)`          - Assert that some expression is true. If not, print
                               an error message, record a failure, and return from
                               from the calling function.
- `nu_check_int_eq(a,b)`     - Check integer a == b
- `nu_check_int_ne(a,b)`     - Check integer a != b
- `nu_check_int_lt(a,b)`     - Check integer a <  b
- `nu_check_int_le(a,b)`     - Check integer a <= b
- `nu_check_int_gt(a,b)`     - Check integer a >  b
- `nu_check_int_ge(a,b)`     - Check integer a >= b
- `nu_check_flt_eq(a,b)`     - Check float a == b
- `nu_check_flt_ne(a,b)`     - Check float a != b
- `nu_check_flt_lt(a,b)`     - Check float a <  b
- `nu_check_flt_le(a,b)`     - Check float a <= b
- `nu_check_flt_gt(a,b)`     - Check float a >  b
- `nu_check_flt_ge(a,b)`     - Check float a >= b
- `nu_check_str_eq(a,b)`     - Check string a == b
- `nu_check_str_ne(a,b)`     - Check string a != b

Note: nu_unit considers it a failure if no checks or asserts are performed.

## Acknowledgements

nu_unit was originall based on the ultra-simple [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html),
which consists of nothing more than two macros. nu_unit is customized to include
additional functionality and to automate some of the mundane tasks that MinUnit
leaves up to the programmer. I hope you like it!
