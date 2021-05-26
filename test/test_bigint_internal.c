/**
 * @file bigint.c
 * @brief Unit testing of internal functions for the BigInt C library.
 *
 * This module is subject to change without notice.
 * 
 * @author Vincent Mai
 * @version 0.5.0
 */

#include "bigint/bigint_internal.h"
#include "sunittest/sunittest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* read-only global variables */

char s_zero[]           = "0";
char s_one[]            = "1";
char s_one_digit[]      = "999999999";
char s_two_digit[]      = "1000000000";
char s_three_digit[]    = "1999999999777777777";
char s_four_digit[]     = "3222222222111111111000000000";
/* TODO: test the largest supported big integer */

uint32_t* zero;
uint32_t* one;
uint32_t* one_digit;
uint32_t* two_digit;
uint32_t* three_digit;
uint32_t* four_digit;

/* static const digits */
const uint32_t c_zero[]         = {1, 0}; 
const uint32_t c_one[]          = {1, 1};
const uint32_t c_one_digit[]    = {1, 999999999};
const uint32_t c_two_digit[]    = {2, 0, 1};
const uint32_t c_three_digit[]  = {3, 777777777, 999999999, 1};
const uint32_t c_four_digit[]   = {4, 0, 111111111, 222222222, 3};

void set_up()
{
    zero         = malloc(2 * sizeof(uint32_t));
    one          = malloc(2 * sizeof(uint32_t));
    one_digit    = malloc(2 * sizeof(uint32_t));
    two_digit    = malloc(3 * sizeof(uint32_t));
    three_digit  = malloc(4 * sizeof(uint32_t));
    four_digit   = malloc(5 * sizeof(uint32_t));

    memcpy(zero,        c_zero,         2 * sizeof(uint32_t));    
    memcpy(one,         c_one,          2 * sizeof(uint32_t));    
    memcpy(one_digit,   c_one_digit,    2 * sizeof(uint32_t));    
    memcpy(two_digit,   c_two_digit,    3 * sizeof(uint32_t));    
    memcpy(three_digit, c_three_digit,  4 * sizeof(uint32_t));    
    memcpy(four_digit,  c_four_digit,   5 * sizeof(uint32_t));    
}

void tear_down() 
{
    free(zero);
    free(one);
    free(one_digit);
    free(two_digit);
    free(three_digit);
    free(four_digit);
}

void test_to_base_giga()
{    
    set_bail_on_fail();

    uint32_t* _zero         = __to_base_giga(s_zero,        strlen(s_zero));
    uint32_t* _one          = __to_base_giga(s_one,         strlen(s_one));
    uint32_t* _one_digit    = __to_base_giga(s_one_digit,   strlen(s_one_digit));
    uint32_t* _two_digit    = __to_base_giga(s_two_digit,   strlen(s_two_digit));
    uint32_t* _three_digit  = __to_base_giga(s_three_digit, strlen(s_three_digit));
    uint32_t* _four_digit   = __to_base_giga(s_four_digit,  strlen(s_four_digit));

    assert_uint32_arr_eq(zero,          _zero,          *zero + 1,           *_zero + 1);
    assert_uint32_arr_eq(one,           _one,           *one + 1,            *_one + 1);
    assert_uint32_arr_eq(one_digit,     _one_digit,     *one_digit + 1,      *_one_digit + 1);
    assert_uint32_arr_eq(two_digit,     _two_digit,     *two_digit + 1,      *_two_digit + 1);
    assert_uint32_arr_eq(three_digit,   _three_digit,   *three_digit + 1,    *_three_digit + 1);
    assert_uint32_arr_eq(four_digit,    _four_digit,    *four_digit + 1,     *_four_digit + 1);

    free(_zero);
    free(_one);
    free(_one_digit);
    free(_two_digit);
    free(_three_digit);
    free(_four_digit);
}

void test_len_decimal()
{
    set_bail_on_fail();

    assert_int_eq((int) strlen(s_zero),         __len_decimal(zero));
    assert_int_eq((int) strlen(s_one),          __len_decimal(one));
    assert_int_eq((int) strlen(s_one_digit),    __len_decimal(one_digit));
    assert_int_eq((int) strlen(s_two_digit),    __len_decimal(two_digit));
    assert_int_eq((int) strlen(s_three_digit),  __len_decimal(three_digit));
    assert_int_eq((int) strlen(s_four_digit),   __len_decimal(four_digit));
}

