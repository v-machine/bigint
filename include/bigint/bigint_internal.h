/**
 * @file _int_bigint.h
 * @brief Private header for unit testing BigInt C library.
 *
 * Subject to change without notice.
 *
 * @author Vincent Mai
 * @version 0.5.0
 */

#ifndef INT_BIGINT_H
#define INT_BIGINT_H

#include <stdio.h>
#include <hashmap.h>
#include <stdint.h>

/**
 * struct BigInt - stores big integer
 * 
 * Big integers are represented with Base-giga digits.
 * Inspired by CPython Bignum (PEP 237), which stores
 * big numbers in Base-30-bit.
 *
 * @sign_len Stores the sign and number of decimal digits.
 * @digits Stores the digits in Base-giga in little-endian order.
 */

typedef struct BigInt BigInt;

struct BigInt
{
	int32_t sign_len;
	uint32_t* digits;
};

/** internal uses */
BigInt* _neg(BigInt* n); // TODO: do this without using BigInt.

/**
 * struct QuoRem - stores a quotian and a remainder.
 *
 * Stores a single-digit and a remainder as an array of digits
 * (both in Base-giga). This struct store the result of a single
 * step in the long division algorithm.
 * 
 * @quotian A single digit quotian.
 * @remainder A multi-digit remainder.
 */
struct QuoRem 
{
	uint32_t quotian;
	uint32_t* remainder;
};

/* Constant unsigned digits */
static uint32_t U_DIGIT_ZERO[2] = {1, 0};
static uint32_t U_DIGIT_ONE[2] = {1, 1};
static uint32_t U_DIGIT_TWO[2] = {1, 2};

/* private functions */
int __same_sign(BigInt* a, BigInt* b);
int __is_zero(uint32_t* n);
int __is_one(uint32_t* n);
int __gt(uint32_t* a, uint32_t* b);
int __st(uint32_t* a, uint32_t* b);
int __eq(uint32_t* a, uint32_t* b);
int __cmp_key(void* a, void* b);
void __incr(uint32_t* n);
void __decr(uint32_t* n);
char* __repr_keyval(void* key);
uint32_t __to_decimal(uint32_t* n);
uint32_t __len_decimal(uint32_t* digits);
uint32_t __hash_digits(void* key);
uint32_t* __to_base_giga(char* sn, int32_t len);
uint32_t* __arg_len_max(uint32_t* a, uint32_t* b);
uint32_t* __arg_len_min(uint32_t* a, uint32_t* b);
uint32_t* __assign_digits(uint32_t n);
uint32_t* __copy_digits(uint32_t* n);
uint32_t* __right_shift(uint32_t* n);
uint32_t* __mult(uint32_t* a, uint32_t* b);
uint32_t* __add(uint32_t* a, uint32_t* b);
uint32_t* __subtr(uint32_t* a, uint32_t* b);
uint32_t* __slice_digits(uint32_t* n, int start, int end);
uint32_t* __power_mod(uint32_t* base, uint32_t* exp,
		             uint32_t* m, HashMap* cache);
uint32_t** __divmod(uint32_t* n, uint32_t* m);
struct QuoRem* __single_divmod(uint32_t* n, uint32_t* d);

/* debugging functions */
void print_digits(char* var_name, uint32_t* d);

#endif /* INT_BIGINT_H */
