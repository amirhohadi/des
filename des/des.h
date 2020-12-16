//
// Created by amirhossein on 12/15/20.
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#ifndef DES_DES_H
#define DES_DES_H

struct block{
    uint32_t lpt;
    uint32_t rpt;
};
typedef struct block block;


extern const int IP[];

extern const int FP[];

extern const int PC_1[];

extern const int PC_2[];

extern const int EBIT[];

extern const uint8_t S[][4][16];

extern const int P[];

extern const int shift[];

extern uint64_t _des_permutate(uint64_t data,const int* mask,int from,int to);
extern uint32_t _des_ffunc(uint32_t r,uint64_t k);
extern uint64_t* des_genKn(uint64_t key);
extern uint64_t _des_iteration(uint64_t data,uint64_t* kn);
extern uint32_t _des_kshift(uint32_t k,int n);
extern void des_reverse(uint64_t* kn);
extern uint64_t des_encrypt(uint64_t data,uint64_t* kn);
extern block* _des_itob(uint64_t data);
extern uint64_t _des_btoi(block* b,int reverse);
extern block* _des_ktob(uint64_t key);
extern uint64_t _des_btok(block* b);
extern void print_in_bits64(uint64_t t);
extern void print_in_bits32(uint32_t t);

uint64_t des_gen_random_key();

#endif //DES_DES_H
