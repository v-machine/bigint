#include "sunittest.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* =========================================================================
 * 							  GLOBAL TEST VARIABLE
 * ========================================================================= */

static int tests_run;
static int tests_failed;
static bool cur_tests_passed = true;
static bool bail_on_fail = false;

/* =========================================================================
 * 					PRIVATE TYPEDEF AND FUNCTION PROTOTYPES
 * ========================================================================= */

static void reset_test_param();
static void print_test_header(const char* test);
static void print_test_log(void);

/* =========================================================================
 * 									ASSERTIONS
 * ========================================================================= */

bool (assert_true) (bool actual)
{
	return actual;
}

bool (assert_false) (bool actual)
{
	return ! actual;
}

bool (assert_int_eq) (int64_t expected, int64_t actual)
{
	return (expected == actual);
}

bool (assert_str_eq) (char* expected, char* actual)
{
	if (strcmp(expected, actual)) return false;
	return true;
}

bool (assert_uint32_arr_eq) (uint32_t* expected, uint32_t* actual,
						    size_t size_expected, size_t size_actual)
{
	if (size_expected != size_actual) return false;
	for (int i = 0; i < size_expected; i++) {
		if (expected[i] != actual[i]) return false;
	}
	return true;
}

/* =========================================================================
 * 							 	 TEST RUNNER
 * ========================================================================= */

void (run_all_tests) (TestFuncPtr t, ...) {
	va_list tests;
	va_start(tests, t);

	run_test(t);
	while (t = va_arg(tests, TestFuncPtr)) {
		run_test(t);
	}
	print_test_log();	
}

void run_test(TestFuncPtr t) {
	set_up();
	reset_test_param();
	t();
	++tests_run;
	if (! cur_tests_passed && bail_on_fail) {
		printf("Aborting remaining tests.\n");
		exit(1);
	}
	tear_down();
}

/* =========================================================================
 * 							 	TEST UTILITY
 * ========================================================================= */

void set_bail_on_fail()
{
	bail_on_fail = true;
}

void reset_test_param()
{
	cur_tests_passed = true;
	bail_on_fail = false;
}

void raise_test_fail(const char* test_name)
{
	if (cur_tests_passed) {
		cur_tests_passed = false;
		print_test_header(test_name);
		++tests_failed;
	}
}

void print_module_header(const char* file_name)
{
	printf("TESTING MODULE: %s...\n", file_name);
}

void print_test_header(const char* test_name)
{
	printf("==================================================\n");
	printf("TESTING: %s...\n", test_name);
}

void print_test_log() {
	printf("----------------------------------\n");
	printf("TEST RESULT: (%d of %d) Passed.\n",
			tests_run - tests_failed, tests_run);
}
