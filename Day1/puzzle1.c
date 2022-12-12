#include <stdio.h>
#include <stdlib.h>

#define FILENAME "test.txt"
#define MAX_BUFFER_SIZE 1024

int main(){
    FILE* file;
    if(file = fopen(FILENAME, "r")==NULL){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    char line[MAX_BUFFER_SIZE];
    int c, //temporal character
        nBufferPointer = 0, //current index to write into the buffer
        nMaxElf = 0, nMaxCalories = 0, //The elf with the most calories 
        nCurrentElf = 1, nCurrentCalories = 0;

    while((c=fgetc(file)) != EOF){
        if(c=='\n'){
            line[nBufferPointer] = '\0';
            if(line[0]=='\0'){ //blank like
                if(nCurrentCalories>0){ //to bypass more than one blank like
                    if(nCurrentCalories>nMaxCalories){
                        nMaxCalories = nCurrentCalories;
                        nMaxElf = nCurrentElf;
                        nCurrentElf++;
                        nCurrentCalories = 0;
                    }
                }
                nBufferPointer=0;
            }  
        }else{
            line[nBufferPointer++] = c;
            if(nBufferPointer>=MAX_BUFFER_SIZE){
                fprintf(stderr, "Error: The buffer overflows, set the MAX_BUFFER_SIZE to a higher value and try again\n", FILENAME);
            }
        }
    }

    printf("The elf with the most calories is elf number %d with %d calories\n", nMaxElf, nMaxCalories);

    return 1;
}