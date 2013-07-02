/*
Copyright (C) 2012 Evan Kuhn

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//==============================================================================
// nu_unit: simple C unit-testing framework
//==============================================================================
#ifndef NU_UNIT_H
#define NU_UNIT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define NU_TEST_OUTPUT 't'
#define NU_SUITE_OUTPUT 's'
#define NU_SUITE_BUFLEN 128

// Internal counters
extern int nu_num_checks;
extern int nu_num_asserts;
extern int nu_num_failures;
extern int nu_num_not_impl;
extern char nu_output_level;
extern char nu_target_suite[NU_SUITE_BUFLEN];

// Initialize the test counters. Call this above your main() function.
#define nu_init() \
  int nu_num_checks = 0; \
  int nu_num_asserts = 0; \
  int nu_num_failures = 0; \
  int nu_num_not_impl = 0; \
  char nu_output_level = 't'; \
  char nu_target_suite[NU_SUITE_BUFLEN]

// Set output level
#define nu_test_level_output() nu_output_level = NU_TEST_OUTPUT
#define nu_suite_level_output() nu_output_level = NU_SUITE_OUTPUT

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
#define nu_assert(msg, expr) \
  do { \
    ++nu_num_asserts; \
    if(!(expr)) { \
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
#define nu_run_test(func, name) \
  do { \
    if(nu_output_level == NU_TEST_OUTPUT) printf("test: %s\n", name); \
    func(); \
  } while(0)

// Run a test suite
#define nu_run_suite(func, name) \
  do { \
    if(!*nu_target_suite || !strcmp(nu_target_suite, name)) { \
      printf("suite: %s\n", name); \
      func(); \
      if(nu_output_level == NU_TEST_OUTPUT) printf("\n"); \
    } \
  } while(0)

// Print a message and increment the not-implemented counter
#define nu_not_implemented() \
  do { \
    ++nu_num_not_impl; \
    printf("- %s:%i test not implemented\n", __FILE__, __LINE__); \
  } while(0)

// Print a summary of the testing events
#define nu_print_summary() \
  do { \
    printf("%i checks, %i asserts, %i failures, %i not implemented\n", \
      nu_num_checks, nu_num_asserts, nu_num_failures, nu_num_not_impl); \
    int failure = (nu_num_failures || (!nu_num_checks && !nu_num_asserts)); \
    printf(failure ? "FAILURE\n" : "SUCCESS\n"); \
  } while(0)

// Exit with success or failure depending on the number of failures
#define nu_exit() \
  exit(!(nu_num_failures || (!nu_num_checks && !nu_num_asserts)))
;

// Print usage info. Used by nu_parse_cmdline().
static void nu_print_usage(const char* program) {
  if(strstr(program, "./")) program += 2;
  printf(
    "USAGE:\n"
    "  ./%s [options]\n"
    "\n"
    "OPTIONS:\n"
    "  -l <level>   Output level. Accepts: 't', 's', 'test', 'suite'.\n"
    "  -s <suite>   Test suite to run. By default, all suites are run.\n"
    "  -h           Show this usage info.\n"
    , program);
}

// Parse command-line args and configure nu_unit
static void nu_parse_cmdline(int argc, char** argv) {
  char c = 0;
  opterr = 0;

  while((c = getopt(argc, argv, "l:s:h")) != -1) {
    switch(c) {
      case 'l':
        if(!strcmp(optarg, "t") || !strcmp(optarg, "test")) {
          nu_output_level = 't';
        }
        else if(!strcmp(optarg, "s") || !strcmp(optarg, "suite")) {
          nu_output_level = 's';
        }
        else {
          fprintf(stderr, "Unknown output level '%s'\n", optarg);
          exit(1);
        }
        break;
      case 's':
        bzero(nu_target_suite, NU_SUITE_BUFLEN);
        snprintf(nu_target_suite, NU_SUITE_BUFLEN, "%s", optarg);
        break;
      case 'h':
        nu_print_usage(argv[0]);
        exit(0);
        break;
      case '?':
        if(strchr("ls", optopt)) {
          fprintf(stderr, "Option -%c requires an argument\n", optopt);
        }
        else {
          fprintf(stderr, "Unknown option '-%c'\n", optopt);
        }
        exit(1);
      default:
        abort();
    }
  }
}

#endif // NU_UNIT_H