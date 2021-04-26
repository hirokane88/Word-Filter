#include "bf.h"
#include "bv.h"
#include "hash.h"
#include "ll.h"
#include "node.h"
#include "parser.h"

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define WORD "[a-zA-Z0-9]+(('|-)[a-zA-Z0-9]*)*"

void print_r(struct LinkedList *l) { //print message for the case of oldspeak
    printf("Dear Comrade,\n\nSubmitting your text helps to preserve feelings and "
           "prevent \nbadthink. Some of the words that you used are not "
           "goodspeak.\nThe list shows how to turn oldspeak into newspeak.");
    printf("\n\n");
    ll_print(l); //print the list of oldspeak
}

void print_t(struct LinkedList *l) { //print message for the case of badspeak
    printf("Dear Comrade,\n\nYou have chosen to use degenerate words that may "
           "cause hurt \nfeelings or cause your comrades to think unpleasant "
           "thoughts.\nThis is doubleplus bad. To correct your wrongthink and "
           "\npreserve community consesus we will be sending you to joycamp "
           "\nadministered by Medellin's Miniluv. Beware of the hippos.");
    printf("\n\nYour errors:\n\n");
    ll_print(l); //print the list of badspeak
}

void print_tr(struct LinkedList *t,
    struct LinkedList *r) { //print message for the case of badspeak and oldspeak
    printf("Dear Comrade,\n\nYou have chosen to use degenerate words that may "
           "cause hurt \nfeelings or cause your comrades to think unpleasant "
           "thoughts.\nThis is doubleplus bad. To correct your wrongthink and "
           "\npreserve community consesus we will be sending you to joycamp "
           "\nadministered by Medellin's Miniluv. Beware of the hippos.");
    printf("\n\nYour errors:\n\n");
    ll_print(t);
    printf("\n\nThink of these words on your vacation!\n\n");
    ll_print(r); //print the list of badspeak and oldspeak
}

int main(int argc, char **argv) {
    FILE *badspeak = fopen("badspeak.txt", "r");
    FILE *newspeak = fopen("newspeak.txt", "r");

    uint32_t hash_size = 10000; //set default hash size
    uint32_t bloom_size = 1048576; //set default bloom size to 2^20
    bool mtf = false;

    int opt = 0; //command line options
    while ((opt = getopt(argc, argv, "hfm")) != -1) {
        switch (opt) {
        case 'h':
            if ((argv[optind] != NULL) && (atoi(argv[optind]) != 0)) { //set hash size
                hash_size = atoi(argv[optind]);
            }
            break;
        case 'f':
            if ((argv[optind] != NULL) && (atoi(argv[optind]) != 0)) { //set bloom size
                bloom_size = atoi(argv[optind]);
            }
            break;
        case 'm': mtf = true; break; //set move-to-front functionality
        default: fprintf(stderr, "Usage: %s -[hfm]\n", argv[0]); exit(1); //default error message
        }
    }

    BloomFilter *bf = bf_create(bloom_size); //initialize a bloom filter
    HashTable *ht = ht_create(hash_size, mtf); //initialize a hash table

    char badword[10000];
    char goodword[10000];
    while (fscanf(badspeak, "%s", badword)
           != -1) { //insert badtext.txt badwords into the bloom filter and hash table
        bf_insert(bf, badword);
        ht_insert(ht, badword, NULL);
    }
    while (
        fscanf(newspeak, "%s %s", badword, goodword)
        != -1) { //insert newspeak.txt oldspeak-->newspeak pairs into the bloom filter and hash table
        bf_insert(bf, badword);
        ht_insert(ht, badword, goodword);
    }
    fclose(badspeak);
    fclose(newspeak);

    regex_t re;
    LinkedList *t_crime = ll_create(true); //list to hold all badspeak words
    LinkedList *r_speak = ll_create(true); //list to hold all oldspeak-->newspeak pairs

    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex. \n");
        exit(1);
    }
    char *word = NULL;
    while ((word = next_word(stdin, &re))
           != NULL) { //read user input filtered out by the regular expression
        if (bf_probe(bf, word)) {
            Node *search = ht_lookup(ht, word);
            if (search != NULL) { //if the word exists in the hash table
                if (search->newspeak == NULL) { //if badspeak
                    ll_insert(t_crime, search->oldspeak, NULL);
                } else { //if oldspeak
                    ll_insert(r_speak, search->oldspeak, search->newspeak);
                }
            }
        }
    }

    if ((ll_length(t_crime) != 0)
        && (ll_length(r_speak) == 0)) { //if only badspeak words were inputted
        print_t(t_crime);
    }
    if ((ll_length(t_crime) != 0)
        && (ll_length(r_speak) != 0)) { //if both badspeak and oldspeak were inputted
        print_tr(t_crime, r_speak);
    }
    if ((ll_length(t_crime) == 0)
        && (ll_length(r_speak) != 0)) { //if only oldspeak words were inputted
        print_r(r_speak);
    }

    clear_words();
    regfree(&re);
    ht_delete(&ht);
    bf_delete(&bf);
    ll_delete(&t_crime);
    ll_delete(&r_speak);
    exit(1);
}
