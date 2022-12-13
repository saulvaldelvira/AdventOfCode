#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

#define FILENAME        "input.txt"
#define MAX_BUFFER_SIZE 1024
#define COLUMNS 9
#define ROWS 8

char    *tempMatrix,
        *matrix;

int heights[COLUMNS]; //array of heights for every column

void fillMatrix(char buffer[], int line){
    int col = 0, aux = 0;
    for(int i=0; buffer[i] != '\0'; i++){
        if(buffer[i]>=65 && buffer[i]<=90 )
            *(tempMatrix + COLUMNS * line + col) = buffer[i];

        aux ++;
        if(aux==4){
            col++;
            aux = 0;
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
 * Stores the next sequence of digits from buffer into result
 */
int nextNumber(char buffer[], char result[], int start){
    for(; buffer[start]<'0' || buffer[start]>'9'; start++);
    int i;
    for(i=0; buffer[start]>='0' && buffer[start]<='9' && buffer[start]!='\0'; start++, i++)
        result[i] = buffer[start];
    result[i] = '\0';
    return start;
    
}

/**
 * Gets the ammount of elements to move, the collumn of source and the 
 * collumn of destiny
 */
void processLine(char buffer[], int *amm, int *from, int *to){
    char aux[MAX_BUFFER_SIZE];
    int n=0;
    
    n = nextNumber(buffer, aux, n);
    *amm = atoi(aux);
    
    n = nextNumber(buffer, aux, n);
    *from = atoi(aux) - 1;

    n = nextNumber(buffer, aux, n);
    *to = atoi(aux) - 1;
}

/**
 * Get the height of the highest column
 */
int getHeight(int h[]){
    int height = 0;
    for(int i=0; i<COLUMNS; i++)
        if(h[i]>height)
            height = h[i];
    return height;
}

/**
 * @brief Prints the matrix
 */
void printMatrix(){
    for(int i=getHeight(heights)-1; i>=0; i--){
        for(int j=0; j<COLUMNS; j++)
            printf("[%c] ", *(matrix + COLUMNS * i + j));
        printf("\n");
    }
}

int main(){
    FILE* file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    tempMatrix  = malloc(COLUMNS * COLUMNS * sizeof(char));
    matrix      = malloc(MAX_BUFFER_SIZE * COLUMNS * sizeof(char));

    //Fill the matrices with spaces
    for(int i=0; i<COLUMNS*COLUMNS; i++)
        *(tempMatrix + i) = ' ';
    for(int i=0; i<MAX_BUFFER_SIZE; i++)
        *(matrix + i) = ' ';

    char    line[MAX_BUFFER_SIZE];
    int     nLineLength; //only used becaus the fgetline method requires it. Never used


    for(int i=0; i<COLUMNS; i++){
        fgetline(line, file, MAX_BUFFER_SIZE, &nLineLength);
        fillMatrix(line, i);
    }

    for(int i=ROWS-1, l=0; i>=0; i--, l++)
        for(int j=0; j<COLUMNS; j++)
            *(matrix + COLUMNS * l + j) = *(tempMatrix + COLUMNS * i + j);

    free(tempMatrix);

    printf("MARIX\n");
    for(int i=ROWS-1; i>=0; i--){
        for(int j=0; j<COLUMNS; j++)
            printf("[%c] ", *(matrix + COLUMNS * i + j));
        printf("\n");
    }

    //For some reason, if we are using the test file, we need 2 of these.
    //With the input file, only one
    fgetline(line, file, MAX_BUFFER_SIZE, &nLineLength); //skips the empty line
    //fgetline(line, file, MAX_BUFFER_SIZE, &nLineLength);
    
    int state = 1; 
    int amm=0, from=0, to=0;

    for(int i=0; i<COLUMNS; i++)
        heights[i] = 0;

    for(int i=0; i<ROWS; i++)
        for(int j=0; j<COLUMNS; j++)
            if(*(matrix + COLUMNS * i + j) != ' ')
                heights[j]++;

    #if DEBUG
    printf("Heihts\n");
    for(int i=0; i<COLUMNS; i++)
        printf("%d ", heights[i]);

    printf("\n");
    #endif

    while(state != EOF){
        state = fgetline(line, file, MAX_BUFFER_SIZE, &nLineLength);

        processLine(line, &amm, &from, &to);
        
        #if DEBUG
        printf("\nMove %d from %d to %d\n", amm, from+1, to+1);
        #endif

        for(int i=0; i<amm; i++){
            *(matrix + (heights[to]-1 + amm - i) * COLUMNS + to) = *(matrix + (heights[from]-1 - i) * COLUMNS + from);
            *(matrix + (heights[from]-1 - i) * COLUMNS + from) = ' ';
        }
        heights[to]+=amm;
        heights[from]-=amm;

        #if DEBUG
        printMatrix();
        for(int i=0; i<COLUMNS; i++)
            printf("%d ", heights[i]);  
        printf("\n");
        #endif
    }
    
    printf("\nResult \n");
    printMatrix();

    printf("The top elements are: ");
    for(int i=0; i<COLUMNS; i++)
        printf("%c", *(matrix + (heights[i]-1) * COLUMNS + i));
    printf("\n");
    
    fclose(file);
    free(matrix);
}