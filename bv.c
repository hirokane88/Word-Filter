#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

struct BitVector *bv_create(uint32_t length) {
    struct BitVector *bv = (struct BitVector *) calloc(1, sizeof(struct BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc(
            ((length / 8) + 1), sizeof(uint8_t)); //allocate length number of vectors
        if (bv->vector) {
            return bv;
        } else {
            free(bv);
            return NULL;
        }
    } else {
        return NULL;
    }
}

void bv_delete(struct BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

uint32_t bv_length(struct BitVector *bv) {
    return bv->length;
}

void bv_set_bit(struct BitVector *bv, uint32_t i) { //set a bit in the vector
    uint8_t byte_i = bv->vector[i / 8]; //byte index
    uint8_t bit_i = i % 8; //bit index
    uint8_t mask = 1 << bit_i;
    bv->vector[i / 8] = byte_i | mask;
}

void bv_clr_bit(struct BitVector *bv, uint32_t i) { //clear a bit in the vector
    uint8_t byte_i = bv->vector[i / 8]; //byte index
    uint8_t bit_i = i % 8; //bit index
    uint8_t mask = ~(1 << bit_i);
    bv->vector[i / 8] = byte_i & mask;
}

uint8_t bv_get_bit(struct BitVector *bv, uint32_t i) { //get a bit's value from the vector
    uint8_t byte_i = bv->vector[i / 8]; //byte index
    uint8_t bit_i = i % 8; //bit index
    uint8_t mask = 1 << bit_i;
    return (byte_i & mask) >> bit_i;
}

void bv_print(struct BitVector *bv) {
    uint32_t temp;
    for (uint32_t i = 0; i < bv->length; i++) {
        temp = i + 1;
        printf("%d", bv_get_bit(bv, i));
        if ((i + 1) % 8 == 0 && i != 0) {
            printf("\n");
        }
    }
    if (temp % 8 == 0) {
        printf("\n");
    } else {
        printf("\n\n");
    }
}
