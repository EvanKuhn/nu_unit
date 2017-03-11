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
  nu_check(1 == 1); // Does not run
}

void misc_nu_methods_suite() {
  nu_run_test(test_not_implemented);
  nu_run_test(test_nu_fail);
  nu_run_test(test_nu_abort);
  nu_run_test(test_nu_check);
  nu_run_test(test_nu_assert);
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