void test_to_decimal()
{
    /* used on single digit only, else risks uint32_t overflow */
    assert_int_eq(atoi(s_zero),         __to_decimal(zero));
    assert_int_eq(atoi(s_one),          __to_decimal(one));
    assert_int_eq(atoi(s_one_digit),    __to_decimal(one_digit));
}

void test_is_zero()
{
    assert_true(__is_zero(zero));

    assert_false(__is_zero(one));
    assert_false(__is_zero(two_digit));
    assert_false(__is_zero(four_digit));
}

void test_is_one()
{
    assert_true(__is_one(one));

    assert_false(__is_one(zero));
    assert_false(__is_one(one_digit));
    assert_false(__is_one(three_digit));
}

void test_gt()
{
    assert_true(__gt(one,           zero));
    assert_true(__gt(two_digit,     one_digit));
    assert_true(__gt(four_digit,    three_digit));

    assert_false(__gt(zero,         one));
    assert_false(__gt(one,          one_digit));
    assert_false(__gt(two_digit,    three_digit));
}

void test_st()
{
    assert_true(__st(zero,          one));
    assert_true(__st(one_digit,     two_digit));
    assert_true(__st(three_digit,   four_digit));
    
    assert_false(__st(one,           zero));
    assert_false(__st(one_digit,     one));    
    assert_false(__st(three_digit,   two_digit));
}

void test_eq()
{
    assert_true(__eq(zero,          zero));
    assert_true(__eq(one,           one));
    assert_true(__eq(two_digit,     two_digit));
    assert_true(__eq(three_digit,   three_digit));

    assert_false(__eq(zero,         one));
    assert_false(__eq(one,          one_digit));
    assert_false(__eq(one_digit,    two_digit));
    assert_false(__eq(two_digit,    three_digit));
}

void test_incr()
{
    uint32_t incr_zero[]         = {1, 1}; 
    uint32_t incr_one_digit[]    = {2, 0, 1};
    uint32_t incr_two_digit[]    = {2, 1, 1};
    uint32_t incr_four_digit[]   = {4, 1, 111111111, 222222222, 3};
    
    // inplace increment
    __incr(zero);
    __incr(one_digit);
    __incr(two_digit);
    __incr(four_digit);

    assert_uint32_arr_eq(incr_zero,         zero,           *incr_zero + 1,         *zero + 1);
    assert_uint32_arr_eq(incr_one_digit,    one_digit,      *incr_one_digit + 1,    *one_digit + 1);
    assert_uint32_arr_eq(incr_two_digit,    two_digit,      *incr_two_digit + 1,    *two_digit + 1);
    assert_uint32_arr_eq(incr_four_digit,   four_digit,     *incr_four_digit + 1,   *four_digit + 1);
}

void test_decr()
{
    uint32_t decr_zero[]        = {1, 0};  // halt at zero
    uint32_t decr_one[]         = {1, 0};
    uint32_t decr_two_digit[]   = {1, 999999999};
    uint32_t decr_four_digit[]  = {4, 999999999, 111111110, 222222222, 3};
    
    // inplace decrement
    __decr(zero);
    __decr(one);
    __decr(two_digit);
    __decr(four_digit);

    assert_uint32_arr_eq(decr_zero,         zero,           *decr_zero + 1,         *zero + 1);
    assert_uint32_arr_eq(decr_one,          one,            *decr_one + 1,          *one + 1);
    assert_uint32_arr_eq(decr_two_digit,    two_digit,      *decr_two_digit + 1,    *two_digit + 1);
    assert_uint32_arr_eq(decr_four_digit,   four_digit,     *decr_four_digit + 1,   *four_digit + 1);
}

void test_arg_len_max()
{    /* compare only the number of digits */
    assert_uint32_arr_eq(two_digit,      __arg_len_max(two_digit, one_digit),    *two_digit + 1,     *two_digit + 1);
    assert_uint32_arr_eq(three_digit,    __arg_len_max(three_digit, two_digit),  *three_digit + 1,   *three_digit + 1);
    assert_uint32_arr_eq(four_digit,     __arg_len_max(four_digit, three_digit), *four_digit + 1,    *four_digit + 1);
}

