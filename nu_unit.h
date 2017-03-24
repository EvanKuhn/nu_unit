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
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

//------------------------------------------------------------------------------
// Global variables, constants, etc
//------------------------------------------------------------------------------

// Version
#define NU_VERSION "1.0.3"

// Constants
#define NU_TEST_OUTPUT 't'
#define NU_SUITE_OUTPUT 's'
#define NU_SUITE_BUFLEN 128

// Enum and operator strings for comparison macros
typedef enum nu_op_e {
    NU_OP_EQ,
    NU_OP_NE,
    NU_OP_LT,
    NU_OP_LE,
    NU_OP_GT,
    NU_OP_GE
} nu_op_t;

static const char* NU_OPNAMES[6] = {
  "==",
  "!=",
  "<",
  "<=",
  ">",
  ">=",
};

// Internal counters
extern int nu_num_checks;
extern int nu_num_asserts;
extern int nu_num_failures;
extern int nu_num_not_impl;
extern int nu_prev_failures;
extern int nu_prev_not_impl;
extern char nu_output_level;
extern char nu_target_suite[NU_SUITE_BUFLEN];
extern bool nu_use_color;
extern char* NOCOLOR;
extern char* RED;
extern char* YELLOW;
extern char* GREEN;
extern char* nu_test_indent;
extern char* nu_msg_indent;
extern char   nu_outbuf[8192];
extern char*  nu_outbuf_ptr;
extern size_t nu_outbuf_free;

// Initialize the test counters. Call this above your main() function.
#define nu_init() \
  int nu_num_checks = 0; \
  int nu_num_asserts = 0; \
  int nu_num_failures = 0; \
  int nu_num_not_impl = 0; \
  int nu_prev_failures = 0; \
  int nu_prev_not_impl = 0; \
  char nu_output_level = NU_TEST_OUTPUT; \
  char nu_target_suite[NU_SUITE_BUFLEN]; \
  bool nu_use_color = false; \
  char* NOCOLOR = ""; \
  char* RED     = ""; \
  char* YELLOW  = ""; \
  char* GREEN   = ""; \
  char* nu_test_indent = ""; \
  char* nu_msg_indent = ""; \
  char nu_outbuf[8192] = {0}; \
  char* nu_outbuf_ptr = nu_outbuf; \
  size_t nu_outbuf_free = sizeof(nu_outbuf)

//------------------------------------------------------------------------------
// Utilities
//------------------------------------------------------------------------------

// Save counters so we can determine the status of a test by comparing counters
// before and after
static void _nu_save_counters()
{
  nu_prev_failures = nu_num_failures;
  nu_prev_not_impl = nu_num_not_impl;
}

// Determine the status of a test by comparing counters before and after.
// Return the color string, since that's what we're after.
static char* _nu_test_status_color()
{
  if (nu_num_failures > nu_prev_failures) return RED;
  if (nu_num_not_impl > nu_prev_not_impl) return YELLOW;
  return GREEN;
}

static void _nu_outbuf_append(const char* format, ...)
{
  va_list args;
  va_start(args, format);
  size_t n = vsnprintf(nu_outbuf_ptr, nu_outbuf_free, format, args);
  nu_outbuf_ptr += n;
  nu_outbuf_free = (n > nu_outbuf_free ? 0 : nu_outbuf_free - n);
  va_end(args);
}

//------------------------------------------------------------------------------
// Testing macros
//------------------------------------------------------------------------------

// Print a message and increment the not-implemented counter
#define nu_not_implemented() \
  do { \
    ++nu_num_not_impl; \
    _nu_outbuf_append("%s%s- %s:%i test not implemented%s\n", \
      nu_msg_indent, YELLOW, __FILE__, __LINE__, NOCOLOR); \
  } while(0)

// Log a failure with nu_unit. This will:
// - Increment the failure counter
// - Print the error message
#define nu_fail(msg) \
  do { \
    _nu_outbuf_append("%s%s- %s%s\n", nu_msg_indent, RED, msg, NOCOLOR); \
    ++nu_num_failures; \
  } while(0)

// Log a failure with nu_unit, and abort the current test. This will:
// - Increment the failure counter
// - Print the error message
// - Return from the current test function
#define nu_abort(msg) \
  do { \
    _nu_outbuf_append("%s%s- %s%s\n", nu_msg_indent, RED, msg, NOCOLOR); \
    ++nu_num_failures; \
    return; \
  } while(0)

