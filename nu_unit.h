//==============================================================================
// nu_unit: C unit-testing framework based on MinUnit
//==============================================================================
#include <stdio.h>
#include <stdlib.h>

//TODO - support nu_output_level() setter w/ values 'tests' and 'suites'

// Internal counters
extern int nu_num_checks;
extern int nu_num_asserts;
extern int nu_num_failures;
extern int nu_num_not_impl;

// Initialize the test counters. Call this above your main() function.
#define nu_init() \
  int nu_num_checks = 0; \
  int nu_num_asserts = 0; \
  int nu_num_failures = 0; \
  int nu_num_not_impl = 0

// Check that some expression is true. If not:
// - Increment the failure counter
// - Print the error message
#define nu_check(msg, expr) \
  do { \
    ++nu_num_checks; \
    if(!(expr)) { \
      printf("- %s:%i check failed: %s\n", __FILE__, __LINE__, msg); \
      ++nu_num_failures; \
    } \
  } while(0)

// Assert that some expression is true. If not:
// - Increment the failure counter
// - Print the error message
// - Exit the current test function
#define nu_assert(msg, test) \
  do { \
    ++nu_num_asserts; \
    if(!(test)) { \
      printf("- %s:%i assert failed: %s\n", __FILE__, __LINE__, msg); \
      ++nu_num_failures; \
      return; \
    } \
  } while(0)

// Log a failure with nu_unit. This will:
// - Increment the failure counter
// - Print the error message
#define nu_fail(msg) \
  do { \
    printf("- %s\n", msg); \
    ++nu_num_failures; \
  } while(0)

// Log a failure with nu_unit, and abort the current test. This will:
// - Increment the failure counter
// - Print the error message
// - Return from the current test function
#define nu_abort(msg) \
  do { \
    printf("- %s\n", msg); \
    ++nu_num_failures; \
    return; \
  } while(0)

// Run a test
#define nu_run_test(test, name) \
  do { \
    printf("test: %s\n", name); \
    test(); \
  } while(0)

// Run a test suite
#define nu_run_suite(suite, name) \
  do { \
    printf("suite: %s\n", name); \
    suite(); \
    printf("\n"); \
  } while(0)

// Print a message and increment the not-implemented counter
#define nu_not_implemented() \
  do { \
    ++nu_num_not_impl; \
    printf("- test not implemented\n"); \
  } while(0)

// Print a summary of the testing events
#define nu_print_summary() \
  do { \
    printf("%i checks, %i asserts, %i failures, %i not implemented\n", \
      nu_num_checks, nu_num_asserts, nu_num_failures, nu_num_not_impl); \
    printf(nu_num_failures ? "FAILURE\n" : "SUCCESS\n"); \
  } while(0)

// Exit with success or failure depending on the number of failures
#define nu_exit() \
  exit(nu_num_failures ? 1 : 0)
