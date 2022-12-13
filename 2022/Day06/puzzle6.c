#include <stdio.h>

#define FILENAME "input.txt"
#define MAX_BUFFER_SIZE 1024

#define LENGHT 14 //The number of consecutive different characters

/**
 * @brief Adds acharacter to s[] and deletes the oldest one
 */
void add(char s[], int length, char element){
    int i;
    for(i=length-1; i>0; i--){
        s[i] = s[i-1];
    }
    s[i] = element;
}

/**
 * @brief Returns 1 if all elements in s[] are different
 * Note:    If there's a '\0' or ' ' in any position (besides the end in the case of '\0')
 *          return 0, because it means the are not enought characters added to compare.
 */
int different(char s[], int length){
    int exclude;
    for(int i=0; i<length; i++){
        if(s[i]=='\0' || s[i]== ' ')
            return 0;
        exclude = i;
        for(int j=0; j<length; j++){
            if(j!=exclude && s[i]==s[j])
                return 0;
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

    int counter = 1;
    char aux[LENGHT+1] = "    \0";
    int c;

    while((c = fgetc(file))!=EOF){
        add(aux, LENGHT, (char) c);
        if(different(aux, LENGHT))
            break;
        counter++;
    }

    printf("First marker at %d\n", counter);
    fclose(file);
    return 1;
}