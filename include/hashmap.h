/**
 * @brief This file contains function prototypes and structs
 *        of a generic hashmap.
 *
 * This hashmap will accept any user-defined data. When supplying data, user 
 * must define their own functions for comparing two keys, for hashing the keys
 * and for representing the entry (key, value pair) as a char*. These functions
 * will be passed into the initialization of a hashmap, and should have
 * identical argument and return types as following:
 * 
 *         int my_cmp_key(void*, void*);
 *         uint32_t my_hash(void*);
 *         char* my_repr(void*, void*);
 *
 * User will be responsible for GC on any heap allocated data that are passed 
 * into a linked list.
 *
 * @author Vincent Mai
 * @version 0.5.0
 */

#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
typedef struct HashMap HashMap;
typedef struct Node Node;

/**
 * @brief Structure for the hashmap.
 */
struct HashMap {
    uint32_t size;                 /**< The size of the hashmap */
    int (*cmp_key)(void*, void*);  /**< func to compare keys */ 
    uint32_t (*hash)(void*);       /**< func to hash keys */
    char* (*repr_key)(void*);      /**< func to represent key as string */
    char* (*repr_val)(void*);      /**< func to represent key as string */
    Node** map;                    /**< an array of chained nodes */
};

/**
 * @brief Initializes a hashmap.
 *
 * Hashmap requires manual memory deallocation by calling hashmap_empty().
 *
 * @param size The size of the hashmap.
 * @param cmp_key The user-defined function for comparing two keys.
 * @param hash The user-defined hash function for keys.
 * @param repr_key The user-defined string representation of key.
 * @param repr_val The user-defined string representation of val.
 */
HashMap* hashmap_init(uint32_t size,
                      int (*cmp_key)(void*, void*),
                      uint32_t (*hash)(void*),
                      char* (*repr_key)(void*),
                      char* (*repr_val)(void*));

/**
 * @brief Inserts a new key, value pair into the hashmap.
 *
 * If key is already present, the value will be overwritten.
 *
 * @param hmap The pointer to the hashmap.
 * @param The key to be inserted into the hashmap.
 * @param The value to be inserted into the hashmap.
 */
void hashmap_insert(HashMap* hmap, void* key, void* val);

/**
 * @brief Retrieves a value by a key.
 *
 * Returns NULL if key is not found.
 *
 * @param hmap The pointer to the hashmap.
 * @param The key for retriving the value.
 */
void* hashmap_get(HashMap* hmap, void* key);

/**
 * @brief Frees the entire hashmap from heap.
 * 
 * @param hmap The double pointer to the hashmap.
 */
void hashmap_free(HashMap** hmap);

/**
 * @brief Removes an entry by the input key.
 *
 * Exits with errno 1 if key is not found.
 * 
 * @param hmap The pointer to the hashmap.
 * @param The key for removing the entry.
 */
void hashmap_remove(HashMap* hmap, void* key);

/**
 * @brief Prints the hashmap entries one per line.
 * 
 * @param hmap The pointer to the hashmap.
 */
void hashmap_print(HashMap* hmap);

/**
 * @brief Empties an entire hashmap
 * 
 * @param hmap The pointer to the hashmap.
 */
void hashmap_clear(HashMap* hmap);

#endif // HASHMAP_H
