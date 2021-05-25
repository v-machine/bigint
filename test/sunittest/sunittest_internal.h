#ifndef SUNIT_INT_H
#define SUNIT_INT_H
/* =========================================================================
 * 							    PRIVATE HEADERS
 * ========================================================================= */

typedef int (*AssertFuncPtr)();
void raise_test_fail(const char* test_name);
void print_module_header(const char* file_name);
	
#define run_all_tests(...) 																			\
{																									\
	print_module_header(__FILE__);																	\
	run_all_tests(__VA_ARGS__, NULL);																\
}																									\

#define assert_true(arg) assert_bool_wrapper(__func__, __LINE__, assert_true, repr, "%s", true, arg) 	
#define assert_false(arg) assert_bool_wrapper(__func__, __LINE__, assert_false, repr, "%s", false, arg) 	
#define assert_int_eq(...) assert_eq_wrapper(__func__, __LINE__, assert_int_eq, repr, "%d", __VA_ARGS__)
#define assert_str_eq(...) assert_eq_wrapper(__func__, __LINE__, assert_str_eq, repr, "%s", __VA_ARGS__)
#define assert_uint32_arr_eq(...) assert_arr_eq_wrapper(__func__, __LINE__, assert_uint32_arr_eq, repr_arr, "%u", __VA_ARGS__)

/* =========================================================================
 * 								ASSERTION WRAPPERS
 * ========================================================================= */

#define assert_bool_wrapper(test_name, line_num, assert_func, repr, format, expected, actual) 		\
{																									\
	if (! assert_func(actual)) {																	\
		raise_test_fail(test_name);																	\
		char* _actual  = expected == true ? "false" : "true";										\
		assert_failed_msg(repr, line_num, format, #expected, _actual);								\
	} 																								\
}

#define assert_eq_wrapper(test_name, line_num, assert_func, repr, format, expected, actual)			\
{																									\
	if (! assert_func(expected, actual)) {															\
		raise_test_fail(test_name);																	\
		assert_failed_msg(repr, line_num, format, expected, actual);								\
	} 																								\
}

#define assert_arr_eq_wrapper(test_name, line_num, assert_func, repr_arr, format,					\
		                      expected, actual, size_expected, size_actual)							\
{																									\
	if (! assert_func(expected, actual, size_expected, size_actual)) {								\
		raise_test_fail(test_name);																	\
		assert_arr_failed_msg(repr_arr, line_num, format, expected, actual,							\
							  size_expected, size_actual);											\
	} 																								\
}

/* =========================================================================
 * 								GENERIC REPRESENTATION
 * ========================================================================= */

#define repr(format, data) printf(format, data)
#define repr_arr(format, arr, size)																	\
{																									\
	printf("[");																					\
	for (int i = 0; i < size; i++) {																\
		printf(format, arr[i]);																		\
		if (i < size - 1) printf(", ");																\
	}																								\
	printf("]");																					\
}

/* =========================================================================
 * 								MESSAGE REPRESENTATION
 * ========================================================================= */

#define assert_failed_msg(repr, line_num, format, expected, actual)	 								\
{																									\
	printf("FAILED on line %d. ", line_num);					 									\
	printf("Expected: "); repr(format, expected);  			  										\
	printf(", actual: "); repr(format, actual);         	  										\
	printf(".\n");											  										\
}

#define assert_arr_failed_msg(repr, line_num, format,												\
							  expected, actual, size_expected, size_actual) 						\
{																									\
	printf("FAILED on line %d. ", line_num);						 								\
	printf("Expected: "); repr(format, expected, size_expected);  			  						\
	printf(", actual: "); repr(format, actual, size_actual);         	  							\
	printf(".\n");											  										\
}

#endif /* SUNIT_INT_H */
