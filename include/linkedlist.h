/**
 * @file linkedlist.h
 * @brief Function prototypes and struct for a generic 
 * 		  linked list implementation.
 *
 * This linked list will accept any user-defined data. When supplying data for
 * the linked list, user must define their own functions for comparing two data
 * instances, and for representing the data as a char array.  These functions
 * will be passed into the initialization of a linked list, and should have
 * identical argument and return types as the following example:
 * 
 * 		int my_cmp_data(void*, void*);
 * 		void my_repr_data(void*);
 *
 * User will be responsible for GC on any heap allocated data that are passed 
 * into a linked list.
 *
 * @author Vincent Mai
 * @version 0.5.0
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct LinkedList LinkedList;
typedef struct Node Node;
typedef struct Node Iterator;

/**
 * @brief Structure for a linked list.
 */
struct LinkedList {
	int (*cmp_data)(void*, void*); /**< User-defined func to  compare data*/
	char* (*repr_data)(void*);     /**< User-defined func to print data */
	Node* head; 		           /**< The head of the linked list */
};

/**
 * @brief Initializes a empty linked list.
 *
 * When done, call linkedlist_free() to remove linked list from heap.
 *
 * @param cmp_data Pointer of function to compare two data instances.
 * @param repr_data Pointer of function to create string representation.
 * @return A pointer to the initialized linked list.
 */
LinkedList* linkedlist_init(int (*cmp_data)(void*, void*), 
						    char* (*repr_data)(void*));

/**
 * @brief Adds a new data to the head of the link list.
 *
 * @param list A pointer to the linked list.
 * @param data The data to be added to the linked list.
 */
void linkedlist_push(LinkedList* list, void* data);

/**
 * @deprecated
 * @brief Updates a given data within the linked list.
 *
 * If not found in the linked list, the data inserted to the front.
 * If the linked list contains multiple identical data, then only
 * the first one will be updated.
 *
 * @param list A pointer to the linked list.
 * @param data The data to be added to the linked list.
 */
void linkedlist_update(LinkedList* list, void* data);

/**
 * @brief Prints the entire linked list.
 *
 * @param list A pointer to the linked list.
 */
void linkedlist_print(LinkedList* list);

/**
 * @brief Frees all nodes in the linked list from heap.
 *
 * @param list A pointer to the linked list.
 */
void linkedlist_free(LinkedList** list);

/**
 * @brief Removes the head of the linked list.
 *
 * Returned data requires manual typecasting.
 *
 * @param list A double pointer to the linked list.
 * @returns the data stored in the head of the list.
 */
void* linkedlist_pop(LinkedList* list);

/**
 * @brief Checks membership of a data within the linked list.
 *
 * @param list A pointer to the linked list.
 * @returns 1 if data in list, or 0 otherwise.
 */
int linkedlist_has(LinkedList* list, void* data);

/**
 * @brief Returns an iterator of the linked list.
 *
 * @param list A pointer to the linked list.
 * @returns An iterator of the linked list.
 */
void* linkedlist_iter(LinkedList* list);

/**
 * @brief Returns the next item from the iterator
 *
 * @param list A double pointer to an iterator of the linked list.
 * @returns An iterator of the linked list.
 */
void* linkedlist_next(Iterator** iter);

#endif // LINKEDLIST_H
