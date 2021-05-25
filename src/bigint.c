/**
 * @file bigint.c
 * @brief Implentation for the BigInt C library.
 *
 * @author Vincent Mai
 * @version 0.5.0
 */

#include "hashmap.h"
#include "bigint/bigint.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define BASE 1000000000L
#define LEN_BASE 9

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
static uint32_t U_DIGIT_ONE[2]  = {1, 1};
static uint32_t U_DIGIT_TWO[2]  = {1, 2};

/* private functions */
int __same_sign(BigInt* a, BigInt* b);
int __is_zero(uint32_t* n);
int __is_one(uint32_t* n);
int __gt(uint32_t* a, uint32_t* b);
int __st(uint32_t* a, uint32_t* b);
int __eq(uint32_t* a, uint32_t* b);
void __incr(uint32_t* n);
void __decr(uint32_t* n);
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

/** debugging functions */
void print_digits(char* var_name, uint32_t* slice);

/****************************** SOURCE CODE ****************************/

BigInt* bigint_init(char* sn) 
{
    BigInt* bigint = malloc(sizeof(*bigint));
    bigint->sign_len = (sn[0] == '-') ?
        1 - strlen(sn) : strlen(sn);
    bigint->digits = __to_base_giga(sn, abs(bigint->sign_len));
    return bigint;
}

BigInt* bigint_int_init(int32_t n) 
{
    // TODO: delegate to __assign_digits(n);
    BigInt* bigint = malloc(sizeof(*bigint));
    bigint->sign_len = (n >= 0) ? n / 10 + 1 : -(n / 10 + 1);
    uint32_t* digits = malloc(2 * sizeof(*digits));
    if (n < BASE) {
        *digits = 1;
        digits[1] = abs(n);
    } else {
        *digits = 2;
        digits[1] = abs(n % BASE);
        digits[2] = abs(n / BASE);
    }
    bigint->digits = digits;
    return bigint;
}

char* bigint_to_str(BigInt* n) 
{
    int32_t len = (n->sign_len < 0) ?
        abs(n->sign_len)+1 : n->sign_len;

    char* s = malloc((len + 1) * sizeof(*s));
    char* s_i = s;
    if (n->sign_len < 0) {
        *s = '-';
        ++s_i;
    } 

    int i_th = *(n->digits);
    //sprintf returns len of formatted string
    s_i += sprintf(s_i, "%u", n->digits[i_th]); 
    --i_th;

    while (i_th > 0) {
        s_i += sprintf(s_i, "%0*u", LEN_BASE, n->digits[i_th]);
        --i_th;
    }
    return s;
}

BigInt* bigint_mult(BigInt* a, BigInt* b) 
{
    if (__is_zero(a->digits) || __is_zero(b->digits))
        return bigint_int_init(0);

    BigInt* res = malloc(sizeof(*res));
    uint32_t* digits = __mult(a->digits, b->digits);
    res->sign_len = __len_decimal(digits);
    res->digits = digits;

    return (__same_sign(a, b)) ? res : _neg(res);
}

BigInt* bigint_add(BigInt* a, BigInt* b) 
{
    BigInt* res = malloc(sizeof(*res));
    if (! __same_sign(a, b)) {
        if (a->sign_len > 0) {
            res = bigint_subtr(a, _neg(b));
            b = _neg(b);
        } else {
            res = bigint_subtr(b, _neg(a));
            a = _neg(a);
        }
        return res;
    }
    res->digits = __add(a->digits, b->digits);
    res->sign_len = __len_decimal(res->digits);
    return (a->sign_len < 0) ? _neg(res) : res; 
}

