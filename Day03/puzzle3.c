#include <stdio.h>
#include <string.h>

#define FILENAME "input.txt"
#define MAX_BUFFER_SIZE 1024
#define ELFS_PER_GROUP 3

int nSum = 0;

struct Group{
    char    buffer1[MAX_BUFFER_SIZE],
            buffer2[MAX_BUFFER_SIZE],
            buffer3[MAX_BUFFER_SIZE];
    int length1,
        length2,
        length3;

    int b1Valid, b2Valid, b3Valid;
} group;

/**
 * Looks for a common character in the three lines 
 */
int parse(){
    for(int i=0; i<group.length1; i++){
        for(int j=0; j<group.length2; j++){

            if(group.buffer1[i]==group.buffer2[j]){

                for(int k=0; k<group.length3; k++){
                    if(group.buffer2[j]==group.buffer3[k]){

                        if(group.buffer1[i]>=65 && group.buffer1[i]<=90)
                            nSum += group.buffer1[i] - 'A' + 27;
                        else if(group.buffer1[i]>=97 && group.buffer1[i]<=122)
                            nSum += group.buffer1[i] - 'a' + 1;
                        return 1;
                    }
                }
            }
        }
    }
}

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

/**
 * If the three lines in the group are valid, processes the group and sets all lines to invalid
 */
void checkGroup(){
    if(group.b1Valid && group.b2Valid && group.b3Valid){
        parse();
        group.b1Valid = 0;
        group.b2Valid = 0;
        group.b3Valid = 0;
    }
}
 /**
  * Adds a line to the group
  */
void addLine(char line[], int nLineLength){
    if(!group.b1Valid){
        strcpy(group.buffer1, line);
        group.length1 =  nLineLength;
        group.b1Valid = 1;
    }
    else if(!group.b2Valid){
        strcpy(group.buffer2, line);
        group.length2 = nLineLength;
        group.b2Valid = 1;
    }else{
        strcpy(group.buffer3, line);
        group.length3 = nLineLength;
        group.b3Valid = 1;
    }
}

int main(){
    FILE* file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    char    line[MAX_BUFFER_SIZE];
    int     nLineLength, nNumLine = 0;

    group.b1Valid = 0;
    group.b2Valid = 0;
    group.b3Valid = 0;

    while(fgetline(line,  file, MAX_BUFFER_SIZE, &nLineLength) != EOF){
        if((nLineLength+1)%2!=0)
            fprintf(stderr, "Error: The line %d doesn't have a pair number of elements\n", nNumLine);
        

        checkGroup();
        addLine(line, nLineLength);

        nNumLine++;
    }

    //Process the last line/group
    addLine(line, nLineLength);
    checkGroup();
    
    printf("The sum of the priorities is %d\n", nSum);

    fclose(file);

    return 0;
}