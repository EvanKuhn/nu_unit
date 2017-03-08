#include "nu_unit.h"

//==============================================================================
// Test functions
//==============================================================================

void test_good_numeric_comparisons() {
  nu_check_int_eq(1, 1);
  nu_check_int_eq(1000, 1000);
  nu_check_flt_eq(3.14, 3.14);
}

void test_bad_numeric_comparisons() {
  int one = 1;
  nu_check_int_eq(one, 2);
  nu_check_int_eq(1000, 2000);
  nu_check_flt_eq(3.14, 3.14159);
}

// A basic test with an assert that succeeds
void test_foo() {
  int foo = 1;
  nu_assert(foo == 1);
}

// This test contains a few checks that fail. Notice that a failed check does
// not cause the function to return.
void test_math_skills() {
  int bar = 2;
  nu_assert(bar == 2);
  nu_check(1+1 == 2);
  nu_check(5-3 == 1);          // This check fails
  nu_check(7*11 == 77);
  nu_check(6/2 == 4);          // And this check fails
}

// This test isn't implemented yet
void test_not_impl() {
  nu_not_implemented();
}

// Here we have an assert that fails, causing the function to return
void test_cake() {
  int cake = 0;
  int healthy = 1;
  int weight = 200;
  nu_assert(1);
  nu_assert(cake == healthy);  // This assert fails
  nu_check(weight < 180);      // This never runs
}

//==============================================================================
// Test suites
//==============================================================================

void test_suite1() {
  nu_run_test(test_good_numeric_comparisons);
  nu_run_test(test_bad_numeric_comparisons);
  nu_run_test(test_foo);
  nu_run_test(test_not_impl);
}

void test_suite2() {
  nu_run_test(test_math_skills);
  nu_run_test(test_cake);
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
  nu_run_suite(test_suite1);
  nu_run_suite_named(test_suite2, "suite2");
  // add more test suites here...

  // Print results and return
  nu_print_summary();
  nu_exit();
}
