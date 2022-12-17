#include <stdio.h>
#include <stdlib.h>

#define FILENAME "test.txt"
#define MATRIX_SIZE 100

int is_visible(int x, int y, int matrix[MATRIX_SIZE][MATRIX_SIZE], int row, int col){
    int result = 0b1111;

    //checks to the left
    for(int i=y-1; i>=0; i--)
        if(matrix[x][i] >= matrix[x][y]){
            result &= 0b1101;
            break;
        }
    //to the right
    for(int i=y+1; i<col; i++)
        if(matrix[x][i] >= matrix[x][y]){
            result &= 0b1110;
            break;
        }
            
    //up
    for(int j=x-1; j>=0; j--)
        if(matrix[j][y] >= matrix[x][y]){
            result &= 0b0111;
            break;
        }

    //down
    for(int j=x+1; j<row; j++)
        if(matrix[j][y] >= matrix[x][y]){
            result &= 0b1011;
            break;
        }
    
    return result;
}

/**
 * @brief   The scenic score is the multiplication of the number of "trees" that are < to the 
 *          one in index [i,j] in the four directions.
 *          NOTE:   Sorry if this is ugly or not very clever code. I'm kinda tired today lol :p.
 *                  Still works tho.
 */
int scenic_score(int x, int y, int matrix[MATRIX_SIZE][MATRIX_SIZE], int row, int col){
    int result = 1,
        counter = 0;
    for(int i=y-1; i>=0; i--){
        counter++;
        if(matrix[x][i]>=matrix[x][y])
            break;
    }

    result *= counter;
    counter=0;
    
    for(int i=y+1; i<col; i++){
        counter++;
        if(matrix[x][i]>=matrix[x][y])
            break;
    }
    result *= counter;
    counter=0;

    for(int j=x-1; j>=0; j--){
        counter++;
        if(matrix[j][y]>=matrix[x][y])
            break;
    }  

    result *= counter;
    counter=0;

    for(int j=x+1; j<row; j++){
        counter++;
        if(matrix[j][y]>=matrix[x][y])
            break;
    }  
        
    result *= counter;

    return result;
}

int main(){
//INITIALIZING

    FILE *file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    int c, 
        nLine   = 0,
        row, col;

    int matrix[MATRIX_SIZE][MATRIX_SIZE];

    int sum, nscenicScore;

//LOOP
    while((c=fgetc(file)) != EOF){
        if(c=='\n'){
            row++;
            col=0;
        }else{
            matrix[row][col++] = c - '0';
        }
    }
    row++;

//Calculate results

    sum = 2*(row-1) + 2*(col-1);

    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++)
            printf("%d", matrix[i][j]);
        printf("\n");
    }

    printf("X: %d, Y:%d\n", row, col);

    for(int i=1; i<row-1; i++)
        for(int j=1; j<col-1; j++)
            if(is_visible(i,j,matrix, row, col))
                sum++;

    printf("The number of visible trees are %d\n", sum);

    nscenicScore = 0;

    for(int i=1; i<row-1; i++){
        for(int j=1; j<col-1; j++){
            int tmp  = scenic_score(i, j, matrix, row, col);
            if(tmp>nscenicScore)
                nscenicScore = tmp;
        }
    }
            
    printf("The highest scenic score is %d\n", nscenicScore);

    fclose(file);
    return 0;
}