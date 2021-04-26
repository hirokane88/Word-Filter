#include "ll.h"
#include "speck.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct HashTable {
    uint64_t salt[2]; //hash table seed
    uint32_t size;
    bool mtf; //move-to-front functionality
    LinkedList **lists;
};

struct HashTable *ht_create(uint32_t size, bool mtf) {
    struct HashTable *ht = (struct HashTable *) malloc(sizeof(struct HashTable));
    if (ht) {
        ht->salt[0] = 0x85ae998311115ae3;
        ht->salt[1] = 0xb6fac2ae33a40089;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (struct LinkedList **) calloc(
            size, sizeof(struct LinkedList *)); //allocate the linked lists of the hash table
        if (!ht->lists) { //if the linked list allocation fails
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(struct HashTable **ht) { //free the hash table memory
    free((*ht)->lists);
    free(*ht);
    ht = NULL;
}

uint32_t ht_size(struct HashTable *ht) {
    return ht->size;
}

struct Node *ht_lookup(
    struct HashTable *ht, char *oldspeak) { //look up the hash index of the oldspeak word
    uint32_t i = hash(ht->salt, oldspeak) % ht_size(ht);
    if (ht->lists[i]) { //if the hash index is initialized
        return ll_lookup(ht->lists[i],
            oldspeak); //look through the hash index's linked list for the oldspeak word
    }
    return NULL;
}

void ht_insert(
    struct HashTable *ht, char *oldspeak, char *newspeak) { //insert a words into the hash table
    uint32_t i = hash(ht->salt, oldspeak) % ht_size(ht);
    if (!ht->lists[i]) { // if linked list doesn't exist
        ht->lists[i] = ll_create(ht->mtf);
    }
    ll_insert(ht->lists[i], oldspeak, newspeak);
}

void ht_print(struct HashTable *ht) {
    if (ht->lists) {
        for (uint32_t i = 0; i < ht->size; i++) {
            printf("___________________________\n");
            ll_print(ht->lists[i]);
        }
    }
}
