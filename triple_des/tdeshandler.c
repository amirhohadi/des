//
// Created by amirhossein on 12/16/20.
//

#include "tdeshandler.h"

int tdes_genkey_handler(FILE* file){
    uint64_t key[3];
    for(int i = 0 ; i < 3 ; i++){
        key[i] = des_gen_random_key();
    }
    int res = 0;
    if(fwrite(&key, sizeof(uint64_t),3,file) != 3) res = 1;
    fclose(file);
    return res;
}
int tdes_encrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile){
    fseek(keyfile,0l,SEEK_END);
    if(ftell(keyfile) != 24){
        fprintf(stderr,"Invalid Key file.key length must be 24 byte.\n");
        exit(EXIT_FAILURE);
    }
    rewind(keyfile);
    uint64_t keys[3];
    fread(&keys, sizeof(uint64_t),3,keyfile);
    uint64_t *kn[3];
    kn[0] = des_genKn(keys[0]);
    kn[1] = des_genKn(keys[1]);
    des_reverse(kn[1]);
    kn[2] = des_genKn(keys[2]);

    uint64_t temp;
    while (!feof(inputfile)){
        fread(&temp,sizeof(uint64_t),1,inputfile);
        temp = des_encrypt(temp,kn[2]);
        temp = des_encrypt(temp,kn[1]);
        temp = des_encrypt(temp,kn[0]);
        fwrite(&temp, sizeof(uint64_t),1,outputfile);
    }
    fclose(inputfile);fclose(keyfile);fclose(outputfile);
    return 0;

}
int tdes_decrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile){
    fseek(keyfile,0l,SEEK_END);
    if(ftell(keyfile) != 24){
        fprintf(stderr,"Invalid Key file.key length must be 24 byte.\n");
        exit(EXIT_FAILURE);
    }
    rewind(keyfile);
    uint64_t keys[3];
    fread(&keys, sizeof(uint64_t),3,keyfile);
    uint64_t *kn[3];
    kn[0] = des_genKn(keys[0]);
    des_reverse(kn[0]);
    kn[1] = des_genKn(keys[1]);
    kn[2] = des_genKn(keys[2]);
    des_reverse(kn[2]);

    uint64_t temp;
    while (!feof(inputfile)){
        fread(&temp,sizeof(uint64_t),1,inputfile);
        temp = des_encrypt(temp,kn[0]);
        temp = des_encrypt(temp,kn[1]);
        temp = des_encrypt(temp,kn[2]);
        fwrite(&temp, sizeof(uint64_t),1,outputfile);
    }
    fclose(inputfile);fclose(keyfile);fclose(outputfile);
    return 0;
}