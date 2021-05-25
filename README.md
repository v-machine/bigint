```                                                                                     
8 888888888o    8 8888     ,o888888o.     8 8888 b.             8 8888888 8888888888 
8 8888    `88.  8 8888    8888     `88.   8 8888 888o.          8       8 8888       
8 8888     `88  8 8888 ,8 8888       `8.  8 8888 Y88888o.       8       8 8888       
8 8888     ,88  8 8888 88 8888            8 8888 .`Y888888o.    8       8 8888       
8 8888.   ,88'  8 8888 88 8888            8 8888 8o. `Y888888o. 8       8 8888       
8 8888888888    8 8888 88 8888            8 8888 8`Y8o. `Y88888o8       8 8888       
8 8888    `88.  8 8888 88 8888   8888888  8 8888 8   `Y8o. `Y8888       8 8888       
8 8888      88  8 8888 `8 8888       .8'  8 8888 8      `Y8o. `Y8       8 8888       
8 8888    ,88'  8 8888    8888     ,88'   8 8888 8         `Y8o.`       8 8888       
8 888888888P    8 8888     `8888888P'     8 8888 8            `Yo       8 8888       

8888888888888888888888888888888    DEV IN PROGRESS    888888888888888888888888888888
```

# Overview
BigInt is a C library for computing integers with arbitrary sizes. BigInt is currently in active development.

# Motivation
Built-in integer types in C are limited by the size of the register. The built-in types should suffice for most general purpose programming. However, when it comes to modern cryptography, the need to compute with primes with hundreds of digits dwarfs even the `long long` type. Thus, the need for a big integer library seemed justified.

Noob to C and facinated by crytography, I have decided to write a big integer library as an excuse to practice C programming and to delve deeper into computer systems. A byproduct of learning, this codebase is (and continutes to be) rough on the edges, but hopefully less so in each succesive iterations. 

# How It Works
A naive approach will be to compute with an array of base ten digits, but this is a blatant waste of space and compute power, as each digit takes up only 4% of the smallest unsigned integer (8 bit). Moreover, computing with single digits as opposed to the entire register word size massively worsen time complexity by 2^30 (x86 architecture) or 2^62 times (x64 architecture). 

Inspired by the way CPython handles big numbers (PEP 237), the BigInt type is structured as following:
```
struct BigInt
{
	int32_t sign_len;
	uint32_t* digits;
};
```
The first field contains the both the sign and the number of digits, while the second an array of the actual digits. To efficiently convert a large integer to BigInt and vice versa, BigInt uses Base-giga (2^30) digits. 

BigInt currently supports the following operations (in-progress API):

```
/* constructor */
BigInt* bigint_init(char* sn);
BigInt* bigint_int_init(int32_t n);

/* string representation */
char* bigint_to_str(BigInt* n);

/* arithmetic operations */
BigInt* bigint_add(BigInt* a, BigInt* b);
BigInt* bigint_subtr(BigInt* a, BigInt* b);
BigInt* bigint_mult(BigInt* a, BigInt* b);
BigInt* bigint_div(BigInt* n, BigInt* d);
BigInt* bigint_mod(BigInt* n, BigInt* m);
BigInt* bigint_log(BigInt* n, BigInt* b);
BigInt* bigint_power_mod (BigInt* base, BigInt* exponent, BigInt* modulo);
BigInt* bigint_abs(BigInt* n);
BigInt* bigint_neg(BigInt* n);

/* comparisons */
int bigint_st(BigInt* a, BigInt* b);
int bigint_gt(BigInt* a, BigInt* b);
int bigint_eq(BigInt* a, BigInt* b);

/* utilities */
BigInt* bigint_copy(BigInt* n);
uint32_t bigint_hash(void* n);

/* destructor */
void bigint_free(BigInt** n);
```

# Usage
Coming soon.

# Known Issues
- bigint_power_mod runs noticibly slower than CPython built-in pow() method. This might be caused by memory mismanagement, specifically heap fragmentation and unnecessary heap allocation that can be handled on the stack. 

- BigInt requires manual heap deallocation, which forces nested statemnt to be fragmented in order to keep track of every reference of BigInt.

# Road Map
- [x] bigint v2.0 prototype implemented
- [x] internal data structure implemented (hashmap)
- [x] home-brew unit testing framework sunittest implemented
- [x] uint tested internal functions
- [ ] uint tested API functions
- [ ] performance analysis 
- [ ] demos created
- [ ] bigint v3.0 prototype (with built-in GC) started
- [ ] ...