BigInt* bigint_subtr(BigInt* a, BigInt* b) 
{
    BigInt* res = malloc(sizeof(*res));
    if (! __same_sign(a, b)) {
        res->digits = __add(a->digits, b->digits);
        res->sign_len = __len_decimal(res->digits);
        if (a->sign_len <= 0) res->sign_len *= -1;
        return res;
    }
    int sign = 1;
    if (bigint_gt(a, b)) {
        res->digits = (a->sign_len >= 0) ? 
            __subtr(a->digits, b->digits) :
            __subtr(b->digits, a->digits);
    } else {
        sign *= -1;
        res->digits = (a->sign_len >= 0) ? 
            __subtr(b->digits, a->digits) :
            __subtr(a->digits, b->digits);
    }
    res->sign_len = __len_decimal(res->digits) * sign;
    return res;
}

BigInt* bigint_div(BigInt* n, BigInt* d) 
{
    if (__is_zero(d->digits)) {
        fprintf(stderr, "Division by zero. Existing...\n");
        exit(EXIT_FAILURE);
    }
    if (__gt(d->digits, n->digits)) {
        return (__same_sign(d, n)) ?
            bigint_int_init(0) : bigint_int_init(-1);
    }
    if (__is_one(d->digits)) {
        return bigint_copy(n);
    }

    BigInt* res = malloc(sizeof(*res));
    uint32_t** quorem = __divmod(n->digits, d->digits);
    uint32_t* quotian = quorem[0];
    free(quorem[1]);
    free(quorem);

    res->digits = quotian;
    res->sign_len = __len_decimal(quotian);
    return (__same_sign(n, d)) ? res : _neg(res);
}

// TODO: make this behave like c modulo operator.
BigInt* bigint_mod(BigInt* n, BigInt* m) 
{
    if (__is_zero(m->digits)) {
        fprintf(stderr, "Division by zero. Existing...\n");
        exit(EXIT_FAILURE);
    }
    if (__is_one(m->digits)) {
        return bigint_int_init(0);
    }

    BigInt* res = malloc(sizeof(*res));
    uint32_t** quorem = __divmod(n->digits, m->digits);
    uint32_t* remainder = quorem[1];
    free(quorem[0]);
    free(quorem);
    
    res->digits = ! (__same_sign(n, m) || __is_zero(remainder)) ?
        __subtr(m->digits, remainder) : remainder;
    res->sign_len = __len_decimal(res->digits);
    return (m->sign_len > 0) ? res : _neg(res);
}

uint32_t bigint_hash(void* n) 
{
    uint32_t* nd = ((BigInt*) n)->digits;
    uint32_t res = BASE * ((BigInt*) n)->sign_len;
    res = res << nd[*nd];

    for (int i = 1; i <= *nd; i++)
        res = (res + 3137 * nd[i]) % 1000003;

    return res;
}

int __cmp_key(void* a, void* b) 
{
    return __eq((uint32_t*) a, (uint32_t*) b);
}

char* __repr_keyval(void* key)
{
    uint32_t* n = (uint32_t*) key;
    char* res = malloc((__len_decimal(n) + 1) * sizeof(*res));
    sprintf(res, "%u", __to_decimal(n));
    return res;
}

uint32_t __hash_digits(void* key) 
{
    uint32_t* n = (uint32_t*) key;
    uint32_t res = *n * (BASE << *n);
    res = res << n[*n];

    for (int i = 1; i <= *n; i++)
        res = (res + 3137 * n[i]) % 1000003;

    return res;
}

BigInt* bigint_power_mod (BigInt* base, BigInt* exp, BigInt* m) 
{
    BigInt* two = bigint_int_init(2);
    BigInt* _size = bigint_log(exp, two);
    uint32_t size = __to_decimal(_size->digits);

    bigint_free(&_size); bigint_free(&two);
    HashMap* cache = hashmap_init(size, __cmp_key, __hash_digits,
                                  __repr_keyval, __repr_keyval);

    BigInt* res = malloc(sizeof(*res));
    
    uint32_t* digits = __power_mod(base->digits, exp->digits,
                                  m->digits, cache);
    
    hashmap_free(&cache);
    res->digits = digits;
    res->sign_len = __len_decimal(digits);
    return res;
}

