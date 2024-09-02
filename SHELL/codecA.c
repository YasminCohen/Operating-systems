#include <ctype.h>
#include <stdio.h>
#include "codecA.h"
#include <stdbool.h>
void encodeOfCodecA(char* strr) {
    int i =0;
    bool Continue = true;
    bool lower;
    while(Continue){
        if(*(strr+i) == '\0'){
            Continue = false;
            break;
        }
        if (!isalpha(*(strr+i))) {
            i++;
            continue;
        }
        lower = islower(*(strr+i));
        if(lower){
            *(strr+i) = toupper(*(strr+i));    
        }
        else if(isupper(*(strr+i))) {
            *(strr+i) = tolower(*(strr+i));
        }
        i++;
    }
}

void decodeOfCodecA(char* strr) {
    int i =0;
    bool Continue = true;
    bool lower;
    while(Continue){
        if(*(strr+i) == '\0'){
            Continue = false;
            break;
        }
        if (!isalpha(*(strr+i))) {
            i++;
            continue;
        }
        lower = islower(*(strr+i));
        if(lower){
            *(strr+i) = toupper(*(strr+i));    
        }
        else if(isupper(*(strr+i))) {
            *(strr+i) = tolower(*(strr+i));
        }
        i++;
    }
}
