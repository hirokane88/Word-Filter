#include "bv.h"
#include "speck.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter; //underlying ADT of the bloomfilter
};

struct BloomFilter *bf_create(uint32_t size) {
    struct BloomFilter *bf = (struct BloomFilter *) malloc(sizeof(struct BloomFilter));
    if (bf) {
        bf->primary[0] = 0x02d232593fbe42ff; //set the seeds for the hash function
        bf->primary[1] = 0x3775cfbf0794f152;
        bf->secondary[0] = 0xc1706bc17ececc04;
        bf->secondary[1] = 0xe9820aa4d2b8261a;
        bf->tertiary[0] = 0xd37b01df0ae8f8d0;
        bf->tertiary[1] = 0x911d454886ca7cf7;
        bf->filter = bv_create(size); //initialize the underlying bitvectors of the bloomfilter
        if (!bf->filter) { //if bitvector creation fails
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

void bf_delete(struct BloomFilter **bf) {
    free(*bf);
    *bf = NULL;
}

uint32_t bf_length(struct BloomFilter *bf) {
    return bv_length(bf->filter);
}

void bf_insert(struct BloomFilter *bf, char *oldspeak) {
    uint32_t a = hash(bf->primary, oldspeak)
                 % bv_length(bf->filter); //hash the oldspeak word with seed primary
    uint32_t b = hash(bf->secondary, oldspeak) % bv_length(bf->filter); //seed secondary
    uint32_t c = hash(bf->tertiary, oldspeak) % bv_length(bf->filter); //seed tertiary
    bv_set_bit(bf->filter, a); //set the hash index bits
    bv_set_bit(bf->filter, b);
    bv_set_bit(bf->filter, c);
}

bool bf_probe(struct BloomFilter *bf, char *oldspeak) {
    uint32_t a
        = hash(bf->primary, oldspeak)
          % bv_length(bf->filter); //hash the oldspeak word using the three different hash seeds
    uint32_t b = hash(bf->secondary, oldspeak) % bv_length(bf->filter);
    uint32_t c = hash(bf->tertiary, oldspeak) % bv_length(bf->filter);
    uint8_t a_val = bv_get_bit(bf->filter, a); //get the bit values of the hashed indexes
    uint8_t b_val = bv_get_bit(bf->filter, b);
    uint8_t c_val = bv_get_bit(bf->filter, c);
    if (a_val == b_val == c_val == 1) { //if all hash indexes are set
        return true;
    } else {
        return false;
    }
}

void bf_print(struct BloomFilter *bf) {
    bv_print(bf->filter);
}