uint32_t* __power_mod(uint32_t* base, uint32_t* exp,
                      uint32_t* m, HashMap* cache) 
{
    if (__is_zero(m)) {
        printf("Division by zero. Existing...\n");
        exit(EXIT_FAILURE);
    }
    if (__eq(base, m) || __is_one(m))
        return __assign_digits(0);
    if (__is_one(base) || __is_zero(exp))
        return __assign_digits(1);

    if (__is_one(exp)) {
        uint32_t** quorem = __divmod(base, m);
        uint32_t* res = quorem[1];
        free(quorem[0]); free(quorem);
        return res;
    } else {
        uint32_t** quorem = __divmod(exp, U_DIGIT_TWO);
        uint32_t* e_0 = quorem[0];
        free(quorem[1]); free(quorem);
        uint32_t* e_1 = __subtr(exp, e_0);
    
        if (! hashmap_get(cache, e_0)) {
            uint32_t* r_0 = __power_mod(base, e_0, m, cache);
            hashmap_insert(cache, e_0, r_0);
        }
        if (! hashmap_get(cache, e_1)) {
            uint32_t* r_1 = __power_mod(base, e_1, m, cache);
            hashmap_insert(cache, e_1, r_1);
        }
        uint32_t* prod = __mult(hashmap_get(cache, e_0),
                                        hashmap_get(cache, e_1));
        e_0 = NULL;
        e_1 = NULL;
        
        return __power_mod(prod, U_DIGIT_ONE, m, cache);
    }    
}

BigInt* bigint_log(BigInt* n, BigInt* b) 
{
    // TODO: refactor one zero two as constant digits
    uint32_t* zero = __assign_digits(0);
    uint32_t* one = __assign_digits(1);

    if (! __gt(n->digits, zero) || ! __gt(b->digits, one)) {
        printf("Math domain error. Existing...\n");
        exit(EXIT_FAILURE);
    }

    free(zero); free(one); // TODO: once refactored removed
    if (__is_one(n->digits)) return bigint_int_init(0);

    // TODO: more efficient implementation 
    // gap = *nd - *bd; base = pow(b, gap);
    // return res * gap (approximately)

    BigInt* res = malloc(sizeof(*res));    
    uint32_t* quo = __copy_digits(n->digits);
    uint32_t* exp = __assign_digits(0);

    while (! __is_zero(quo)) {
        __incr(exp);
        uint32_t** quorem = __divmod(quo, b->digits);
        free(quo); quo = quorem[0];

        free(quorem[1]);
        free(quorem);
    }

    __decr(exp);
    free(quo);

    res->digits = exp;
    res->sign_len = __len_decimal(exp);
    return res;
}

/**************************** BIGINT COMPARISON ****************************/ 

int bigint_gt(BigInt* a, BigInt* b) 
{
    if (a->sign_len > b->sign_len) return 1;
    if (a->sign_len == b->sign_len) {
        if (a->sign_len > 0) {
            return __st(a->digits, b->digits) ? 0 : 1;
        } else {
            return __gt(a->digits, b->digits) ? 0 : 1;
        }
    }
    return 0;
}

int bigint_eq(BigInt* a, BigInt* b) 
{
    if (a->sign_len != b->sign_len) return 0;
    return __eq(a->digits, b->digits);
}

int bigint_st(BigInt* a, BigInt* b) 
{
    return ! bigint_gt(a, b) && ! bigint_eq(a, b);
}

/**************************** BIGINT UTILITY *****************************/ 

BigInt* bigint_copy(BigInt* n) 
{
    BigInt* cp_n = malloc(sizeof(*n));
    cp_n->sign_len = n->sign_len;
    cp_n->digits = __copy_digits(n->digits); 
    return cp_n;
}

void bigint_free(BigInt** n) 
{
    free((*n)->digits);
    free(*n);
    *n = NULL;
}

/***************************** PRIVATE FUNCTIONS *****************************/

/** inplace negation */
BigInt* _neg(BigInt* n) 
{
    if (! __is_zero(n->digits)) {
        n->sign_len *= -1;
    }
    return n;
}

int __same_sign(BigInt* a, BigInt* b) 
{
    return (a->sign_len > 0 && b->sign_len > 0) || 
           (a->sign_len < 0 && b->sign_len < 0);
}

