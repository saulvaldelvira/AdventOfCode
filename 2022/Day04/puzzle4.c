#include <stdio.h>
#include <stdlib.h>

#define FILENAME "input.txt"
#define MAX_BUFFER_SIZE 1024

typedef struct {
    int a, b;
} Range;

int fgetline(char buffer[], FILE* f,  int max, int *length){
    int c, nBuffer=0;
    for(int i=0; i<max; i++){
        c=fgetc(f);
        if(c==EOF){
            buffer[nBuffer++] = '\0';
            *length = nBuffer;
            return EOF;
        }  
        else if(c=='\n')
            break;
        else
            buffer[nBuffer++]=c;
    }
    buffer[nBuffer++] = '\0';
    *length = nBuffer;
    return 1;
}

void getRange(Range *r, char line[]){
    char aux[MAX_BUFFER_SIZE];
    int i, j;
    for(i=0; line[i]!= '-'; i++)
        aux[i] = line[i];
    aux[i] = '\0';
    (*r).a = atoi(aux);


    for(j=0, i++; line[i] != '\0'; j++, i++)
        aux[j] = line[i];
    aux[j] = '\0';
    (*r).b = atoi(aux);

}

int main(){
    FILE* file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }
 
    char    line[MAX_BUFFER_SIZE],
            aux[MAX_BUFFER_SIZE];
    int nLineLength, nBufferlength;
    Range r1, r2;
    int nSum = 0;

    int state = 1; //when the file ends, goes to -1 (EOF)
    while(state != EOF){
        state = fgetline(line, file, MAX_BUFFER_SIZE, &nLineLength);

        
        for(nBufferlength = 0; line[nBufferlength] != ','; nBufferlength++)
            aux[nBufferlength] = line[nBufferlength];
        aux[nBufferlength] = '\0';

        getRange(&r1, aux);

        int i;
        for(i=0, nBufferlength++ ; line[nBufferlength] != '\0'; nBufferlength++, i++)
            aux[i] = line[nBufferlength];
        aux[i] = '\0';

        getRange(&r2, aux);


        if(
            (r1.a<=r2.a && r1.b>=r2.b) ||
            (r1.a>=r2.a && r1.b<=r2.b) ||
            (r2.a>=r1.a && r2.a<=r1.b) ||
            (r2.b>=r1.a && r2.b<=r1.b)
        ) nSum++;

    }

    printf("The number of pairs that overlap is %d\n", nSum);

    fclose(file);
    return 1;
}

