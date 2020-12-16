//
// Created by amirhossein on 12/16/20.
//

#include <stdio.h>
#include <stdint.h>
#include "../des/des.h"
#ifndef DES_TDESHANDLER_H
#define DES_TDESHANDLER_H

extern int tdes_genkey_handler(FILE* file);
extern int tdes_encrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile);
extern int tdes_decrypt_handler(FILE* inputfile,FILE* keyfile,FILE* outputfile);

#endif //DES_TDESHANDLER_H
