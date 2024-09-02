#include <stdbool.h>
#include <ctype.h>
#include "codecB.h"

void encodeOfCodecB(char* str) {
    bool Continue = true;
    int i =0;
    while(Continue){
        if(*(str+i) == '\0'){
            Continue = false;
            break;
        }
        if(isalpha(str[i])){
            *(str+i) += 3;
        }
        i++;
    }
}

void decodeOfCodecB(char* str) {
    bool Continue = true;
    int i =0;
    while(Continue){
        if(*(str+i) == '\0'){
            Continue = false;
            break;
        }
        if(isalpha(str[i])){
            *(str+i) -= 3;
        }
        i++;
    }
}
