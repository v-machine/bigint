/**
 * @file bigint.c
 * @brief Unit testing for the BigInt C library.
 * 
 * @author Vincent Mai
 * @version 0.5.0
 */

#include "bigint/bigint.h"
#include "bigint/bigint_internal.h"
#include "sunittest/sunittest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* read-only global variables */

char s_zero[]         = "0";
char s_small[]        = "-11";
char s_one_digit[]    = "999999999";
char s_two_digit[]    = "-1000000000";
char s_three_digit[]  = "1999999999111111111";
char s_four_digit[]   = "-3222222222111111111000000000";

BigInt* zero;
BigInt* small;
BigInt* one_digit;
BigInt* two_digit;
BigInt* three_digit;

void set_up()
{
    zero = bigint_init(s_zero);
    small = bigint_init(s_small);
    one_digit = bigint_init(s_one_digit);
    two_digit = bigint_init(s_two_digit);
    three_digit = bigint_init(s_three_digit);
}

void tear_down()
{
    bigint_free(&zero);
    bigint_free(&small);
    bigint_free(&one_digit);
    bigint_free(&two_digit);
    bigint_free(&three_digit);
}

int __strlen(char* sn)
{
    return (sn[0] == '-') ? 1 - strlen(sn) : strlen(sn);
}

void test_bigint_init()
{
    set_bail_on_fail();

    int strlen_s_zero        = __strlen(s_zero);
    int strlen_s_small       = __strlen(s_small);
    int strlen_s_one_digit   = __strlen(s_one_digit);
    int strlen_s_two_digit   = __strlen(s_two_digit); 
    int strlen_s_three_digit = __strlen(s_three_digit);
    int strlen_s_four_digit  = __strlen(s_four_digit);

    uint32_t* u_zero         = __to_base_giga(s_zero,        abs(strlen_s_zero)); 
    uint32_t* u_small        = __to_base_giga(s_small,       abs(strlen_s_small)); 
    uint32_t* u_one_digit    = __to_base_giga(s_one_digit,   abs(strlen_s_one_digit));
    uint32_t* u_two_digit    = __to_base_giga(s_two_digit,   abs(strlen_s_two_digit));
    uint32_t* u_three_digit  = __to_base_giga(s_three_digit, abs(strlen_s_three_digit));
    uint32_t* u_four_digit   = __to_base_giga(s_four_digit,  abs(strlen_s_four_digit));

    BigInt* _zero = bigint_init(s_zero);
    BigInt* _small = bigint_init(s_small);
    BigInt* _one_digit = bigint_init(s_one_digit);
    BigInt* _two_digit = bigint_init(s_two_digit);
    BigInt* _three_digit = bigint_init(s_three_digit);
    BigInt* _four_digit = bigint_init(s_four_digit);

    assert_uint32_arr_eq(u_zero,        _zero->digits,        *u_zero + 1,        *(_zero->digits) + 1);
    assert_uint32_arr_eq(u_small,       _small->digits,       *u_small + 1,       *(_small->digits) + 1);
    assert_uint32_arr_eq(u_one_digit,   _one_digit->digits,   *u_one_digit + 1,   *(_one_digit->digits) + 1);
    assert_uint32_arr_eq(u_two_digit,   _two_digit->digits,   *u_two_digit + 1,   *(_two_digit->digits) + 1);
    assert_uint32_arr_eq(u_three_digit, _three_digit->digits, *u_three_digit + 1, *(_three_digit->digits) + 1);
    assert_uint32_arr_eq(u_four_digit,  _four_digit->digits,  *u_four_digit + 1,  *(_four_digit->digits) + 1);

    assert_int_eq(strlen_s_zero,        _zero->sign_len);
    assert_int_eq(strlen_s_small,       _small->sign_len);
    assert_int_eq(strlen_s_one_digit,   _one_digit->sign_len);
    assert_int_eq(strlen_s_two_digit,   _two_digit->sign_len);
    assert_int_eq(strlen_s_three_digit, _three_digit->sign_len);
    assert_int_eq(strlen_s_four_digit,  _four_digit->sign_len);

    free(u_zero);
    free(u_small);
    free(u_one_digit);
    free(u_two_digit);
    free(u_three_digit);
    free(u_four_digit);

    free(_zero->digits);
    free(_small->digits);
    free(_one_digit->digits);
    free(_two_digit->digits);
    free(_three_digit->digits);
    free(_four_digit->digits);

    free(_zero);
    free(_small);
    free(_one_digit);
    free(_two_digit);
    free(_three_digit);
    free(_four_digit);
}

void test_bigint_int_init()
{
    BigInt* _zero      = bigint_int_init(0);
    BigInt* _small     = bigint_int_init(-11);
    BigInt* _one_digit = bigint_int_init(999999999);

    assert_uint32_arr_eq(zero->digits,      _zero->digits,      *(zero->digits) + 1,      *(_zero->digits) + 1);
    assert_uint32_arr_eq(small->digits,     _small->digits,     *(small->digits) + 1,     *(_small->digits) + 1);
    assert_uint32_arr_eq(one_digit->digits, _one_digit->digits, *(one_digit->digits) + 1, *(_one_digit->digits) + 1);

    assert_int_eq(zero->sign_len,      _zero->sign_len);
    assert_int_eq(small->sign_len,     _small->sign_len);
    assert_int_eq(one_digit->sign_len, _one_digit->sign_len);

    free(_zero->digits);
    free(_small->digits);
    free(_one_digit->digits);

    free(_zero);
    free(_small);
    free(_one_digit);
}

void test_bigint_free()
{
    BigInt* _zero = bigint_init(s_zero);
    BigInt* _small = bigint_init(s_small);
    BigInt* _one_digit = bigint_init(s_one_digit);
    BigInt* _two_digit = bigint_init(s_two_digit);
    BigInt* _three_digit = bigint_init(s_three_digit);
    BigInt* _four_digit = bigint_init(s_four_digit);

    bigint_free(&_zero);
    bigint_free(&_small);
    bigint_free(&_one_digit);
    bigint_free(&_two_digit);
    bigint_free(&_three_digit);
    bigint_free(&_four_digit);

    assert_false(_zero);
    assert_false(_small);
    assert_false(_one_digit);
    assert_false(_two_digit);
    assert_false(_three_digit);
    assert_false(_four_digit);
}

void test_bigint_to_str()
{
    char* _s_zero        = bigint_to_str(zero);
    char* _s_small       = bigint_to_str(small);
    char* _s_one_digit   = bigint_to_str(one_digit);
    char* _s_two_digit   = bigint_to_str(two_digit);
    char* _s_three_digit = bigint_to_str(three_digit);

    assert_str_eq(s_zero,           _s_zero);
    assert_str_eq(s_small,          _s_small);
    assert_str_eq(s_one_digit,   _s_one_digit);
    assert_str_eq(s_two_digit,      _s_two_digit);
    assert_str_eq(s_three_digit,    _s_three_digit);

    free(_s_zero);
    free(_s_small);
    free(_s_one_digit);
    free(_s_two_digit);
    free(_s_three_digit);
}

void test_bigint_add()
{

}

void test_bigint_subtr()
{

}

void test_bigint_mult()
{

}

void test_bigint_div()
{

}

int main()
{
    run_all_tests(
        test_bigint_init,
        test_bigint_int_init,
        test_bigint_free,
        test_bigint_to_str,
        test_bigint_add,
        test_bigint_subtr,
        test_bigint_mult,
        test_bigint_div
    );
    return 0;
}
