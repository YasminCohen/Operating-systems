#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    int flagV = 0;
    int flagF = 0;

    FILE *sroceFile;
    FILE *destFile;

    if (argc <= 2 || argc >5) {
        printf("All the arguments that should have been entered were not entered.Need to insert 2 files , The tool will support -v flag for verbose output. and The tool will support -f flag, that allows to overwrite the target file");
        return 1;
    }

    if(argc == 4){
        if(strcmp(argv[3], "-v") == 0){
            flagV = 1;
        }
        else if(strcmp(argv[3], "-f") == 0){
            flagF = 1;
        }else{
        printf("There are only two flags that can be used: The tool will support -v flag for verbose output. and The tool will support -f flag, that allows to overwrite the target file");
        return 1;
        }
    }
    else if(argc == 5){
        if(((strcmp(argv[3], "-v") == 0) && (strcmp(argv[4], "-f") == 0))||((strcmp(argv[3], "-f") == 0)&& (strcmp(argv[4], "-v") == 0))){
            flagF = 1;
            flagV = 1;  
        }else{
        printf("There are only two flags that can be used: The tool will support -v flag for verbose output. and The tool will support -f flag, that allows to overwrite the target file");
        return 1;
        }
    }

    if ((sroceFile = fopen(argv[1], "r")) == NULL){ 
        if(flagV == 1){
            printf("general failure\n");
        }    
        printf("the file not exit");
        return 1;
    }

    if((destFile = fopen(argv[2], "r")) != NULL && flagF == 0){
        if(flagV == 1){
            printf("target file exist\n ");   
        }
        fclose(sroceFile);
        fclose(destFile);
        return 1;
    }else{
        if((destFile = fopen(argv[2], "w")) == NULL){
            fclose(sroceFile);
            return 1;
        }
    }
    fseek(sroceFile, 0L, SEEK_END);
    float file_size = ftell(sroceFile);
    fseek(sroceFile, 0L, SEEK_SET);

    char buffer[1024];
    size_t Size;

    while ((Size = fread(buffer, 1, sizeof(buffer), sroceFile)) > 0) {
        fwrite(buffer, 1, Size, destFile);
    }
    fseek(destFile, 0L, SEEK_END);
    float sizeOfCopy = ftell(destFile);
    fseek(destFile, 0L, SEEK_SET);

    if (sizeOfCopy!= file_size) {
        if(flagV == 1){
            printf("general failure\n");
        }
        printf("Error in copying");
        fclose(sroceFile);
        fclose(destFile);
        return 1;
    }else{
        if( (flagV == 1)){
            printf("Success.\n");
        }
    }

    fclose(sroceFile);
    fclose(destFile);

    return 0;

}
