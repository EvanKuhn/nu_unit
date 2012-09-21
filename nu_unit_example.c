#include "nu_unit.h"

//==============================================================================
// Test functions
//==============================================================================
void test_foo() {
  int foo = 1;
  nu_assert("foo isn't 1", foo == 1);
}

void test_math_skills() {
  int bar = 2;
  nu_assert("bar isn't 2", bar == 2);
  nu_check("i can add", 1+1 == 2);
  nu_check("i can subtract", 5-3 == 1);
  nu_check("i can multiply", 7*11 == 77);
  nu_check("i can divide", 6/2 == 4);
}

void test_not_impl() {
  nu_not_implemented();
}

void test_cake() {
  int cake = 0;
  int healthy = 1;
  int weight = 200;
  nu_assert("i like food", 1);
  nu_assert("cake is healthy", cake == healthy);
  nu_check("my weight is ok", weight < 180);
}

//==============================================================================
// Test suites
//==============================================================================
void test_suite1() {
  nu_run_test(test_foo,      "foo");
  nu_run_test(test_not_impl, "procrastination");
}

void test_suite2() {
  nu_run_test(test_math_skills, "math skills");
  nu_run_test(test_cake,        "cake");
}

//==============================================================================
// Main
//==============================================================================
nu_init();

int main(int argc, char **argv) {
  // Run test suites
  nu_run_suite(test_suite1, "suite 1");
  nu_run_suite(test_suite2, "suite 2");
  // add more test suites here...

  // Print results and return
  nu_print_summary();
  nu_exit();
}