/**
 * Upper-bound exclusive.
 */
uint32_t* __slice_digits(uint32_t* n, int start, int end) 
{
    assert(end >= start);
    assert(start >= 1 && end <= *(n) + 1);

    uint32_t* digits = malloc((end - start + 1) * sizeof(*digits));
    *digits = end - start;
    for (int i = start; i < end; i++) {
        digits[i - start + 1] = n[i];
    }

    return digits;
}

/**
 * Right shifting digits, equivalent to multiply by base.
 */
uint32_t* __right_shift(uint32_t* n) 
{
    if (__is_zero(n)) return __assign_digits(0);
    int len = *(n) + 1;
    uint32_t* digits = malloc((len + 1) * sizeof(*digits));
    *digits = len;
    digits[1] = 0;
    for (int i = 1; i <= *(n); i++)
        digits[i + 1] = n[i];
    return digits;    
}

/***************************** UNSIGNED OPERATIONS *****************************/

// base conversion to 30 bit (actual: 10**9);
uint32_t* __to_base_giga(char* sn, int32_t len) 
{
    if (sn[0] == '-') ++sn;

    // store len of digits at idx 0
    uint32_t len_digits = len / (LEN_BASE);
    if (len % LEN_BASE) ++len_digits;
    uint32_t* digits = malloc((len_digits + 1) * sizeof(*digits)); 

    if (len_digits == 1) {
        *digits = 1;
        *(digits + 1) = atoi(sn);
        return digits;
    }
    
    uint32_t* cur_digit = digits + 1;
    int s_place = 0;
    int mult = 1;
    *digits = 1;
    *cur_digit = 0;

    for (int i = len - 1; i >= 0; i--) {
        uint32_t n_i = sn[i] - '0';
        *cur_digit += n_i * mult;
        mult *= 10;
        ++s_place;
        if (i > 0 && s_place % LEN_BASE == 0) {
            ++cur_digit;
            *cur_digit = 0;
            mult = 1;
            ++*digits;
        }
    }
    return digits;
}

uint32_t* __assign_digits(uint32_t n) 
{
    int len = (n < BASE) ? 1 : 2; // max(uint32_t) < 4*BASE
    uint32_t* digits = malloc((len+1) * sizeof(*digits));
    *digits = len;
    if (len == 1) {
        digits[1] = n;
    } else {
        digits[1] = n % BASE;
        digits[2] = n / BASE;
    }
    return digits;
}

uint32_t __to_decimal(uint32_t* n) 
{
    return (*n == 1) ?
        n[1] : n[2] * BASE + n[1];
}

int __gt(uint32_t* a, uint32_t* b) 
{
    if (*(a) > *(b)) return 1;
    if (*(a) == *(b)) {
        int i = *(a);
        while (i > 0) {
            if (a[i] > b[i]) return 1;
            if (a[i] < b[i]) return 0;
            --i;
        }
    }
    return 0;
}

int __eq(uint32_t* a, uint32_t* b) 
{
    if (*(a) != *(b)) return 0;
    int i = *(a);
    while (i > 0) {
        if (a[i] != b[i]) return 0;
        --i;
    }
    return 1;
}

int __st(uint32_t* a, uint32_t* b) 
{
    return ! __gt(a, b) && ! __eq(a, b);
}

/*
 * Inplace increment. Not thread safe.
 */
void __incr(uint32_t* n) 
{
    for (int i = 1; i <= *n; i++) {
        if (n[i] < BASE - 1) {
            n[i] += 1;
            return;
        } else {
            n[i] = 0;
        }
    }

    n = realloc(n, (*n + 2) * sizeof(*n));
    ++*n;
    n[*n] = 1;
}

/*
 * Inplace decrement, halt at zero. Not thread safe.
 */
void __decr(uint32_t* n) 
{
    if (__is_zero(n)) return;

    for (int i = 1; i <= *n; i++) {
        if (n[i] >= 1) {
            n[i] -= 1;
            if (i > 1 && i == *n && n[i] == 0) --*n;
            return;
        } else {
            n[i] = BASE - 1;
        }
    }
}

