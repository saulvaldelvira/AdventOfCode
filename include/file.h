#pragma once
#ifndef file_h
#define file_h
#include <stdio.h>
#define MAX_BUFFER_SIZE 1024

#ifndef DEBUG
    #define DEBUG 1
#endif

int fgetline(char dest[], FILE* f,  int max){
    int c, nBuffer=0;
    for(int i=0; i<max; i++){
        c=fgetc(f);
        if(c==EOF){
            dest[nBuffer++] = '\0';
            return EOF;
        }  
        else if(c=='\n')
            break;
        else
            dest[nBuffer++]=c;
    }
    dest[nBuffer++] = '\0';
    return 1;
}

void split(char line[], char dest1[], char dest2[], char sep){
    int i;
    for(i=0; line[i]!=sep; i++){
        if(line[i] == '\0'){
            dest1[i] = '\0';
            dest2[0] = '\0';
            return;
        }
        dest1[i] = line[i];
    }
       
    dest1[i] = '\0';
    int j;
    for(j=0, i++; line[j+i]!='\0'; j++)
        dest2[j] = line[j+i];
    dest2[j] = '\0';
}

void skip_multiple(char line[], char dest[], const char seps[], int nSeps, char *trigger){
    int i;
    for(i=0; line[i] != '\0'; i++){
        int found = 0;
        for(int j=0; j<nSeps; j++){
            if(line[i]==seps[j]){
                *trigger = seps[j];
                found = 1;
            }
        }
        if(found)
            break;
    }

    int j;
    for(j=0, i++; line[i]!='\0'; j++, i++){
        dest[j] = line[i];
    }
    dest[j] = '\0';
}

int get_number(char line[], char dest[], int pos){

    int i = 0, count = 0;

   while (1){
        while(line[i] < '0' || line[i] > '9'){
            if(line[i]=='\0'){
                dest[0] = '\0';
                return -1; //skip till the next number
            }
            i++;
        }       
        
        if(count++==pos){ //if we reach the number we want, copy it to dest
            int j;
            for(j=0; line[i] >= '0' && line[i] <= '9'; i++, j++)
                dest[j] = line[i];
            dest[j] = '\0';
            return 1;
        }else{ //skip this number
            while(line[i++] >= '0' && line[i] <= '9');
            if (line[i-1] == '\0'){ //if we reach the end, and have not found our number, return -1
                dest[0] = '\0';
                return -1;
            }
        }
   }
   
    
}

void skip(char line[], char dest[], char sep){
    char seps[2];
    seps[0] = sep;
    seps[1] = '\0';
    char aux;
    skip_multiple(line, dest, seps, 1, &aux);
}

void no_whitespace(char line[], char dest[]){
    char aux[MAX_BUFFER_SIZE];
    int j=0;
    for(int i=0; line[i]!='\0'; i++){
        if(line[i]!=' ')
            aux[j++] = line[i];
    }
    aux[j] = '\0';

    for(int i=0; i<=j; i++)
        dest[i] = aux[i];
    
    aux[0] = '\0';
}

#endif