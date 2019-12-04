#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "LM.h"

#define STRBUF_SIZE 512

/*
typedef struct{
    int ngram_cnts[3];
    
}LM;


LM lm;

FILE *myfopen(char *file_path, char *mode){
    FILE *ret = fopen(file_path, mode);
    if(ret == NULL){
        printf("cannot open %s\n", file_path);
        exit(1);
    }
    return ret;
}

void getLM(char *lm_path, int order){
    FILE *lmfp = myfopen(lm_path, "r");
    char strbuf[STRBUF_SIZE];
    fscanf(lmfp, "%s", strbuf);
    for(int i=1;i<=order;++i){
        fscanf(lmfp, "%s", strbuf);  fscanf(lmfp, "%s", strbuf);
        if((strbuf[0]-'0' != i) || (strbuf[1] != '=')){
            printf("Broken language model\n");
            exit(1);
        }
        lm.ngram_cnts[i-1] = atoi(&strbuf[2]);  printf("%d\n", lm.ngram_cnts[i-1]);
    }
    
    for(int i=0;i<order;++i){
        for(int j=0;j<lm.ngram_cnts[i];++j){
            if(j > 6) exit(0);
            
            double 
            
            
            
            
            
            
            
        }
    }
    fclose(lmfp);
}
*/
int main(int argc, char **argv){
    if(argc != 5){
        printf("Format: %s [file_in] [mapping] [lm] [file_out]\n", argv[0]);
        exit(1);
    }
    //getLM(argv[3], 2);
    
    
    exit(0);
}