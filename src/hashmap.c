/**
 * @brief This file contains the implementation of a generic hashmap.
 *
 * @author Vincent Mai
 * @version 0.5.0
 */

#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** private struct */
struct Node {
	void* key;
	void* val;
	Node* next;
};

/** private functions */
uint32_t __hash(HashMap* hmap, uint32_t pre_hash);
Node* __node_find(HashMap* hmap, Node* node, void* key);
void __init_map(HashMap* hmap, uint32_t size);
void __node_push(Node** node, void* key, void* val);
void __node_del(HashMap* hmap, Node** node, void* key);
void __node_free(Node** node);
void __node_repr(HashMap* hmap, Node* node);
void __key_err(HashMap* hmap, void* key);

HashMap* hashmap_init(uint32_t size,
					  int (*cmp_key)(void*, void*),
					  uint32_t (*hash)(void*),
					  char* (*repr_key)(void*),
					  char* (*repr_val)(void*)) {

	HashMap* hmap = malloc(sizeof(*hmap));
	__init_map(hmap, size);
	hmap->size = size;
	hmap->cmp_key = cmp_key;
	hmap->hash = hash;
	hmap->repr_key = repr_key;
	hmap->repr_val = repr_val;
	// hmap->map = malloc(size * sizeof(*(hmap->map)));
	return hmap;
}

void hashmap_insert(HashMap* hmap, void* key, void* val) {
	uint32_t map_idx = __hash(hmap, hmap->hash(key));
	if (! hmap->map[map_idx]) {
		Node* node = NULL;
		__node_push(&node, key, val);
		hmap->map[map_idx] = node; 
	} else {
		Node* found = __node_find(hmap, hmap->map[map_idx], key);
		if (! found)
			__node_push(&(hmap->map[map_idx]), key, val);
		else 
			found->val = val;
	}
}

void* hashmap_get(HashMap* hmap, void* key) {
	uint32_t map_idx = __hash(hmap, hmap->hash(key));
	Node* res = __node_find(hmap, hmap->map[map_idx], key);
	return (res) ? res->val : NULL;
}

void hashmap_free(HashMap** hmap) {
	for (int i = 0; i < (*hmap)->size; i++)
		__node_free(&(*hmap)->map[i]);
	free(*hmap);
	*hmap = NULL;
}

void hashmap_remove(HashMap* hmap, void* key) {	
	uint32_t map_idx = __hash(hmap, hmap->hash(key));
    __node_del(hmap, &(hmap->map[map_idx]), key);
}

void hashmap_print(HashMap* hmap) {
	for (int i = 0; i < hmap->size; i++) {
		__node_repr(hmap, hmap->map[i]);
	}
}

/***************************** PRIVATE FUNCTIONS ******************************/
/**
 * Initializes all map entries to NULL
 */
void __init_map(HashMap* hmap, uint32_t size) {
	hmap->map = malloc(size * sizeof(*(hmap->map)));
	for (int i = 0; i < size; i++) {
		hmap->map[i] = NULL;
	}
}
/**
 * Returns a hash based on the pre_hash.
 */
uint32_t __hash(HashMap* hmap, uint32_t pre_hash) {
	uint32_t map_idx = pre_hash % hmap->size;
	return map_idx;
}

/**
 * Pushes a new node to the front of the linked list.
 */
void __node_push(Node** node, void* key, void* val) {
	Node* new_node = malloc(sizeof(*new_node));
	new_node->key = key;
	new_node->val = val;
	new_node->next = *node;
	*node = new_node;
}

/**
 * Given a key, returns a node if found in the linked list.
 */
Node* __node_find(HashMap* hmap, Node* node, void* key) {
	while (node) {
		if (hmap->cmp_key(node->key, key))
			return node;
		node = node->next;
	}
	return NULL;
}

/** 
 * Given a key, deletes the node that contains the key.
 */
void __node_del(HashMap* hmap, Node** node, void* key) {
	Node* head = *node;
	Node* pre_node = *node;
	Node** cur_node = node;
	while (*cur_node) {
		if (hmap->cmp_key((*cur_node)->key, key)) {
			if (pre_node == *cur_node) {
				Node* removed = *cur_node;
				*cur_node = (*cur_node)->next;
				free(removed);
			} else {
				pre_node->next = (*cur_node)->next;
				free(*cur_node);
				*cur_node = NULL;
				*node = head;
			}
			return;
		}
		pre_node = *cur_node;
		*cur_node = (*cur_node)->next;
	}
	__key_err(hmap, key);
}

/** 
 * Frees all node in a linked list
 */
void __node_free(Node** node) {
	if (*node) __node_free(&((*node)->next));
	free(*node);
	*node = NULL;
}

/** 
 * Returns a representation of the nodes in the linked list.
 */
void __node_repr(HashMap* hmap, Node* node) {
	while (node) {
		char* s_k = hmap->repr_key(node->key);
		char* s_v = hmap->repr_val(node->val);
		printf("(%s : %s)\n", s_k, s_v);
		free(s_k); free(s_v);
		node = node->next;
	}
}

/**
 * Exits with error if key is not found.
 */
void __key_err(HashMap* hmap, void* key) {
	char* s_k = hmap->repr_key(key);
	printf("Key '%s' not found.\n", s_k);
	free(s_k);
	exit(1);
}