int __is_zero(uint32_t* n) 
{
    return *(n) == 1 && n[1] == 0 ? 1 : 0;
}

int __is_one(uint32_t* n) 
{
    return *(n) == 1 && n[1] == 1 ? 1 : 0;
}

uint32_t* __copy_digits(uint32_t* n) 
{
    int32_t len = *(n) + 1; 
    uint32_t* cp_n = malloc(len * sizeof(*cp_n));
    memcpy(cp_n, n, len * sizeof(*cp_n));
    *(cp_n) = *(n);
    return cp_n;
}

/*
 * Returns the number of decimal digits.
 */
uint32_t __len_decimal(uint32_t* digits) 
{
    int32_t len_msb = 0;
    uint32_t msb = digits[*digits];    

    if (msb == 0) return 1;

    while (msb > 0) {
        msb /= 10;
        ++len_msb;
    }
    
    return len_msb + LEN_BASE * (*digits - 1);
}

/*
 * Returns the argument with greater digits length
 */
uint32_t* __arg_len_max(uint32_t* a, uint32_t* b) 
{
    return *(a) > *(b) ? a : b;
}

/*
 * Returns the argument with lesser digits length
 */
uint32_t* __arg_len_min(uint32_t* a, uint32_t* b) 
{
    return *(a) < *(b) ? a : b;
}

uint32_t* __mult(uint32_t* a, uint32_t* b) 
{
    if (__is_one(a)) return __copy_digits(b);
    if (__is_one(b)) return __copy_digits(a);

    int len = *(a) + *(b);
    uint32_t* digits = calloc((len + 1), sizeof(*digits));

    for (int i = 1; i <= *(a); i++) {
        for (int j = 1; j <= *(b); j++) {
            uint64_t tmp = (uint64_t) digits[i+j-1] + (uint64_t) a[i] * (uint64_t) b[j];
            digits[i+j-1] = tmp % BASE;
            digits[i+j] += tmp / BASE;
        }
    }

    *digits = (digits[len] > 0) ? len : len - 1;
    return digits;
}

uint32_t* __add(uint32_t* a, uint32_t* b) 
{
    uint32_t* arg_max = __arg_len_max(a, b);
    uint32_t* arg_min = __arg_len_min(a, b);
    uint32_t max_len = *(arg_max) + 1;
    uint32_t min_len = *(arg_min);

    uint32_t* digits = malloc((max_len + 1) * sizeof(*digits));
    uint32_t carry = 0; 

    *digits = max_len;

    for(int i = 1; i <= max_len; i++) {
        if (i > min_len && carry == 0) {
            memcpy(digits + i, arg_max + i, 
               (max_len - i + 1) * sizeof(*digits));
            --*digits;
            break;
        }

        uint32_t a_i = i > *(a)? 0 : a[i];
        uint32_t b_i = i > *(b)? 0 : b[i];
        uint32_t tmp = carry + a_i + b_i;

        if (tmp >= BASE) {
            digits[i] = tmp - BASE;
            carry = 1;
        } else {
            digits[i] = tmp;
            carry = 0;
        }
    }
    return digits;
}

/*
 * Assuming a > b, as handled by its caller.
 */
uint32_t* __subtr(uint32_t* a, uint32_t* b) 
{
    uint32_t max_len = *(a);
    uint32_t min_len = *(b);

    uint32_t* digits = malloc((max_len + 1) * sizeof(*digits));
    uint32_t carry = 0;

    for(int i = 1; i <= max_len; i++) {
        if (i > min_len && carry == 0) {
            memcpy(digits + i, a + i, 
                   (max_len - i + 1) * sizeof(*digits));
            break;
        }

        uint32_t a_i = i > *(a)? 0 : a[i];
        uint32_t b_i = i > *(b)? 0 : b[i];

        if (a_i - carry < b_i) {
            digits[i] = BASE - carry - b_i + a_i;
            carry = 1;
        } else {
            digits[i] = a_i - carry - b_i;
            carry = 0;
        }
    }

    *digits = max_len;
    for (int i = max_len; i > 1; --i) {
        if (digits[i] != 0) break;
        --*digits;
    }
    return digits;
}

