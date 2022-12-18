#pragma once
#ifndef file_h
#define file_h
#include <stdio.h>
#define MAX_BUFFER_SIZE 1024

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

#endif