// Check that some expression is true. If not:
// - Increment the failure counter
#define nu_check(expr) \
  do { \
    ++nu_num_checks; \
    if(!(expr)) { \
      _nu_outbuf_append("%s%s- %s:%i nu_check(%s) failed%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #expr, NOCOLOR); \
      ++nu_num_failures; \
    } \
  } while(0)

// Assert that some expression is true. If not:
// - Increment the failure counter
// - Exit the current test function
#define nu_assert(expr) \
  do { \
    ++nu_num_asserts; \
    if(!(expr)) { \
      _nu_outbuf_append("%s%s- %s:%i nu_assert(%s) failed%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #expr, NOCOLOR); \
      ++nu_num_failures; \
      return; \
    } \
  } while(0)

// Check that some expression is true. If not:
// - Increment the failure counter
#define nu_check_true(expr) \
  do { \
    ++nu_num_checks; \
    if(!(expr)) { \
      _nu_outbuf_append("%s%s- %s:%i nu_check_true(%s) failed%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #expr, NOCOLOR); \
      ++nu_num_failures; \
    } \
  } while(0)

// Check that some expression is false. If not:
// - Increment the failure counter
#define nu_check_false(expr) \
  do { \
    ++nu_num_checks; \
    if((expr)) { \
      _nu_outbuf_append("%s%s- %s:%i nu_check_false(%s) failed%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #expr, NOCOLOR); \
      ++nu_num_failures; \
    } \
  } while(0)

// Check that some expression evaluates to null. If not:
// - Increment the failure counter
#define nu_check_null(expr) \
  do { \
    ++nu_num_checks; \
    if((expr) != NULL) { \
      _nu_outbuf_append("%s%s- %s:%i nu_check_null(%s) failed%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #expr, NOCOLOR); \
      ++nu_num_failures; \
    } \
  } while(0)

// Check that some expression evaluates to not null. If not:
// - Increment the failure counter
#define nu_check_not_null(expr) \
  do { \
    ++nu_num_checks; \
    if((expr) == NULL) { \
      _nu_outbuf_append("%s%s- %s:%i nu_check_not_null(%s) failed%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #expr, NOCOLOR); \
      ++nu_num_failures; \
    } \
  } while(0)

static inline void _nu_check_int_helper(char* macro, int a, char* a_name, int b, char* b_name,
  nu_op_t op, char* file, int line)
{
  ++nu_num_checks;
  bool status = false;

  switch (op) {
    case NU_OP_EQ: status = (a == b); break;
    case NU_OP_NE: status = (a != b); break;
    case NU_OP_LT: status = (a <  b); break;
    case NU_OP_LE: status = (a <= b); break;
    case NU_OP_GT: status = (a >  b); break;
    case NU_OP_GE: status = (a >= b); break;
  }

  if (!status) {
    ++nu_num_failures;
    _nu_outbuf_append("%s%s- %s:%i %s(%s, %s) failed: (%d %s %d) is false%s\n",
      nu_msg_indent, RED, file, line, macro, a_name, b_name, a, NU_OPNAMES[op], b, NOCOLOR);
  }
}

#define nu_check_int_eq(a, b) \
  do { _nu_check_int_helper("nu_check_int_eq", a, #a, b, #b, NU_OP_EQ, __FILE__, __LINE__); } while(0)

#define nu_check_int_ne(a, b) \
  do { _nu_check_int_helper("nu_check_int_ne", a, #a, b, #b, NU_OP_NE, __FILE__, __LINE__); } while(0)

#define nu_check_int_lt(a, b) \
  do { _nu_check_int_helper("nu_check_int_lt", a, #a, b, #b, NU_OP_LT, __FILE__, __LINE__); } while(0)

#define nu_check_int_le(a, b) \
  do { _nu_check_int_helper("nu_check_int_le", a, #a, b, #b, NU_OP_LE, __FILE__, __LINE__); } while(0)

#define nu_check_int_gt(a, b) \
  do { _nu_check_int_helper("nu_check_int_gt", a, #a, b, #b, NU_OP_GT, __FILE__, __LINE__); } while(0)

#define nu_check_int_ge(a, b) \
  do { _nu_check_int_helper("nu_check_int_ge", a, #a, b, #b, NU_OP_GE, __FILE__, __LINE__); } while(0)

// Yuck. Copy-and-paste the integer function for floats.
static inline void _nu_check_flt_helper(char* macro, float a, char* a_name, float b, char* b_name,
  nu_op_t op, char* file, int line)
{
  ++nu_num_checks;
  bool status = false;

  switch (op) {
    case NU_OP_EQ: status = (a == b); break;
    case NU_OP_NE: status = (a != b); break;
    case NU_OP_LT: status = (a <  b); break;
    case NU_OP_LE: status = (a <= b); break;
    case NU_OP_GT: status = (a >  b); break;
    case NU_OP_GE: status = (a >= b); break;
  }

  if (!status) {
    ++nu_num_failures;
    _nu_outbuf_append("%s%s- %s:%i %s(%s, %s) failed: (%f %s %f) is false%s\n",
      nu_msg_indent, RED, file, line, macro, a_name, b_name, a, NU_OPNAMES[op], b, NOCOLOR);
  }
}

#define nu_check_flt_eq(a, b) \
  do { _nu_check_flt_helper("nu_check_flt_eq", a, #a, b, #b, NU_OP_EQ, __FILE__, __LINE__); } while(0)

#define nu_check_flt_ne(a, b) \
  do { _nu_check_flt_helper("nu_check_flt_ne", a, #a, b, #b, NU_OP_NE, __FILE__, __LINE__); } while(0)

#define nu_check_flt_lt(a, b) \
  do { _nu_check_flt_helper("nu_check_flt_lt", a, #a, b, #b, NU_OP_LT, __FILE__, __LINE__); } while(0)

#define nu_check_flt_le(a, b) \
  do { _nu_check_flt_helper("nu_check_flt_le", a, #a, b, #b, NU_OP_LE, __FILE__, __LINE__); } while(0)

#define nu_check_flt_gt(a, b) \
  do { _nu_check_flt_helper("nu_check_flt_gt", a, #a, b, #b, NU_OP_GT, __FILE__, __LINE__); } while(0)

#define nu_check_flt_ge(a, b) \
  do { _nu_check_flt_helper("nu_check_flt_ge", a, #a, b, #b, NU_OP_GE, __FILE__, __LINE__); } while(0)

#define nu_check_str_eq(a, b) \
  do { \
    ++nu_num_checks; \
    if(strcmp(a,b)) { \
      _nu_outbuf_append("%s%s- %s:%i nu_check_str_eq(%s, %s) failed: (\"%s\" == \"%s\") is false%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #a, #b, a, b, NOCOLOR); \
      ++nu_num_failures; \
    } \
  } while(0)

#define nu_check_str_ne(a, b) \
  do { \
    ++nu_num_checks; \
    if(!strcmp(a,b)) { \
      _nu_outbuf_append("%s%s- %s:%i nu_check_str_ne(%s, %s) failed: (\"%s\" != \"%s\") is false%s\n", \
        nu_msg_indent, RED, __FILE__, __LINE__, #a, #b, a, b, NOCOLOR); \
      ++nu_num_failures; \
    } \
  } while(0)

//------------------------------------------------------------------------------
// Core nu_unit functions and macros
//------------------------------------------------------------------------------

// Pointer to a test or suite function w signature: void func(void);
typedef void (*funcptr)(void);

// Run a test
#define nu_run_test(func) \
  do { \
    nu_run_test_named(func, #func); \
  } while(0)

static void nu_run_test_named(funcptr func, char* name)
{
  // Reset the output buffer
  memset(nu_outbuf, 0, sizeof(nu_outbuf));
  nu_outbuf_ptr = nu_outbuf;
  nu_outbuf_free = sizeof(nu_outbuf);

  // Run test and figure out the status color
  _nu_save_counters();
  func();
  char* color = _nu_test_status_color();

  // Print colorized output
  if(nu_output_level == NU_TEST_OUTPUT)
    printf("%s%stest: %s%s\n", nu_test_indent, color, name, NOCOLOR);
  printf("%s", nu_outbuf);
}

// Run a test suite
#define nu_run_suite(func) \
  do { \
    nu_run_suite_named(func, #func); \
  } while(0)

static void nu_run_suite_named(funcptr func, char* name)
{
  if(!*nu_target_suite || !strcmp(nu_target_suite, name)) {
    printf("suite: %s\n", name);
    func();
    if(nu_output_level == NU_TEST_OUTPUT) printf("\n");
  }
}

// Print a summary of the testing events
void nu_print_summary()
{
  int failure = (nu_num_failures || (!nu_num_checks && !nu_num_asserts));
  char* color = (failure ? RED : GREEN);
  char* status = (failure ? "FAILURE" : "SUCCESS");
  printf("%i checks, %i asserts, %i failures, %i not implemented\n", \
    nu_num_checks, nu_num_asserts, nu_num_failures, nu_num_not_impl);
  printf("%s%s%s\n", color, status, NOCOLOR);
}

// Exit with success or failure depending on the number of failures
#define nu_exit() \
  exit(!(nu_num_failures || (!nu_num_checks && !nu_num_asserts)))
;

// Print usage info. Used by nu_parse_cmdline().
static void nu_print_usage(const char* program)
{
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
static void nu_parse_cmdline(int argc, char** argv)
{
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
