#include "file.h"
#include "string.h"
#include <stdlib.h>
#define DEBUG 0

#if DEBUG
    #define FILENAME "test.txt"
#else
    #define FILENAME "input.txt"
#endif

#define NOOP "noop"
#define ADDX "addx"

#define MAX_CRT_X 40
#define MAX_CRT_Y 6

int clock;
int rX;
int instruction_running;

void noop(int start_clock){
    if(clock-start_clock == 1)
        instruction_running = 0;
    else clock++;
}

void addx(int start_clock, char arg[]){
    if(clock-start_clock == 2){
        rX += atoi(arg);
        instruction_running = 0;
    }else clock++;
}

int main(){
    FILE *file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    int cycle_check = 20,
        increment   = 40;
    
    char line[MAX_BUFFER_SIZE],
         command[MAX_BUFFER_SIZE],
         number[MAX_BUFFER_SIZE];
    int status = 1;

    clock = 0;
    rX = 1;
    instruction_running = 0;

    int signal_strenght = 0;

    int crtX = 0,
        crtY = 0;

    char CRT[MAX_CRT_Y][MAX_CRT_X];
    for(int i=0; i<MAX_CRT_Y; i++)
        for(int j=0; j<MAX_CRT_X; j++)
            CRT[i][j] = ' ';

    while(status != EOF){
        status = fgetline(line, file, MAX_BUFFER_SIZE);

        split(line, command, number, ' ');

        int start_clock = clock;
        instruction_running = 1;

#if DEBUG
        printf("Enter instruction \"%s\" in cycle %d || rX: %d\n", line, clock, rX);
#endif
        while (instruction_running){
            if(strcmp(line, NOOP)==0){
                noop(start_clock);
            }else if(strcmp(command, ADDX)==0){
                addx(start_clock, number);
            }else {
                printf("ERROR: %s (%d)\n", line, strcmp(line, NOOP));
                return -1;
            }

            if(clock==cycle_check){
                signal_strenght += rX * clock;
                cycle_check += increment;
            }

            //Only prints a CRT pixel when a clock has moved.
            //In this lopp, if the above calls to noop and addx finish their run, the clock cycle
            //doesn't move, so we check if the instrucion is still runing (means that the clock still ticking) before printing
            if(instruction_running){
                if(crtX==MAX_CRT_X){ //if we finish the line, move to the next one
                    crtX=0;
                    crtY++;
                }
                if(crtY!=MAX_CRT_Y){ //if we reached the end of the 6th line, stop printing
                    if(crtX == rX || crtX == (rX-1) || crtX == (rX+1)){
                        CRT[crtY][crtX] = '#';
                        #if DEBUG
                        printf("\tsprite coincidence\n");
                        #endif
                    }else{
                        CRT[crtY][crtX] = '.';
                    }
                    crtX++;

                    #if DEBUG
                    printf("(clock %d) CRT_Line_%d: ",clock, crtY);
                    for(int i=0; i<MAX_CRT_X; i++)
                        printf("%c", CRT[crtY][i]);
                    printf("\n");
                    #endif
                }
            }
            

        }
        #if DEBUG
        printf("Exit in cycle %d || rX: %d\n\n", clock, rX);
        #endif
    }

    printf("The signal strenght is %d\n", signal_strenght);

    printf("\n");
    for(int i=0; i<MAX_CRT_Y; i++){
        for(int j=0; j<MAX_CRT_X; j++)
            printf("%c", CRT[i][j]);
        printf("\n");
    }


}