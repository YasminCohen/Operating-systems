#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char* argv[]) {
    if(argc <=2) {
        printf("More parameters need to be entered\n");
        return 1;
    }
    if(strcmp(argv[1], "codecA")!=0 && strcmp(argv[1], "codecB")!=0){
        return 1;
    }
    
    int cmp1 = strcmp(argv[1], "codecA");
    int cmp2 = strcmp(argv[1],"codecB");

    if(cmp1==0) {
        decodeOfCodecA(argv[2]);
    }

    if(cmp2==0) {
        decodeOfCodecB(argv[2]);
    }
    printf("%s\n", argv[2]);

    return 0;  
}