/**
 * The quotian in each digit place will always be smaller than BASE.
 */
struct QuoRem* __single_divmod(uint32_t* n, uint32_t* d) 
{
    struct QuoRem* quorem = malloc(sizeof(*quorem));
    uint32_t* q_tmp = malloc(sizeof(*q_tmp));
    uint32_t* prod = malloc(sizeof(*prod));
    uint32_t steps = 1;
    uint32_t qn;
    uint32_t q;
    
    do {
        free(q_tmp); free(prod); 
        q_tmp = __assign_digits(steps);
        prod = __mult(q_tmp, d);
        qn = steps;

        if (__eq(prod, n)) {
            free(q_tmp); free(prod); 
            quorem->quotian = qn;
            quorem->remainder = __assign_digits(0);
            return quorem;
        }
        steps *= 2;
    } while (__st(prod, n));

    q = qn / 2;
    
    // binary search
    while(qn > q) {
        free(q_tmp); free(prod);
        q_tmp = __assign_digits((qn + q) / 2);
        prod = __mult(q_tmp, d);
        if (qn - q == 1) break;
        if (__gt(prod, n)) {
            qn = __to_decimal(q_tmp);
        } else {
            q = __to_decimal(q_tmp);
        }
    }
    quorem->quotian = q;
    quorem->remainder = __subtr(n, prod);
    free(q_tmp); free(prod);

    return quorem;
}

uint32_t** __divmod(uint32_t* n, uint32_t* m) 
{
    uint32_t** res = malloc(sizeof(*res));
    int len = *(n) - *(m) + 1;
    uint32_t* carry;
    uint32_t* quotian;

    if (__st(n, m)) {
        quotian = __assign_digits(0);
        carry = __copy_digits(n);
        res[0] = quotian;
        res[1] = carry;
        return res;
    }

    uint32_t* n_i = __slice_digits(n, len, *(n) + 1);

    if (! __st(n_i, m)) {
        carry = __assign_digits(0);
    } else {
        --len;
        carry = __right_shift(n_i);
        free(n_i);
        n_i = __slice_digits(n, len, len + 1);
        
    }

    quotian = malloc((len + 1) * sizeof(*quotian));
    *quotian = len;
    
    for (int pos = len; pos >= 1; pos--) {
        uint32_t* sum = __add(n_i, carry);
        struct QuoRem* quorem = __single_divmod(sum, m);

        free(sum);
        free(carry);
        free(n_i);

        quotian[pos] = quorem->quotian;

        if (pos > 1) {
            carry = __right_shift(quorem->remainder);
            n_i = __slice_digits(n, pos-1, pos);
        } else {
            carry = __copy_digits(quorem->remainder);
        }
        free(quorem->remainder);
        free(quorem);
    }

    res[0] = quotian;
    res[1] = carry;
    return res;
}

/***************************** TEST SUITE FUNCTIONS *****************************/

void print_digits(char* var_name, uint32_t* digits) 
{
    printf("%s->digits: ", var_name);
    for (int i = 1; i <= *(digits); i++) {
        printf("s[%d]: %u ", i, digits[i]);
    }
    printf("\n");
}

void debug_digits(char* a, uint32_t* ad,
                  char* b, uint32_t* bd,
                  char* r, uint32_t* rd) 
{
    int n = 0;
    if (*(ad) > n) n =*(ad);
    if (*(bd) > n) n =*(bd);
    if (*(rd) > n) n =*(rd);

    for (int i = n; i >= 1; i--) {
        printf("%s[%d]: %u ", a, i, ad[i]);
        printf("%s[%d]: %u ", b, i, bd[i]);
        printf("%s[%d]: %u ", r, i, rd[i]);
        printf("\n");
    }
}
