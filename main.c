/*
 * @Author(Amir Hossein Ohadi)
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "gopt/gopt.h"

int main(int argc,char** argv) {

    void* options = gopt_sort(&argc,argv,gopt_start(
            gopt_option('h',GOPT_ONCE,gopt_shorts('h','?'),gopt_longs("help")),
            gopt_option('g',GOPT_ARG,gopt_shorts('g'),gopt_longs("gen-key")),
            gopt_option('e',GOPT_ARG,gopt_shorts('e'),gopt_longs("encrypt")),
            gopt_option('d',GOPT_ARG,gopt_shorts('d'),gopt_longs("decrypt"))
            ));

    if(gopt(options,'h')){
        fprintf(stdout,"%s\n",man);
        exit(EXIT_SUCCESS);
    }
    else if(gopt(options,'g')){
        const char* file = gopt_arg_i(options,'g',0);
        if(file == NULL){
            fprintf(stderr,"-g must have a argument.\n");
            exit(EXIT_FAILURE);
        }
        int res = handle_gen_key(file);
        handle_res(res);
    }
    else if(gopt(options,'e')){
        if(argc != 3){
            fprintf(stderr,"the argument has not enough options.\n");
            exit(EXIT_FAILURE);
        }
        const char* inputfile = gopt_arg_i(options,'e',0);
        const char* keyfile = argv[1];
        const char* outputfile = argv[2];
        int res = handle_encryption(inputfile,keyfile,outputfile);
        handle_res(res);

    }
    else if(gopt(options,'d')){
        if(argc != 3){
            fprintf(stderr,"the argument has not enough options.\n");
            exit(EXIT_FAILURE);
        }
        const char* inputfile = gopt_arg_i(options,'d',0);
        const char* keyfile = argv[1];
        const char* outputfile = argv[2];

        int res = handle_decryption(inputfile,keyfile,outputfile);
        handle_res(res);
    }
    else handle_wrong_arg();
    return 0;
}


uint64_t permutate(uint64_t data,const int* table,int from,int to){
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

uint64_t* genKn(uint64_t key){
    // 64-bit key to 56-bit key
    uint64_t kplus = permutate(key,PC_1,64,56);
    //divide it to C(lpt) and D(rpt)
    block* k = ktob(kplus);
    //init an array for storing Kn
    uint64_t* kn = (uint64_t*) malloc(sizeof(uint64_t)*16);
    for(int i = 0 ; i < 16 ; i++){
        //shift each part of key
        k->lpt = kshift(k->lpt,shift[i]);
        k->rpt = kshift(k->rpt,shift[i]);
        //calc CnDn and store in Kn
        kn[i] = btok(k);
        //56-bit key to 48-bit key (mutate using PC-2)
        kn[i] = permutate(kn[i],PC_2,56,48);
    }
    free(k);
    return kn;
}

uint64_t iteration(uint64_t data,uint64_t* kn){
    //divide data to lpt & rpt
    block* d = itob(data);
    //16 iteration
    for(int i = 0 ; i < 16 ; i++){
        //L(n) = R(n-1) & R(n) = L(n-1) + f(R(n-1),Kn)
        uint32_t ln1 = d->lpt;
        d->lpt = d->rpt;
        d->rpt = ln1 ^ ffunc(d->rpt,kn[i]);
    }
    //Concat {R16,L16}
    uint64_t res = btoi(d,1);
    free(d);
    return res;
}

uint32_t ffunc(uint32_t r,uint64_t k){
    uint64_t r64 = r;
    //expanding 32-bit R(n-1) to 48-bit
    r64 = permutate(r64,EBIT,32,48);

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

    res = permutate(res,P,32,32);
    uint32_t rr = res;
    return rr;
}

void reverse(uint64_t* kn){
    for(int i = 0 ; i < 8 ; i++){
        uint64_t t = kn[i];
        kn[i] = kn[15-i];
        kn[15-i] = t;
    }
}

uint64_t encrypt(uint64_t data,uint64_t* kn){
    uint64_t temp = permutate(data,IP,64,64);
    temp = iteration(temp,kn);
    temp = permutate(temp,FP,64,64);
    return temp;
}

block* itob(uint64_t data){
    block* block1 = (block*) malloc(sizeof(block));
    block1->rpt = data;
    block1->lpt = (data >> 32);
    return block1;
}

uint64_t btoi(block* b,int reverse){
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

block* ktob(uint64_t data){
    block* block1 = (block*) malloc(sizeof(block));
    block1->rpt = data & 0xfffffff;
    block1->lpt = (data >> 28);
    return block1;
}
uint64_t btok(block* b){
    uint64_t res = b->lpt;
    res = res << 28;
    res += b->rpt;
    return res;
}

uint32_t kshift(uint32_t k,int n){
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



void handle_wrong_arg(){
    printf("no args or wrong args.run des -h for help.\n");
    exit(0);
}

void handle_res(int res){
    if(res == 0) printf("succeed.\n");
    else printf("something went wrong.");
    exit(EXIT_SUCCESS);
}

int handle_gen_key(const char* filepath){
    FILE* file = fopen(filepath,"wb");
    if(file == 0) return -1;
    uint64_t key = gen_random_key();
    fwrite(&key,sizeof(uint64_t),1,file);
    fclose(file);
    return 0;
}

int handle_encryption(const char* inputfilepath,const char* keyfilepath,const char* outputfilepath){
    FILE* datafile = fopen(inputfilepath,"rb");
    if(datafile == 0) return -1;
    FILE* cipherfile = fopen(outputfilepath,"wb");
    if(cipherfile == 0) return -1;

    FILE* keyfile = fopen(keyfilepath,"rb");
    if(keyfile == 0) return -1;
    fseek(keyfile,0l,SEEK_END);
    if(ftell(keyfile) != 8){
        printf("(%ld)Invalid Key!!!\n",ftell(keyfile));
        return -1;
    }
    rewind(keyfile);
    uint64_t key;
    fread(&key,sizeof(uint64_t),1,keyfile);
    uint64_t* kn = genKn(key);
    uint64_t temp;
    while (!feof(datafile)){
        fread(&temp,sizeof(uint64_t),1,datafile);
        temp = encrypt(temp,kn);
        fwrite(&temp, sizeof(uint64_t),1,cipherfile);
    }
    fclose(datafile);
    fclose(cipherfile);
    fclose(keyfile);
    free(kn);
    return 0;
}

int handle_decryption(const char* inputfilepath,const char* keyfilepath,const char* outputfilepath){
    FILE* cipherfile = fopen(inputfilepath,"rb");
    if(cipherfile == 0) return -1;
    FILE* datafile = fopen(outputfilepath,"wb");
    if(datafile == 0) return -1;

    FILE* keyfile = fopen(keyfilepath,"rb");
    if(keyfile == 0) return -1;
    fseek(keyfile,0l,SEEK_END);
    if(ftell(keyfile) != 8){
        printf("(%ld)Invalid Key!!!\n",ftell(keyfile));
        return -1;
    }
    rewind(keyfile);
    uint64_t key;
    fread(&key,sizeof(uint64_t),1,keyfile);
    uint64_t* kn = genKn(key);
    reverse(kn);
    uint64_t temp;
    while (!feof(cipherfile)){
        fread(&temp,sizeof(uint64_t),1,cipherfile);
        temp = encrypt(temp,kn);
        fwrite(&temp, sizeof(uint64_t),1,datafile);
    }
    fclose(datafile);
    fclose(cipherfile);
    fclose(keyfile);
    free(kn);
    return 0;
}

uint64_t gen_random_key(){
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