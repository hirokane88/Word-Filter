#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char *oldspeak;
    char *newspeak;
    struct Node *next;
    struct Node *prev;
};

char *s_copy(char *s) { //clone a string
    if (s == NULL) {
        char *c = NULL;
        return c;
    }
    char *c = (char *) calloc(1, sizeof(s)); //allocate memory for the clone
    for (uint64_t i = 0; i < sizeof(s); i += 1) {
        c[i] = s[i];
    }
    return c; //return the cloned string
}

struct Node *node_create(char *oldspeak, char *newspeak) {
    struct Node *n = (struct Node *) calloc(1, sizeof(struct Node));
    if (n) {
        if (oldspeak != NULL && newspeak != NULL) { //if oldspeak
            n->oldspeak = s_copy(oldspeak);
            n->newspeak = s_copy(newspeak);
        } else if (oldspeak != NULL) { //if badspeak
            n->oldspeak = s_copy(oldspeak);
        } else { //if words are NULL
            free(n);
        }
    } else {
        ;
    }
    return n;
}

void node_delete(struct Node **n) { //free allocated node memory
    free((*n)->newspeak);
    free((*n)->oldspeak);
    free(*n);
    *n = NULL;
}

void node_print(struct Node *n) {
    if (n->newspeak != NULL & n->oldspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    if (n->newspeak == NULL & n->oldspeak != NULL) {
        printf("%s\n", n->oldspeak);
    }
}