void test_arg_len_min()
{    /* compare only the number of digits */
    assert_uint32_arr_eq(one_digit,     __arg_len_min(two_digit, one_digit),    *one_digit + 1,     *one_digit + 1);
    assert_uint32_arr_eq(two_digit,     __arg_len_min(three_digit, two_digit),  *two_digit + 1,     *two_digit + 1);
    assert_uint32_arr_eq(three_digit,   __arg_len_min(four_digit, three_digit), *three_digit + 1,   *three_digit + 1);
}

void test_assign_digits()
{
    uint32_t* _zero      = __assign_digits(0);
    uint32_t* _one       = __assign_digits(1);
    uint32_t* _one_digit = __assign_digits(999999999);

    assert_uint32_arr_eq(zero,      _zero,      *zero + 1,      *_zero + 1);
    assert_uint32_arr_eq(one,       _one,       *one + 1,       *_one + 1);
    assert_uint32_arr_eq(one_digit, _one_digit, *one_digit + 1, *_one_digit + 1);

    free(_zero);
    free(_one);
    free(_one_digit);
}

void test_copy_digits()
{
    uint32_t* _zero         = __copy_digits(zero);
    uint32_t* _one          = __copy_digits(one);
    uint32_t* _one_digit    = __copy_digits(one_digit);
    uint32_t* _two_digit    = __copy_digits(two_digit);
    uint32_t* _three_digit  = __copy_digits(three_digit);
    uint32_t* _four_digit   = __copy_digits(four_digit);
    
    assert_uint32_arr_eq(zero,          _zero,          *zero + 1,          *_zero + 1);
    assert_uint32_arr_eq(one,           _one,           *one + 1,           *_one + 1);
    assert_uint32_arr_eq(one_digit,     _one_digit,     *one_digit + 1,     *_one_digit + 1);
    assert_uint32_arr_eq(two_digit,     _two_digit,     *two_digit + 1,     *_two_digit + 1);
    assert_uint32_arr_eq(three_digit,   _three_digit,   *three_digit + 1,   *_three_digit + 1);
    assert_uint32_arr_eq(four_digit,    _four_digit,    *four_digit + 1,    *_four_digit + 1);

    free(_zero);
    free(_one);
    free(_one_digit);
    free(_two_digit);
    free(_three_digit);
    free(_four_digit);
}

void test_slice_digits()
{
    uint32_t slice_three_digit_j[]  = {1, 999999999};
    uint32_t slice_three_digit_jk[] = {2, 999999999, 1};
    uint32_t slice_three_digit_ij[] = {2, 777777777, 999999999};

    uint32_t* _slice_one             = __slice_digits(one_digit, 1, 2);
    uint32_t* _slice_three_digit_ijk = __slice_digits(three_digit, 1, 4);
    uint32_t* _slice_three_digit_j   = __slice_digits(three_digit, 2, 3);
    uint32_t* _slice_three_digit_ij  = __slice_digits(three_digit, 1, 3);
    uint32_t* _slice_three_digit_jk  = __slice_digits(three_digit, 2, 4);

    assert_uint32_arr_eq(one_digit,              _slice_one,             1, 1);
    assert_uint32_arr_eq(three_digit,            _slice_three_digit_ijk, 3, 3);
    assert_uint32_arr_eq(slice_three_digit_j,    _slice_three_digit_j,   1, 1);
    assert_uint32_arr_eq(slice_three_digit_ij,   _slice_three_digit_ij,  2, 2);
    assert_uint32_arr_eq(slice_three_digit_jk,   _slice_three_digit_jk,  2, 2);

    free(_slice_one);
    free(_slice_three_digit_ijk);
    free(_slice_three_digit_j);
    free(_slice_three_digit_ij);
    free(_slice_three_digit_jk);
}

void test_right_shift() 
{
    uint32_t shift_one[]        = {2, 0, 1};
    uint32_t shift_two_digit[]  = {3, 0, 0, 1};
    uint32_t shift_four_digit[] = {5, 0, 0, 111111111, 222222222, 3};

    uint32_t* _shift_zero       = __right_shift(zero);
    uint32_t* _shift_one        = __right_shift(one);
    uint32_t* _shift_two_digit  = __right_shift(two_digit);
    uint32_t* _shift_four_digit = __right_shift(four_digit);

    assert_uint32_arr_eq(zero,              _shift_zero,        *zero + 1,              *_shift_zero + 1);
    assert_uint32_arr_eq(shift_one,         _shift_one,         *shift_one + 1,         *_shift_one + 1);
    assert_uint32_arr_eq(shift_two_digit,   _shift_two_digit,   *shift_two_digit + 1,   *_shift_two_digit + 1);
    assert_uint32_arr_eq(shift_four_digit,  _shift_four_digit,  *shift_four_digit + 1,  *_shift_four_digit + 1);

    free(_shift_zero);
    free(_shift_one);
    free(_shift_two_digit);
    free(_shift_four_digit);
}

