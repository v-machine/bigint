#ifndef SUNIT_H
#define SUNIT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void (*TestFuncPtr) (void);

/* =========================================================================
 * 							 	 TEST SETUP
 * ========================================================================= */
void set_up();
void tear_down();

/* =========================================================================
 * 							   TEST PARAMETERS
 * ========================================================================= */

void set_bail_on_fail();

/* =========================================================================
 * 							 	 TEST RUNNER
 * ========================================================================= */

void run_test(TestFuncPtr t);
void run_all_tests(TestFuncPtr t, ...);

/* =========================================================================
 * 							 	  ASSERTIONS
 * ========================================================================= */

bool assert_true(bool actual);
bool assert_false(bool actual);
bool assert_int_eq(int64_t expected, int64_t actual);
bool assert_str_eq(char* expected, char* actual);
bool assert_uint32_arr_eq(uint32_t* expected, uint32_t* actual, size_t size_expected, size_t size_actual);

#include "sunittest_internal.h"

#endif /* SUNIT_H */
