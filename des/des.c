//
// Created by amirhossein on 12/15/20.
//


#include "des.h"

const int IP[]={58,50,42,34,26,18,10,2,
                60,52,44,36,28,20,12,4,
                62,54,46,38,30,22,14,6,
                64,56,48,40,32,24,16,8,
                57,49,41,33,25,17,9,1,
                59,51,43,35,27,19,11,3,
                61,53,45,37,29,21,13,5,
                63,55,47,39,31,23,15,7};

const int FP[]={40,8,48,16,56,24,64,32,
                39,7,47,15,55,23,63,31,
                38,6,46,14,54,22,62,30,
                37,5,45,13,53,21,61,29,
                36,4,44,12,52,20,60,28,
                35,3,43,11,51,19,59,27,
                34,2,42,10,50,18,58,26,
                33,1,41,9,49,17,57,25};

const int PC_1[] = {57,49,41,33,25,17,9,
                    1,58,50,42,34,26,18,
                    10,2,59,51,43,35,27,
                    19,11,3,60,52,44,36,
                    63,55,47,39,31,23,15,
                    7,62,54,46,38,30,22,
                    14,6,61,53,45,37,29,
                    21,13,5,28,20,12,4};

const int PC_2[] = {14,17,11,24,1,5,
                    3,28,15,6,21,10,
                    23,19,12,4,26,8,
                    16,7,27,20,13,2,
                    41,52,31,37,47,55,
                    30,40,51,45,33,48,
                    44,49,39,56,34,53,
                    46,42,50,36,29,32};

const int EBIT[] = {32,1,2,3,4,5,
                    4,5,6,7,8,9,
                    8,9,10,11,12,13,
                    12,13,14,15,16,17,
                    16,17,18,19,20,21,
                    20,21,22,23,24,25,
                    24,25,26,27,28,29,
                    28,29,30,31,32,1};

