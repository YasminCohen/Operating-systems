#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]) {

    if(argc<=2){
        printf("All the arguments that should have been entered were not entered.Need to insert 2 files , The tool will support -v flag for verbose output. and The toll will support -i flag, that mean “ignore case” so “AAA” and “aaa” meaning equals\n");
        return 1;
    }
    if(argc>5){
        printf("Too many arguments!\n");
        return 1;
    }

    FILE* fp1 = fopen(argv[1],"r");
    FILE* fp2 = fopen(argv[2],"r");
   
    if(fp1 == NULL || fp2 == NULL){
        perror("There is a problem with one or more files\n");
        return 1;
    }

    int ResultInWords = 0;
    int ignore =0;
    
    if(argc == 4){
    
        if(strcmp(argv[3], "-v") == 0){
            ResultInWords = 1;
        }
        else if(strcmp(argv[3], "-i") == 0){
            ignore = 1;
        }
        else return 1;
    }
    
    if(argc == 5){

        if(strcmp(argv[3], "-v") == 0){
            ResultInWords = 1;
            if(strcmp(argv[4], "-i") == 0){
                ignore = 1;
            }
        }
        else if(strcmp(argv[3], "-i") == 0){
            ignore = 1;
            if(strcmp(argv[4], "-v") == 0){
                ResultInWords = 1;
            }
        }else return 1;    
    }
 
    char char1;
    char char2;
    int comper = 0;

    while (((char1 = fgetc(fp1)) != EOF) && ((char2 = fgetc(fp2))!= EOF)&& comper ==0)
    {	
        if(ignore == 1){
            char1 = tolower(char1);
            char2 = tolower(char2);
        }
        if(char1 != char2){ 
            if(ResultInWords == 1){
                printf("distinct\n");
            }
            comper = 1;
            break;
        }
    }
    fclose(fp1);
    fclose(fp2);

    if(comper == 1){
        
        return 1;

    }
    if(ResultInWords == 1)
    {
        printf("equal\n");
    }

    return 0;  
}
