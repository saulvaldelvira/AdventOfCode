#include <stdio.h>
#include <stdlib.h>

#define FILENAME "test.txt"
#define MAX_BUFFER_SIZE 1024

int nSum = 0;

int parse(char buffer[], int length){
    for(int i=0; i<length/2-1; i++){
        for(int j=length/2; j<length; j++)
            if(buffer[i]==buffer[j]){
                if(buffer[i]>=65 && buffer[i]<=90)
                    nSum += buffer[i] - 'A' + 27;
                else if(buffer[i]>=97 && buffer[i]<=122)
                    nSum += buffer[i] - 'a' + 1;
                else
                    return -1;
            }
    }
    return 1;
}

int main(){
    FILE* file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    char line[MAX_BUFFER_SIZE];
    int nLineLength, nNumLine = 0;

    while((nLineLength = getline(&line, MAX_BUFFER_SIZE, file)) != EOF){
        if(nLineLength%2!=0)
            fprintf(stderr, "Error: The line %d doesn't have a pair number of elements\n", nNumLine);
        
            if(!parse(line, nLineLength))
                fprintf(stderr, "Error: Found a character that is NOT a letter in line %d\n", nNumLine);


        nNumLine++;
    }

    

    fclose(file);
}