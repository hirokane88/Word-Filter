#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

struct LinkedList *ll_create(bool mtf) {
    struct LinkedList *ll = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    ll->length = 0;
    ll->head = node_create("null_head", NULL); //create the head node of the linked list
    ll->tail = node_create("null_tail", NULL); //create the tail node of the linked list
    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;
    ll->mtf = mtf; //initialize the move-to-front functionality
    return ll;
}

void ll_delete(struct LinkedList **ll) { //free allocated linked list memory
    free(*ll);
    *ll = NULL;
}
uint32_t ll_length(struct LinkedList *ll) {
    return ll->length;
}

struct Node *ll_lookup(
    struct LinkedList *ll, char *oldspeak) { //search for oldspeak in the linked list
    Node *i_node = ll->head->next; //start the list iterator at the head of the list
    while (i_node != ll->tail) { //iterate through the entire list

        if (strcmp(i_node->oldspeak, oldspeak) == 0) { //if oldspeak is found in list
            if (ll->mtf == true) { //if true, move the node to the front of the list
                i_node->prev->next = i_node->next;
                i_node->next->prev = i_node->prev;
                i_node->next = ll->head->next;
                i_node->prev = ll->head;
                ll->head->next->prev = i_node;
                ll->head->next = i_node;
            }
            return i_node;
        }

        i_node = i_node->next; //increament the list iterator
    }
    i_node = NULL;
    return i_node;
}

void ll_insert(struct LinkedList *ll, char *oldspeak, char *newspeak) { //insert a node in the list
    Node *n = node_create(oldspeak, newspeak);
    n->prev = ll->head;
    n->next = ll->head->next;
    ll->head->next->prev = n;
    ll->head->next = n;
    ll->length += 1;
}

void ll_print(struct LinkedList *ll) {
    if (ll) {
        Node *i_node = ll->head;
        while (i_node->next != ll->tail) {
            node_print(i_node->next);
            i_node = i_node->next;
        }
    }
}
