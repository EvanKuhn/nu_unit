## nu_unit: simple C unit-testing framework

nu_unit is a simple unit-testing framework for C. It provides basic unit-testing
functionality and is small enough to learn in just a few minutes. It's a single
.h file, so it requires no compiling; just include it directly in your
source code.

nu_unit supports:
- unit tests and test suites
- checks and asserts
- marking tests not-implemented
- custom error messages, with file name and line number
- basic statistic tracking and reporting
- control via command-line options

## Using nu_unit

An example will illustrate nu_unit best:

```c
#include "nu_unit.h"

// A basic unit-test function
void test_math() {
  nu_assert("i can add", 1+1 == 2);
  nu_check("i can divide", 323/17 == 8);  // Or can I...?
  nu_check("i can multiply", 5*3 == 15);
  // Add more checks and asserts
}

// This test hasn't been implemented yet
void test_literature() {
  nu_not_implemented(); // Maybe later...
}

// A test-suite function
void schoolwork_test_suite() {
  nu_run_test(test_math,       "Math");
  nu_run_test(test_literature, "Literature");
}

// We need to initialize nu_unit before running our tests
nu_init();

int main(int argc, char **argv) {
  // Parse command-line options accepted by nu_unit
  nu_parse_cmdline(argc, argv);

  // Run test suites
  nu_run_suite(schoolwork_test_suite, "SchoolWork");
  // add more test suites here...

  // Print results and return
  nu_print_summary();
  nu_exit();
}
```

A test typically consists of calls to `nu_check()` and `nu_assert()`. Upon
failure, both macros will print an error message and increment the failure
count. `nu_check()` will allow the function to continue, while `nu_assert()`
will cause it to return.

In nu_unit, all tests and test suites are functions of the form `void func()`.
Tests and suites are run via the `nu_run_test()` and `nu_run_suite()` macros,
respectively. Both of these allow you to name your tests and suites.

To run your tests, you'll first have to initialize nu_unit via `nu_init()`.
Then in your main:
- parse command-line args.
- run your tests/suites.
- print the summary info (number of checks, asserts, failures, etc).
- exit with success or failure.

The example program above will print:

```
suite: SchoolWork
test: Math
- example.c:6 check failed: i can divide
test: Literature
- example.c:13 test not implemented

2 checks, 1 asserts, 1 failures, 1 not implemented
FAILURE
```

Take a look at the `nu_unit_example.c` file for a slightly longer example.

## Reference

nu_unit is composed of the following macros:

- `nu_init()`                - Initialize nu_unit. Call before entering your
                               `main()` function.
- `nu_parse_cmdline()`       - Parse command-line arguments to allow the user to
                               run a specific suite or alter the output level.
- `nu_test_level_output()`   - Set test-level output (the default). Each test's
                               name will be printed.
- `nu_suite_level_output()`  - Set suite-level output. Test names will be omitted.
- `nu_check(msg, expr)`      - Check that some expression is true. If not, print
                               the message and record a failure.
- `nu_assert(msg, expr)`     - Assert that some expression is true. If not, print
                               the message, record a failure, and return from
                               from the calling function.
- `nu_fail(msg)`             - Print the message and log a failure.
- `nu_abort(msg)`            - Print the message, log a failure, and return from
                               the calling function.
- `nu_run_test(func, name)`  - Run a test function. Print the name to stdout.
- `nu_run_suite(func, name)` - Run a test-suite function. Print the name to stdout.
- `nu_not_implemented()`     - Mark a test as not-implemented, and print a message
                               to stdout.
- `nu_print_summary()`       - Print the statistics collected during testing:
                               number of checks, asserts, failures, and tests not
                               implemented.
- `nu_exit()`                - Exit the program via the C `exit()` system call.
                               Use a return value of 1 if any checks or asserts
                               failed.

## Acknowledgements

nu_unit is based on the ultra-simple [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html),
which consists of nothing more than two macros. nu_unit is customized to include
additional functionality and to automate some of the mundane tasks that MinUnit
leaves up to the programmer.

I hope you like it!

-- Evan Kuhn, 2012-09-22
