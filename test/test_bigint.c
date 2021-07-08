/**
 * @file bigint.c
 * @brief Unit testing for the BigInt C library.
 * 
 * @author Vincent Mai
 * @version 0.5.0
 */

#include "bigint/bigint.h"
#include "sunittest/sunittest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* read-only global variables */

char zero[]         = "0";
char small[]        = "11";
char single_digit[] = "999999999";
char two_digit[]    = "1000000000";
char three_digit[]  = "1999999999111111111";
char four_digit[]   = "3222222222111111111000000000";

void test_bigint_init()
{
    set_bail_on_fail();

    BigInt*    bn_zero = bigint_init(zero);
    BigInt*    bn_small = bigint_init(small);
    BigInt*    bn_single_digit = bigint_init(single_digit);
    BigInt*    bn_two_digit = bigint_init(two_digit);
    BigInt*    bn_three_digit = bigint_init(three_digit);

    assert_str_eq(zero, bigint_to_str(bn_zero));
    assert_str_eq(small, bigint_to_str(bn_small));
    assert_str_eq(single_digit, bigint_to_str(bn_single_digit));
    assert_str_eq(two_digit, bigint_to_str(bn_two_digit));
    assert_str_eq(three_digit, bigint_to_str(bn_three_digit));

    free(bn_zero);
    free(bn_small);
    free(bn_single_digit);
    free(bn_two_digit);
    free(bn_three_digit);
}

void set_up() {
}

void tear_down() {
}

int main()
{
    run_all_tests(
        test_bigint_init
    );
    return 0;
}
