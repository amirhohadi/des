/*
 * @Author(Amir Hossein Ohadi)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "gopt/gopt.h"
#include "des/deshandler.h"
#include "triple_des/tdeshandler.h"

FILE *open_file(const char* filepath,const char* mode);
void hanlde_gen_key(int(*f)(FILE*),const char* filepath);
void handle_encryption(int(*f)(FILE*,FILE*,FILE*),const char * inputfile,const char* keypath,const char* outputpath);

static int verbose = 0;

int main(int argc,char** argv) {

    const char *keyfile,*inputfile,*outputfile,*mode;

    void* options = gopt_sort(&argc,argv,gopt_start(
            gopt_option('h',GOPT_ONCE,gopt_shorts('h','?'),gopt_longs("help")),
            gopt_option('v',GOPT_NOARG,gopt_shorts('v'),gopt_longs("verbose")),
            gopt_option('g',GOPT_ARG,gopt_shorts('g'),gopt_longs("gen-key")),
            gopt_option('k',GOPT_ARG,gopt_shorts('k'),gopt_longs("key")),
            gopt_option('e',GOPT_ARG,gopt_shorts('e'),gopt_longs("encrypt")),
            gopt_option('d',GOPT_ARG,gopt_shorts('d'),gopt_longs("decrypt")),
            gopt_option('f',GOPT_ARG,gopt_shorts('f'),gopt_longs("file")),
            gopt_option('o',GOPT_ARG,gopt_shorts('o'),gopt_longs("output"))
            ));

    if(gopt(options,'h')){
        fprintf(stdout,"help\n");
        exit(EXIT_SUCCESS);
    }
    if(gopt(options,'v')) verbose = 1;

    if(gopt(options,'f')) inputfile = gopt_arg_i(options,'f',0);

    if(gopt(options,'k')) keyfile = gopt_arg_i(options,'k',0);
    else keyfile = "mykey.key";

    if(gopt(options,'o')) outputfile = gopt_arg_i(options,'o',0);
    else outputfile = "temp.output";

    if(gopt(options,'g')){
        mode = gopt_arg_i(options,'g',0);
        if(strcmp(mode,"des") == 0){
            hanlde_gen_key(des_genkey_handler,keyfile);
        }else if(strcmp(mode,"3des") == 0){
            hanlde_gen_key(tdes_genkey_handler,keyfile);
        }else{
            fprintf(stderr,"unknown mode.\n");
            exit(EXIT_FAILURE);
        }
    }

    if(gopt(options,'e')){
        mode = gopt_arg_i(options,'e',0);
        if(strcmp(mode,"des") == 0){
            handle_encryption(des_encrypt_handler,inputfile,keyfile,outputfile);
        }else if(strcmp(mode,"3des") == 0){
            handle_encryption(tdes_encrypt_handler,inputfile,keyfile,outputfile);
        }else{
            fprintf(stderr,"unknown mode.\n");
            exit(EXIT_FAILURE);
        }
    }

    if(gopt(options,'d')){
        mode = gopt_arg_i(options,'d',0);
        if(strcmp(mode,"des") == 0){
            handle_encryption(des_decrypt_handler,inputfile,keyfile,outputfile);
        }else if(strcmp(mode,"3des") == 0){
            handle_encryption(tdes_decrypt_handler,inputfile,keyfile,outputfile);
        }else{
            fprintf(stderr,"unknown mode.\n");
            exit(EXIT_FAILURE);
        }
    }


    return 0;
}

FILE *open_file(const char* filepath,const char* mode){
    FILE* file = fopen(filepath,mode);
    if(file == NULL){
        if(errno == ENOENT) fprintf(stderr,"%s:file does not exists\n",filepath);
        else if(errno == EACCES)
            fprintf(stderr,"%s:cannot access this file or directory.permission denied\n",filepath);
        else fprintf(stderr,"something happend!\n");
        exit(EXIT_FAILURE);
    }
    return file;
}


void hanlde_gen_key(int(*f)(FILE*),const char* filepath){
    FILE* keyfile = open_file(filepath,"wb");
    if(f(keyfile) == 0){
        fprintf(stdout,"succeed\n");
        exit(EXIT_SUCCESS);
    }else{
        fprintf(stderr,"something happend!\n");
        exit(EXIT_FAILURE);
    }
}

void handle_encryption(int(*f)(FILE*,FILE*,FILE*),const char * inputfile,const char* keypath,const char* outputpath){
    FILE* input = open_file(inputfile,"rb");
    FILE* key = open_file(keypath,"rb");
    FILE* output = open_file(outputpath,"wb");
    if(f(input,key,output) == 0){
        fprintf(stdout,"succeed\n");
        exit(EXIT_SUCCESS);
    }else{
        fprintf(stderr,"something happend!\n");
        exit(EXIT_FAILURE);
    }
}