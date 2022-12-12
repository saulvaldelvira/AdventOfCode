#include <stdio.h>

#define FILENAME "input.txt"

#define OP_ROCK     'A'
#define OP_PAPER    'B'
#define OP_SCISSORS 'C'

#define LOOSE   'X'
#define DRAW    'Y'
#define WIN     'Z'

char    cOp, cOutcome; //character for the oponent and the player's choices.
char    buffer[4];
int     nScore = 0;

void match(){
    cOp = buffer[0];
    cOutcome = buffer[2];

    nScore += (cOutcome - 'X') * 3;

    if(
        (cOp==OP_ROCK       && cOutcome == LOOSE)  ||
        (cOp==OP_SCISSORS   && cOutcome == DRAW)   ||
        (cOp==OP_PAPER      && cOutcome == WIN)
    )nScore += 3; //i have to use scissors

    else if(
        (cOp==OP_PAPER      && cOutcome == LOOSE)  ||
        (cOp==OP_ROCK       && cOutcome == DRAW)   ||
        (cOp==OP_SCISSORS   && cOutcome == WIN)
    ) nScore += 1; // i have to use rock
    else nScore += 2; // i have to use paper    
}

int main(){
    FILE* file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    int     nBufferIndex;
    int     c;

    while((c=fgetc(file)) != EOF){
        if(c=='\n'){
            match();
            nBufferIndex = 0;
        }else{
            buffer[nBufferIndex++] = c;
        }
    }
    match();

    fclose(file);

    printf("The player got %d total points\n", nScore);
}