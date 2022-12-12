#include <stdio.h>
#include <stdlib.h>

#define FILENAME "test.txt"
#define MAX_BUFFER_SIZE 1024
#define NUMBER_TOP_RANKING 3 //number of elfs to show a ranking of


struct topElfs{
    int elfs[NUMBER_TOP_RANKING],
        calories[NUMBER_TOP_RANKING];
} top;

/**
 * @brief If the given calories are bigger than the smallest in the top (the one at the end of the array of calories),
 *        replaces the calories and the elf number and starts to filter up to the top to keep the array sorted.
 * 
 * @param cal the calorie value to check
 * @param elf the elf number
 */
void measureCalories(int cal, int elf){
    if(cal>top.calories[NUMBER_TOP_RANKING-1]){
        top.calories[NUMBER_TOP_RANKING-1] = cal;
        top.elfs[NUMBER_TOP_RANKING-1] = elf;
        for(int i=NUMBER_TOP_RANKING-1; i>=1; i--)
            if(top.calories[i]>top.calories[i-1]){
                int aux;
                aux = top.calories[i-1];
                top.calories[i-1] = top.calories[i];
                top.calories[i] = aux;

                aux = top.elfs[i-1];
                top.elfs[i-1] = top.elfs[i];
                top.elfs[i] = aux;
            }
        }
}

int main(){
    FILE* file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    char line[MAX_BUFFER_SIZE];
    int c, //temporal character
        nBufferPointer = 0, //current index to write into the buffer
        nCurrentElf = 1, nCurrentCalories = 0;

    while((c=fgetc(file)) != EOF){
        if(c=='\n'){ //if we reach a newline, parse it
            line[nBufferPointer] = '\0';
            if(line[0]=='\0'){ //blank like
                if(nCurrentCalories>0){ //to bypass if there's more than one consecutive blank lines
                    measureCalories(nCurrentCalories, nCurrentElf++); //increases nCurrentElf
                    nCurrentCalories=0;
                }
            }else //else, add the calories
             nCurrentCalories += atoi(line);

           nBufferPointer=0;
        }else{//just add the character to the buffer for the line
            line[nBufferPointer++] = c;
            if(nBufferPointer>=MAX_BUFFER_SIZE){
                fprintf(stderr, "Error: The buffer overflows, set the MAX_BUFFER_SIZE to a higher value and try again\n");
            }
        }
    }

    //parse the last line, since the while loop breaks after finding EOF, leaving it unattended.
    measureCalories(nCurrentCalories, nCurrentElf);

    printf("\nTop %d elfs\n", NUMBER_TOP_RANKING);

    nCurrentCalories = 0; //just recycling this variable for the addition;
    for(int i=0; i<NUMBER_TOP_RANKING; i++){
        nCurrentCalories+=top.calories[i];
        printf("Elf number %d: %d calories\n", top.elfs[i], top.calories[i]);
    }
    
    printf("\nThe top %d elfs carry a total of %d calories\n", NUMBER_TOP_RANKING, nCurrentCalories);

    return 1;
}