const uint8_t S[][4][16] = { // S1
        {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
                {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
                {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
                {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
        },  //S2
        {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
                {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
                {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
                {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
        },  //S3
        {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
                {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
                {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
                {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
        },  //S4
        {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
                {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
                {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
                {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
        },  //S5
        {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
        },  //S6
        {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
                {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
                {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
                {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
        },  //S7
        {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
                {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
                {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
                {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
        },  //S8
        {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
                {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
                {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
                {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
        }
};

const int P[] = {16,7,20,21,
                 29,12,28,17,
                 1,15,23,26,
                 5,18,31,10,
                 2,8,24,14,
                 32,27,3,9,
                 19,13,30,6,
                 22,11,4,25};

const int shift[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};


uint64_t _des_permutate(uint64_t data,const int* table,int from,int to){
    //generate bit array of data
    uint64_t bitarray[64];
    uint64_t sh = 0x01;
    for(int i = 0 ; i < 64 ; i++){
        bitarray[63-i] = (data & sh) >> i;
        sh = sh << 1;
    }

    //apply permutation table
    uint64_t res = 0;
    const int temp = to - 1;
    for(int i = 0 ; i < to ; i++)
        res |= (bitarray[(64 - from)+(table[i]-1)] << (temp - i));

    return res;
}

uint64_t* des_genKn(uint64_t key){
    // 64-bit key to 56-bit key
    uint64_t kplus = _des_permutate(key,PC_1,64,56);
    //divide it to C(lpt) and D(rpt)
    block* k = _des_ktob(kplus);
    //init an array for storing Kn
    uint64_t* kn = (uint64_t*) malloc(sizeof(uint64_t)*16);
    for(int i = 0 ; i < 16 ; i++){
        //shift each part of key
        k->lpt = _des_kshift(k->lpt,shift[i]);
        k->rpt = _des_kshift(k->rpt,shift[i]);
        //calc CnDn and store in Kn
        kn[i] = _des_btok(k);
        //56-bit key to 48-bit key (mutate using PC-2)
        kn[i] = _des_permutate(kn[i],PC_2,56,48);
    }
    free(k);
    return kn;
}

uint64_t _des_iteration(uint64_t data,uint64_t* kn){
    //divide data to lpt & rpt
    block* d = _des_itob(data);
    //16 iteration
    for(int i = 0 ; i < 16 ; i++){
        //L(n) = R(n-1) & R(n) = L(n-1) + f(R(n-1),Kn)
        uint32_t ln1 = d->lpt;
        d->lpt = d->rpt;
        d->rpt = ln1 ^ _des_ffunc(d->rpt,kn[i]);
    }
    //Concat {R16,L16}
    uint64_t res = _des_btoi(d,1);
    free(d);
    return res;
}

uint32_t _des_ffunc(uint32_t r,uint64_t k){
    uint64_t r64 = r;
    //expanding 32-bit R(n-1) to 48-bit
    r64 = _des_permutate(r64,EBIT,32,48);

    //xor r and k
    r64 = r64 ^ k;

    //divided to 8 6-bit element
    //0x3f == 0011-1111 (6 bit 1) for dividing 6 bit from data
    uint64_t and = 0x3f;
    uint8_t group[8];
    for(int i = 0 ; i < 8 ; i++){
        group[7-i] = (r64 & and) >> (i * 6);
        and = and << 6;

    }

    //generate row and column from data for S-BOX replacement
    uint8_t row[8],column[8];
    for(int i = 0 ; i < 8 ; i++){
        //select last bit and fist bit d & 0b100-000(0x20) and d & 0b000-001(0x01)
        row[i] = ((group[i] & 0x20) >> 4) | (group[i] & 0x01);
        //select remaining bits d & 0b011-110(0x1e)
        column[i] = (group[i] & 0x1e) >> 1;
    }

    // Res = {S1(R1,C1),S2(R2,C2),S3(R3,C3),S4(R4,C4),S5(R5,C5),S6(R6,C6),S7(R7,C7),S8(R8,C8)}
    // like verilog concat
    uint64_t res = 0;
    for(int i = 0 ; i < 8 ; i++) {
        res |= S[7 - i][row[7 - i]][column[7 - i]] << (i * 4);
    }

    res = _des_permutate(res,P,32,32);
    uint32_t rr = res;
    return rr;
}

void des_reverse(uint64_t* kn){
    for(int i = 0 ; i < 8 ; i++){
        uint64_t t = kn[i];
        kn[i] = kn[15-i];
        kn[15-i] = t;
    }
}

uint64_t des_encrypt(uint64_t data,uint64_t* kn){
    uint64_t temp = _des_permutate(data,IP,64,64);
    temp = _des_iteration(temp,kn);
    temp = _des_permutate(temp,FP,64,64);
    return temp;
}

block* _des_itob(uint64_t data){
    block* block1 = (block*) malloc(sizeof(block));
    block1->rpt = data;
    block1->lpt = (data >> 32);
    return block1;
}

uint64_t _des_btoi(block* b,int reverse){
    if(reverse){
        uint64_t res = b->rpt;
        res = res << 32;
        res += b->lpt;
        return res;
    }else{
        uint64_t res = b->lpt;
        res = res << 32;
        res += b->rpt;
        return res;
    }
}

block* _des_ktob(uint64_t data){
    block* block1 = (block*) malloc(sizeof(block));
    block1->rpt = data & 0xfffffff;
    block1->lpt = (data >> 28);
    return block1;
}
uint64_t _des_btok(block* b){
    uint64_t res = b->lpt;
    res = res << 28;
    res += b->rpt;
    return res;
}

uint32_t _des_kshift(uint32_t k,int n){
    uint32_t res = k;
    for(int i = 0 ; i < n ; i++){
        unsigned char lastbit = (res & 0x8000000) >> 27;
        res = ((res << 1) & 0xfffffff) + lastbit;
    }
    return res;
}

void print_in_bits64(uint64_t t){
    unsigned char bitarray[64];
    uint64_t sh = 0x01;
    for(int i = 0 ; i < 64 ; i++){
        bitarray[63-i] = (t & sh) >> i;
        sh = sh << 1;
    }
    int count = 0;
    for(int i = 0 ; i < 64 ; i++){
        printf("%d",bitarray[i]);
        count++;
        if(count == 4 && i != 63){
            printf("-");
            count = 0;
        }
    }
    printf("\n");
}
void print_in_bits32(uint32_t t){
    unsigned char bitarray[32];
    uint32_t sh = 0x01;
    for(int i = 0 ; i < 32 ; i++){
        bitarray[31-i] = (t & sh) >> i;
        sh = sh << 1;
    }
    int count = 0;
    for(int i = 0 ; i < 32 ; i++){
        printf("%d",bitarray[i]);
        count++;
        if(count == 4 && i != 31){
            printf("-");
            count = 0;
        }
    }
    printf("\n");
}

uint64_t des_gen_random_key(){
    time_t t;
    srand((unsigned )time(&t));
    uint8_t temp;
    uint64_t res = 0;
    const int lownum = 0x0 ,highnum = 0xff;
    for(int i = 0 ; i < 8 ; i++){
        temp = (rand() % (highnum - lownum)) + lownum;
        res |= temp << (i*8);
    }
    return res;
}