void test_add()
{    
    uint32_t add_to_three_digit[] = {1, 222222223};

    uint32_t res_three_digit[]  = {3, 0, 0, 2};
    uint32_t res_four_digit[]   = {4, 777777777, 111111110, 222222224, 3};

    uint32_t* _res_one          = __add(zero, one);
    uint32_t* _res_two_digit    = __add(one, one_digit);
    uint32_t* _res_three_digit  = __add(three_digit, add_to_three_digit);
    uint32_t* _res_four_digit   = __add(four_digit, three_digit);

    assert_uint32_arr_eq(one,             _res_one,           *zero + 1,            *_res_one + 1);
    assert_uint32_arr_eq(two_digit,       _res_two_digit,     *two_digit + 1,       *_res_two_digit + 1);
    assert_uint32_arr_eq(res_three_digit, _res_three_digit,   *res_three_digit + 1, *_res_three_digit + 1);
    assert_uint32_arr_eq(res_four_digit,  _res_four_digit,    *res_four_digit + 1,  *_res_four_digit + 1);

    free(_res_one);
    free(_res_two_digit);
    free(_res_three_digit);
    free(_res_four_digit);
}

void test_subtr() 
{
    uint32_t res_three_digit[] = {3, 777777777, 999999998, 1};
    uint32_t res_four_digit [] = {2, 500000000, 111111111};

    /* hidden failed cases */
    uint32_t n[] = {4, 0, 0, 0, 1};
    uint32_t m[] = {3, 500000000, 888888888, 999999999};

    uint32_t* _res_zero         = __subtr(zero, zero);
    uint32_t* _res_one          = __subtr(two_digit, one_digit);
    uint32_t* _res_one_digit    = __subtr(two_digit, one);
    uint32_t* _res_three_digit  = __subtr(three_digit, two_digit);
    uint32_t* _res_four_digit   = __subtr(n, m);

    assert_uint32_arr_eq(zero,               _res_zero,           *zero + 1,           *_res_zero + 1);
    assert_uint32_arr_eq(one,                _res_one,            *one + 1,            *_res_one + 1);
    assert_uint32_arr_eq(one_digit,          _res_one_digit,      *one_digit + 1,      *_res_one_digit + 1);
    assert_uint32_arr_eq(res_three_digit,    _res_three_digit,    *three_digit + 1,    *_res_three_digit + 1);
    assert_uint32_arr_eq(res_four_digit,     _res_four_digit,     *res_four_digit + 1, *_res_four_digit + 1);

    free(_res_zero);
    free(_res_one);
    free(_res_one_digit);
    free(_res_three_digit);
    free(_res_four_digit);
}

void test_mult()
{
    uint32_t res_two_digit[]  = {2, 0, 999999999};
    uint32_t res_four_digit[] = {4, 0, 777777777, 999999999, 1};
    uint32_t res_five_digit[] = {5, 395061729, 49382716, 111111108, 999999999, 3};
    uint32_t res_six_digit[]  = {6, 0, 913580247, 802469135, 506172836, 444444443, 6};

    uint32_t* _res_zero       = __mult(zero, one_digit);
    uint32_t* _res_one_digit  = __mult(one, one_digit);
    uint32_t* _res_two_digit  = __mult(two_digit, one_digit);
    uint32_t* _res_four_digit = __mult(two_digit, three_digit);
    uint32_t* _res_five_digit = __mult(three_digit, three_digit);
    uint32_t* _res_six_digit  = __mult(three_digit, four_digit);

    assert_uint32_arr_eq(zero,              _res_zero,          *zero + 1,              *_res_zero + 1);
    assert_uint32_arr_eq(one_digit,         _res_one_digit,     *one_digit + 1,         *_res_one_digit + 1);
    assert_uint32_arr_eq(res_two_digit,     _res_two_digit,     *res_two_digit + 1,     *_res_two_digit + 1);
    assert_uint32_arr_eq(res_four_digit,    _res_four_digit,    *res_four_digit + 1,    *_res_four_digit + 1);
    assert_uint32_arr_eq(res_five_digit,    _res_five_digit,    *res_five_digit + 1,    *_res_five_digit + 1);
    assert_uint32_arr_eq(res_six_digit,     _res_six_digit,     *res_six_digit + 1,     *_res_six_digit + 1);

    free(_res_zero);
    free(_res_one_digit);
    free(_res_two_digit);
    free(_res_four_digit);
    free(_res_five_digit);
    free(_res_six_digit);
}

