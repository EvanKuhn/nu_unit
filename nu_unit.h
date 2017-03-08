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
#include <stdbool.h>
#include <string.h>

// Version
#define NU_VERSION "0.0.2"

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
extern bool nu_use_color;
extern char* NOCOLOR;
extern char* RED;
extern char* YELLOW;
extern char* GREEN;
extern char* nu_test_indent;
extern char* nu_msg_indent;

// Initialize the test counters. Call this above your main() function.
#define nu_init() \
  int nu_num_checks = 0; \
  int nu_num_asserts = 0; \
  int nu_num_failures = 0; \
  int nu_num_not_impl = 0; \
  char nu_output_level = NU_TEST_OUTPUT; \
  char nu_target_suite[NU_SUITE_BUFLEN]; \
  bool nu_use_color = false; \
  char* NOCOLOR = ""; \
  char* RED     = ""; \
  char* YELLOW  = ""; \
  char* GREEN   = ""; \
  char* nu_test_indent = ""; \
  char* nu_msg_indent = ""

// Check that some expression is true. If not:
// - Increment the failure counter
// - Print the error message
#define nu_check(msg, expr) \
  do { \
    ++nu_num_checks; \
    if(!(expr)) { \
      printf("%s%s- %s:%i check failed: %s%s\n", nu_msg_indent, RED, __FILE__, __LINE__, msg, NOCOLOR); \
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
      printf("%s%s- %s:%i assert failed: %s%s\n", nu_msg_indent, RED, __FILE__, __LINE__, msg, NOCOLOR); \
      ++nu_num_failures; \
      return; \
    } \
  } while(0)

// Log a failure with nu_unit. This will:
// - Increment the failure counter
// - Print the error message
#define nu_fail(msg) \
  do { \
    printf("%s- %s%s\n", RED, msg, NOCOLOR); \
    ++nu_num_failures; \
  } while(0)

// Log a failure with nu_unit, and abort the current test. This will:
// - Increment the failure counter
// - Print the error message
// - Return from the current test function
#define nu_abort(msg) \
  do { \
    printf("%s- %s%s\n", RED, msg, NOCOLOR); \
    ++nu_num_failures; \
    return; \
  } while(0)

// Run a test
#define nu_run_test(func, name) \
  do { \
    if(nu_output_level == NU_TEST_OUTPUT) printf("%stest: %s\n", nu_test_indent, name); \
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
    printf("%s%s- %s:%i test not implemented%s\n", nu_msg_indent, YELLOW, __FILE__, __LINE__, NOCOLOR); \
  } while(0)

// Print a summary of the testing events
#define nu_print_summary() \
  do { \
    int failure = (nu_num_failures || (!nu_num_checks && !nu_num_asserts)); \
    char* color = (failure ? RED : GREEN); \
    printf("%s%i checks, %i asserts, %i failures, %i not implemented%s\n", \
      color, nu_num_checks, nu_num_asserts, nu_num_failures, nu_num_not_impl, NOCOLOR); \
    printf("%s%s%s\n", (failure ? RED : GREEN), (failure ? "FAILURE" : "SUCCESS"), NOCOLOR); \
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
    "  -c           Enable colorized output.\n"
    "  -v           Print the nu_unit version and exit.\n"
    "  -h           Show this usage info.\n"
    , program);
}

// Parse command-line args and configure nu_unit
static void nu_parse_cmdline(int argc, char** argv) {
  char c = 0;
  opterr = 0;

  while((c = getopt(argc, argv, "l:s:cvh")) != -1) {
    switch(c) {
      case 'l':
        if(!strcmp(optarg, "t") || !strcmp(optarg, "test")) {
          nu_output_level = NU_TEST_OUTPUT;
        }
        else if(!strcmp(optarg, "s") || !strcmp(optarg, "suite")) {
          nu_output_level = NU_SUITE_OUTPUT;
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
      case 'c':
        nu_use_color = true;
        break;
      case 'v':
        printf("nu_unit version %s\n", NU_VERSION);
        exit(0);
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

  // Set color constants
  if (nu_use_color) {
    NOCOLOR = "\x1B[0m";
    RED     = "\x1B[31m";
    GREEN   = "\x1B[32m";
    YELLOW  = "\x1B[33m";
  }

  // Set indent levels
  if (nu_output_level == NU_TEST_OUTPUT) {
    nu_test_indent = "  ";
    nu_msg_indent = "    ";
  }
}

#endif // NU_UNIT_H
