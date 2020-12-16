//
// Created by amirhossein on 12/15/20.
//

#include <stdio.h>
#include <stdint.h>
#include "deshandler.h"


int des_genkey_handler(FILE* file){
    uint64_t key = des_gen_random_key();
    int res = 0;
    if(fwrite(&key, sizeof(uint64_t),1,file) != 1) res = 1;
    fclose(file);
    return res;
}
int des_encrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile){
    fseek(keyfile,0l,SEEK_END);
    if(ftell(keyfile) != 8){
        fprintf(stderr,"Invalid Key file.key length must be 8 byte.\n");
        exit(EXIT_FAILURE);
    }
    rewind(keyfile);
    const uint64_t key_data;
    uint64_t temp;
    fread(&key_data, sizeof(uint64_t),1,keyfile);
    uint64_t* kn = des_genKn(key_data);
    while (!feof(inputfile)){
        fread(&temp,sizeof(uint64_t),1,inputfile);
        temp = des_encrypt(temp,kn);
        fwrite(&temp, sizeof(uint64_t),1,outputfile);
    }
    fclose(inputfile);fclose(keyfile);fclose(outputfile);
    return 0;
}
int des_decrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile){
    fseek(keyfile,0l,SEEK_END);
    if(ftell(keyfile) != 8){
        fprintf(stderr,"Invalid Key file.key length must be 8 byte.\n");
        exit(EXIT_FAILURE);
    }
    rewind(keyfile);
    const uint64_t key_data;
    uint64_t temp;
    fread(&key_data, sizeof(uint64_t),1,keyfile);
    uint64_t* kn = des_genKn(key_data);
    des_reverse(kn);
    while (!feof(inputfile)){
        fread(&temp,sizeof(uint64_t),1,inputfile);
        temp = des_encrypt(temp,kn);
        fwrite(&temp, sizeof(uint64_t),1,outputfile);
    }
    fclose(inputfile);fclose(keyfile);fclose(outputfile);
    return 0;
}
