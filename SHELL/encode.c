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
    int cmp1 = strcmp(argv[1],"codecA");
    int cmp2 = strcmp(argv[1],"codecB");

    if(cmp1!=0 && cmp2!=0){
        return 1;
    }
    if(cmp1 == 0) {
        encodeOfCodecA(argv[2]);
    }
    else if(cmp2 == 0) {
        encodeOfCodecB(argv[2]);
    }
    printf("%s\n", argv[2]);

    return 0;  
}
