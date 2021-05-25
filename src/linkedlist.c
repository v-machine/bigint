/**
 * @file linkedlist.c
 * @brief Implementation of a generic linked list
 *
 * @author Vincent Mai
 * @version 0.5.0
 */

#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Private Struct */
struct Node {
    void* data;
    Node* next;
};

/** Private Functions */
Node* _in_linkedlist(LinkedList* head, void* key); // deprecated
void _free_node(Node* node);

LinkedList* linkedlist_init(int (*cmp_data)(void*, void*), 
                            char* (*repr_data)(void*)) {
    LinkedList* list = malloc(sizeof(*list));
    list->repr_data = repr_data;
    list->cmp_data = cmp_data;
    list->head = NULL;
    return list;
}

void linkedlist_push(LinkedList* list, void* data) {
    Node* new_node = malloc(sizeof(*new_node));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
}

/** deprecated */
void linkedlist_update(LinkedList* list, void* data) {
    Node* node = _in_linkedlist(list, data);
    if (! node) linkedlist_push(list, data);
    else node->data = data;
}

void* linkedlist_iter(LinkedList* list) {
    Node* iter = list->head;
    return iter;
}

void* linkedlist_next(Iterator** iter) {
    if ((*iter)->data) {
        void* data = (*iter)->data;
        *iter = (*iter)->next;
        return data;
    }
    return NULL;
}

void linkedlist_print(LinkedList* list) {
    if(! list->head) {
        printf("Linked list is empty.\n");
        return;
    }
    printf("start");
    Node* head = list->head;
    while(head) {
        char* s_data = list->repr_data(head->data);
        printf(" -> %s", s_data);
        free(s_data);
        head = head->next;
    }
    printf(" -> NULL\n");
}

void* linkedlist_pop(LinkedList* list) {
    if(! list->head) return NULL;
    Node* removed = list->head;
    list->head = removed->next;
    void* data = removed->data;
    free(removed);
    return data;
}

int linkedlist_has(LinkedList* list, void* data) {
    Node* node = list->head;
    while(node) {
        if (list->cmp_data(data, node->data))
            return 1;
        node = node->next;
    }
    return 0;
}

void linkedlist_free(LinkedList** list) {
    _free_node((*list)->head);
    free(*list);
    *list = NULL;
}

/****************************** PRIVATE FUNCTIONS *****************************/

/**
 * Deprecated!
 * Returns the node containing the data if found in the linked list.
 * Returns a NULL pointer if data is not found.
 */
Node* _in_linkedlist(LinkedList* list, void* data) {
    Node* node = list->head;
    while(node) {
        if (list->cmp_data(data, node->data))
            return node;
        node = node->next;
    }
    return NULL;
}

/**
 * Frees all nodes from the linked list.
 */
void _free_node(Node* node) {
    if (node->next) {
        _free_node(node->next);
        free(node);
    }
}