void test_single_divmod()
{
    uint32_t two[]        = {1, 2};

    struct QuoRem* _res_a = __single_divmod(one_digit, one);
    struct QuoRem* _res_b = __single_divmod(one_digit, two);
    struct QuoRem* _res_c = __single_divmod(two_digit, two);
    struct QuoRem* _res_d = __single_divmod(two_digit, one_digit);

    assert_int_eq(999999999,    _res_a->quotian);
    assert_int_eq(499999999,    _res_b->quotian);
    assert_int_eq(500000000,    _res_c->quotian);
    assert_int_eq(1,            _res_d->quotian);

    assert_uint32_arr_eq(zero,   _res_a->remainder, *zero + 1,    *(_res_a->remainder) + 1);  
    assert_uint32_arr_eq(one,    _res_b->remainder, *one + 1,     *(_res_b->remainder) + 1);
    assert_uint32_arr_eq(zero,   _res_c->remainder, *zero + 1,    *(_res_c->remainder) + 1);
    assert_uint32_arr_eq(one,    _res_d->remainder, *one + 1,     *(_res_d->remainder) + 1);

    free(_res_a);
    free(_res_b);
    free(_res_c);
    free(_res_d);
}

void test_divmod()
{
    uint32_t** _res_a = __divmod(three_digit,    one);
    uint32_t** _res_b = __divmod(three_digit,    one_digit);
    uint32_t** _res_c = __divmod(three_digit,    two_digit);
    uint32_t** _res_d = __divmod(four_digit,     one);
    uint32_t** _res_e = __divmod(four_digit,     one_digit);
    uint32_t** _res_f = __divmod(four_digit,     two_digit);
    uint32_t** _res_g = __divmod(four_digit,     three_digit);

    uint32_t quo_b[] = {2, 1, 2};
    uint32_t quo_c[] = {2, 999999999, 1};
    uint32_t quo_e[] = {3, 333333336, 222222225, 3};
    uint32_t quo_f[] = {3, 111111111, 222222222, 3};
    uint32_t quo_g[] = {2, 611111111, 1};

    uint32_t rem_b[] = {1, 777777778};
    uint32_t rem_c[] = {1, 777777777};
    uint32_t rem_e[] = {1, 333333336};
    uint32_t rem_g[] = {2, 586419753, 469135803};
    
    assert_uint32_arr_eq(three_digit,   _res_a[0],    *three_digit + 1,   *(_res_a[0]) + 1);  
    assert_uint32_arr_eq(quo_b,         _res_b[0],    *quo_b + 1,         *(_res_b[0]) + 1);  
    assert_uint32_arr_eq(quo_c,         _res_c[0],    *quo_c + 1,         *(_res_c[0]) + 1);  
    assert_uint32_arr_eq(four_digit,    _res_d[0],    *four_digit + 1,    *(_res_d[0]) + 1);  
    assert_uint32_arr_eq(quo_e,         _res_e[0],    *quo_e + 1,         *(_res_e[0]) + 1);  
    assert_uint32_arr_eq(quo_f,         _res_f[0],    *quo_f + 1,         *(_res_f[0]) + 1);  
    assert_uint32_arr_eq(quo_g,         _res_g[0],    *quo_g + 1,         *(_res_g[0]) + 1);  

    assert_uint32_arr_eq(zero,      _res_a[1],    *zero + 1,     *(_res_a[1]) + 1);  
    assert_uint32_arr_eq(rem_b,     _res_b[1],    *rem_b + 1,    *(_res_b[1]) + 1);  
    assert_uint32_arr_eq(rem_c,     _res_c[1],    *rem_c + 1,    *(_res_c[1]) + 1);  
    assert_uint32_arr_eq(zero,      _res_d[1],    *zero + 1,     *(_res_d[1]) + 1);  
    assert_uint32_arr_eq(rem_e,     _res_e[1],    *rem_e + 1,    *(_res_e[1]) + 1);  
    assert_uint32_arr_eq(zero,      _res_f[1],    *zero + 1,     *(_res_f[1]) + 1);  
    assert_uint32_arr_eq(rem_g,     _res_g[1],    *rem_g + 1,    *(_res_g[1]) + 1);  

    free(_res_a[0]); free(_res_a[1]); free(_res_a);
    free(_res_b[0]); free(_res_b[1]); free(_res_b);
    free(_res_c[0]); free(_res_c[1]); free(_res_c);
    free(_res_d[0]); free(_res_d[1]); free(_res_d);
    free(_res_e[0]); free(_res_e[1]); free(_res_e);
    free(_res_f[0]); free(_res_f[1]); free(_res_f);
    free(_res_g[0]); free(_res_g[1]); free(_res_g);
}

