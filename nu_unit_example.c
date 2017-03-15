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

#include "nu_unit.h"

//==============================================================================
// Integer comparisons
//==============================================================================

void test_good_int_comparisons() {
  int five = 5;
  nu_check_int_eq(five, 5);
  nu_check_int_ne(five, 1);
  nu_check_int_lt(five, 6);
  nu_check_int_le(five, 5);
  nu_check_int_le(five, 6);
  nu_check_int_gt(five, 4);
  nu_check_int_ge(five, 4);
  nu_check_int_ge(five, 5);
}

void test_bad_int_comparisons() {
  int five = 5;
  nu_check_int_eq(five, 10);
  nu_check_int_ne(five, 5);
  nu_check_int_lt(five, 4);
  nu_check_int_lt(five, 5);
  nu_check_int_le(five, 4);
  nu_check_int_gt(five, 6);
  nu_check_int_gt(five, 5);
  nu_check_int_ge(five, 6);
}

void int_comparison_suite() {
  nu_run_test(test_good_int_comparisons);
  nu_run_test(test_bad_int_comparisons);
}

//==============================================================================
// Floating-point comparisons
//==============================================================================

void test_good_float_comparisons() {
  float five = 5.0;
  nu_check_flt_eq(five, 5.0);
  nu_check_flt_ne(five, 1.1);
  nu_check_flt_lt(five, 5.5);
  nu_check_flt_le(five, 5.0);
  nu_check_flt_le(five, 6.2);
  nu_check_flt_gt(five, 4.0);
  nu_check_flt_ge(five, 4.0);
  nu_check_flt_ge(five, 5.0);
}

void test_bad_float_comparisons() {
  float five = 5.0;
  nu_check_flt_eq(five, 5.1);
  nu_check_flt_ne(five, 5.0);
  nu_check_flt_lt(five, 2.5);
  nu_check_flt_lt(five, 5.0);
  nu_check_flt_le(five, 4.2);
  nu_check_flt_gt(five, 7.1);
  nu_check_flt_gt(five, 5.0);
  nu_check_flt_ge(five, 6.0);
}

void float_comparison_suite() {
  nu_run_test(test_good_float_comparisons);
  nu_run_test(test_bad_float_comparisons);
}

//==============================================================================
// String comparisons
//==============================================================================

void test_good_string_comparisons() {
  char* foo = "foo";
  nu_check_str_eq(foo, "foo");
  nu_check_str_ne(foo, "bar");
}

void test_bad_string_comparisons() {
  char* foo = "foo";
  nu_check_str_eq(foo, "bar");
  nu_check_str_ne(foo, "foo");
}

void string_comparison_suite() {
  nu_run_test(test_good_string_comparisons);
  nu_run_test(test_bad_string_comparisons);
}

//==============================================================================
// Miscellaneous nu functionality
//==============================================================================

void test_not_implemented() {
  nu_not_implemented();
}

void test_nu_fail() {
  nu_fail("This error message will be printed");
  nu_fail("...and so will this one");
}

void test_nu_abort() {
  nu_abort("This error message will be printed");
  nu_abort("...but this one will not");
}

void test_nu_check() {
  nu_check(1 == 2); // Runs, fails
  nu_check(1 == 1); // Runs
}

void test_nu_assert() {
  nu_assert(1 == 2); // Runs, fails
  nu_check(1 == 1);  // Does not run
}

void test_nu_check_true() {
  nu_check_true(1 == 1); // Succeeds
  nu_check_true(1 == 2); // Fails
}

void test_nu_check_false() {
  nu_check_false(1 == 1); // Fails
  nu_check_false(1 == 2); // Succeeds
}

void test_nu_check_null() {
  char* i_am_null = NULL;
  char* not_null = "i'm not null";
  nu_check_null(i_am_null);
  nu_check_null(not_null);
}

void test_nu_check_not_null() {
  char* i_am_null = NULL;
  char* not_null = "i'm not null";
  nu_check_not_null(i_am_null);
  nu_check_not_null(not_null);
}

void misc_nu_methods_suite() {
  nu_run_test(test_not_implemented);
  nu_run_test(test_nu_fail);
  nu_run_test(test_nu_abort);
  nu_run_test(test_nu_check);
  nu_run_test(test_nu_assert);
  nu_run_test(test_nu_check_true);
  nu_run_test(test_nu_check_false);
  nu_run_test(test_nu_check_null);
  nu_run_test(test_nu_check_not_null);
}

//==============================================================================
// Main
//==============================================================================

// Don't forget to initialize nu_unit!
nu_init();

int main(int argc, char **argv) {
  // Parse command-line options accepted by nu_unit
  nu_parse_cmdline(argc, argv);

  // Run test suites
  nu_run_suite(int_comparison_suite);
  nu_run_suite(float_comparison_suite);
  nu_run_suite(string_comparison_suite);
  nu_run_suite(misc_nu_methods_suite);
  // add more test suites here...

  // Print results and return
  nu_print_summary();
  nu_exit();
}
