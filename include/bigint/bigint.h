/**
 * @file bigint.h
 * @brief Public API for the BigInt C library.
 *
 * Contains function prototypes for large intergers (BigInt) arightmetics.
 * Aside for taking inspiration from the CPython struct for Bignum PEP(237),
 * all implemntation is of my own. This library is not intented for security
 * usage such as crytography. Use at your own risk.
 *
 * @author Vincent Mai
 * @version 0.5.0
 */

#ifndef BIGINT_H
#define BIGINT_H

#include <stdint.h>

typedef struct BigInt BigInt;

/**
 * @brief Initializes a BigInt by a string.
 *
 * @param sn A string representating a big integer.
 * @return A pointer to the initialized BigInt.
 */
BigInt* bigint_init(char* sn);

/**
 * @brief Initializes a BigInt by an integer.
 *
 * @param n An integer to 
 * @return A pointer to the initialized BigInt.
 */
BigInt* bigint_int_init(int32_t n);

/**
 * @brief Converts a BigInt to string.
 *
 * @param n A BigInt to be converted to string.
 * @return The string representation of the BigInt.
 */
char* bigint_to_str(BigInt* n);

/**
 * @brief Sums a and b.
 *
 * @param a A large integer as a BigInt.
 * @param b A large integer as a BigInt.
 * @return A pointer to the sum as a BigInt.
 */
BigInt* bigint_add(BigInt* a, BigInt* b);

/**
 * @brief Subtracts a with b.
 *
 * @param a A large integer as a BigInt.
 * @param b A large integer as a BigInt.
 * @return A pointer to the difference as a BigInt.
 */
BigInt* bigint_subtr(BigInt* a, BigInt* b);

/**
 * @brief Multiplies a and b.
 *
 * @param a A large integer as a BigInt.
 * @param b A large integer as a BigInt.
 * @return A pointer to the product as a BigInt.
 */
BigInt* bigint_mult(BigInt* a, BigInt* b);

/**
 * @brief Divides n by d.
 *
 * This is a integer division, where the quotian (q) is defined as the
 * largest interger that satisfies q * b  <= a.
 *
 * @param n The divident as a BigInt.
 * @param d The divisor as a BigInt.
 * @return A pointer to the quotian as a BigInt.
 */
BigInt* bigint_div(BigInt* n, BigInt* d);

/**
 * @brief Computes n modulo m.
 *
 * TODO: follow C convention
 * This modulo operation follows the C '%' modulo (actually remainder)
 * operator as defined in C99, i.e., (n / m) * m + (n % m) == n.
 *
 * @param n The divident as a BigInt.
 * @param m The divisor as a BigInt.
 * @return A pointer to the remainder as a BigInt.
 */
BigInt* bigint_mod(BigInt* n, BigInt* m);

/**
 * @brief Computes logarithm of n with base b.
 *
 * This is a integer logarithm, where the exponent e is defined as the
 * largest positive interger that satisfies b ** e == n.
 *
 * @param n A large integer as a BigInt.
 * @param b The base of logarithm as a BigInt.
 * @return A pointer to the exponent as a BigInt.
 */
BigInt* bigint_log(BigInt* n, BigInt* b);

/**
 * @brief Computes (b raised to e) modulo m.
 *
 * The exponent b, e must be positive. The modulo function behaves
 * indentically to bigint_mod().
 *
 * @param b The base as a BigInt.
 * @param e The exponent as a BigInt.
 * @param m The divisor as a BigInt.
 * @return A pointer to the remainder as a BigInt.
 */
BigInt* bigint_power_mod (BigInt* b, BigInt* e, BigInt* m);

/**
 * @brief Returns the absolute value of n
 *
 * @param n A large integer as a BigInt.
 * @return A pointer to the absolute value of n as a BigInt.
 */
BigInt* bigint_abs(BigInt* n);

/**
 * @brief Negates the sign of n
 *
 * @param n A large integer as a BigInt.
 * @return A pointer to the negated n as a BigInt.
 */
BigInt* bigint_neg(BigInt* n);

/**
 * @brief Compares if a is smaller than b.
 *
 * @param a Large integer stored as a BigInt.
 * @param b Large integer stored as a BigInt.
 * @return 1 if a > b, and 0 otherwise.
 */
int bigint_st(BigInt* a, BigInt* b);

/**
 * @brief Compares if a is greater than b.
 *
 * @param a Large integer stored as a BigInt.
 * @param b Large integer stored as a BigInt.
 * @return 1 if a < b, and 0 otherwise.
 */
int bigint_gt(BigInt* a, BigInt* b);

/**
 * @brief Compares if a is equal to b.
 *
 * @param a Large integer stored as a BigInt.
 * @param b Large integer stored as a BigInt.
 * @return 1 if a == b, and 0 otherwise.
 */
int bigint_eq(BigInt* a, BigInt* b);

/**
 * @brief Returns a copy of an BigInt.
 *
 * @param n Large integer stored as a BigInt.
 * @return A pointer to the copy of n.
 */
BigInt* bigint_copy(BigInt* n);

/**
 * @brief Returns a 32bit hash code of the input BigInt.
 *
 * This hash function is designed to be used by a hashmap-like data
 * structure. The void pointer arguments is defined such that the 
 * caller data structure can be decoupled from the type BigInt.
 *
 * @param n Large integer stored as a BigInt.
 * @return A 32bit hash code.
 */
uint32_t bigint_hash(void* n);

/**
 * @brief Frees the allocated heap memory.
 *
 * @param n The address of a BigInt pointer.
 */
void bigint_free(BigInt** n);

#endif /* BIGINT_H */