void test_power_mod()
{
    int size = 92;    
    // cache needs to be reset per __power_mod
    HashMap* cache = hashmap_init(size, __cmp_key, __hash_digits,
                                  __repr_keyval, __repr_keyval);

    uint32_t res_c[] = {1, 333333334};
    uint32_t res_d[] = {3, 645076623, 339392621, 1};
    uint32_t res_e[] = {2, 173585468, 182018086};

    uint32_t* _res_zero_a    = __power_mod(zero, three_digit, one, cache);
    hashmap_clear(cache);
    uint32_t* _res_zero_b     = __power_mod(two_digit, three_digit, two_digit, cache);
    hashmap_clear(cache);
    uint32_t* _res_one_a    = __power_mod(one, one_digit, two_digit, cache);
    hashmap_clear(cache);
    uint32_t* _res_one_b     = __power_mod(two_digit, zero, three_digit, cache);
    hashmap_clear(cache);
    uint32_t* _res_a         = __power_mod(one_digit, one, three_digit, cache);
    hashmap_clear(cache);
    uint32_t* _res_b         = __power_mod(two_digit, one_digit, one_digit, cache);
    hashmap_clear(cache);
    uint32_t* _res_c         = __power_mod(three_digit, four_digit, one_digit, cache);
    hashmap_clear(cache);
    uint32_t* _res_d         = __power_mod(two_digit, two_digit, three_digit, cache); // BUG Found!
    hashmap_clear(cache);
    uint32_t* _res_e         = __power_mod(two_digit, four_digit, three_digit, cache); // BUG Found!
    
    assert_uint32_arr_eq(zero,          _res_zero_a,    *zero + 1,         *_res_zero_a + 1);
    assert_uint32_arr_eq(zero,          _res_zero_b,    *zero + 1,         *_res_zero_b + 1);
    assert_uint32_arr_eq(one,           _res_one_a,     *one + 1,          *_res_one_a + 1);
    assert_uint32_arr_eq(one,           _res_one_b,     *one + 1,          *_res_one_b + 1);
    assert_uint32_arr_eq(one_digit,     _res_a,         *one_digit + 1,    *_res_a + 1);
    assert_uint32_arr_eq(one,           _res_b,         *one + 1,          *_res_b + 1);
    assert_uint32_arr_eq(res_c,         _res_c,         *res_c + 1,        *_res_c + 1);
    assert_uint32_arr_eq(res_d,         _res_d,         *res_d + 1,        *_res_d + 1);
    assert_uint32_arr_eq(res_e,         _res_e,         *res_e + 1,        *_res_e + 1);

    hashmap_free(&cache);
    
    free(_res_zero_a);
    free(_res_zero_b);
    free(_res_one_a);
    free(_res_one_b);
    free(_res_a);
    free(_res_b);
    free(_res_c);
    free(_res_d);
    free(_res_e);
}

int main()
{
    run_all_tests(
        test_to_base_giga,
        test_len_decimal,
        test_to_decimal,
        test_is_zero,
        test_is_one,
        test_gt,
        test_st,
        test_eq,
        test_incr,
        test_decr,
        test_arg_len_max,
        test_arg_len_min,
        test_assign_digits,
        test_copy_digits,
        test_slice_digits,
        test_right_shift,
        test_add,
        test_subtr,
        test_mult,
        test_single_divmod,
        test_divmod,
        test_power_mod
    );
    return 0;
}
