//
// Created by amirhossein on 12/15/20.
//


#ifndef DES_DESHANDLER_H
#define DES_DESHANDLER_H
#include "des.h"
int des_genkey_handler(FILE* file);
int des_encrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile);
int des_decrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile);

#endif //DES_DESHANDLER_H
