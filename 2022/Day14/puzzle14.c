#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
        #include <windows.h>
#elif __linux__
        #include <unistd.h>
#endif

#ifdef DEBUG
	#define FILENAME "test.txt"
#else
        #define FILENAME "input.txt"
#endif
    
#define BUFFER_SIZE 1000


typedef struct Coords{
	uint64_t x, y;
} Coords;

/**
* Counts the longest line in the file
*/
int count_input(FILE *file){
	int n = 0;
        int cols = 0;
	int c;
	while((c = getc(file)) != EOF){
                if(c == '\n'){
                        if(n > cols){
                                cols  = n;
                        }
                        n = 0;
                }
                n++;
        }
        if(n > cols)
                cols  = n;
        rewind(file);
        return cols;
}

/**
* Counts the number of pairs of coordinates are in the line
*/
int count_line(char line[]){
        int n = 0;
        for(int i = 0; line[i] != '\0'; i++)
                if(line[i] == '-')
                        n++;
        return n + 1; // Add one because n is the number of separators
}

/**
* Gets a line from the file
*/
int get_line(char dest[], FILE *source, int max){
        int c;
        int n = 0;
        while(n < max){
                c = getc(source);
                if(c == EOF){
                        dest[n] = '\0';
                        return EOF;
                }else if(c == '\n'){
                        dest[n] = '\0';
                        return 1;
                } else{
                        dest[n++] = (char) c;
                }
        }
        return 0;
}

/**
 * Draws a line between the given coordinates
*/
void draw_line(char waterfall[BUFFER_SIZE][BUFFER_SIZE], int x_1, int y_1, int x_2, int y_2){
#ifdef DEBUG
        printf("Drawing from [%d,%d] to [%d,%d]\n", x_1, y_1, x_2, y_2);
#endif
        if(x_1 == x_2){
                if(y_1 < y_2){
                        for(int i = y_1; i <= y_2; i++){
                                waterfall[i][x_1] = '#';
                        }
                }else if(y_1 > y_2){
                        for(int i = y_2; i <= y_1; i++){
                                waterfall[i][x_1] = '#';
                        }
                }
        }else if(y_1 == y_2){
                if(x_1 < x_2){
                        for(int i = x_1; i <= x_2; i++){
                                waterfall[y_1][i] = '#';
                        }
                }else if(x_1 > x_2){
                        for(int i = x_2; i <= x_1; i++){
                                waterfall[y_1][i] = '#';
                        }
                }
        }else{
                fprintf(stderr, "Malformated Input: Nothing to draw from [%d,%d] to [%d,%d]\n", x_1, y_1, x_2, y_2);
        }
}

/**
* Prints the animation in the console
*/
void print_waterfall(char waterfall[BUFFER_SIZE][BUFFER_SIZE],int min_y, int y,int min_x, int x, int part){
#ifdef ANIMATION
        #ifdef _WIN32
                system("cls");
        #elif __linux__
                system("clear");
        #endif
        printf("PART %d\n", part);
#endif
        for(int i = min_y; i < y; i++){
                for(int j = min_x; j < x; j++){
                        printf("%c", waterfall[i][j]);
                }
                printf("\n");
        }
#ifdef ANIMATION
        #ifdef _WIN32
                Sleep(30);
        #elif __linux__
                usleep(30000);
        #endif
#endif
}

int main(){
        FILE *file = fopen(FILENAME, "rt");
        int line_size = count_input(file);
#if DEBUG
        printf("Longest line: %d\n", line_size);
#endif
        char line[line_size];
        char waterfall[BUFFER_SIZE][BUFFER_SIZE];

        for(int i = 0; i < BUFFER_SIZE; i++)
                for(int j = 0; j < BUFFER_SIZE; j++)
                        waterfall[i][j] = '.';

        int max_x = 0;          // The max x coordinate reached
        int max_y = 0;          // The max y coordinate reached
        int min_x = 9999;       // The min x coordinate reached

        // Iterate the whole file to find the lowest value of x
        int status = 1;
        while(status != EOF){
                status = get_line(line, file, line_size);
                if(line[0] == '\n' || status == EOF)
                        continue;

                int nLines = count_line(line);
                char *aux = line;
                for(int i = 0; i < nLines; i++){
                        while(!isdigit(*aux))
                                aux++;
                        int x = atoi(aux);
                        while(isdigit(*aux))
                                aux++;

                        while(!isdigit(*aux))
                                aux++;
                        //Skip the y 
                        while(isdigit(*aux))
                                aux++;
                        
                        if(x < min_x)
                                min_x = x;
                }
        }

        rewind(file);

        status = 1;
        while(status != EOF){
                status = get_line(line, file, line_size);
                if(line[0] == '\n' || status == EOF)
                        continue;

                // Parse Line
                int nLines = count_line(line);
#if DEBUG
                printf("There are %d coords in this line\n", nLines);
#endif          
                // Parse the coordinates from the line
                Coords cords[nLines];
                char *aux  = line;
                for(int i = 0; i < nLines; i++){
                        while(!isdigit(*aux))
                                aux++;
                        int x = atoi(aux);
                        while(isdigit(*aux))
                                aux++;

                        while(!isdigit(*aux))
                                aux++;
                        int y = atoi(aux);
                        while(isdigit(*aux))
                                aux++;

                        cords[i].x = x;
                        cords[i].y = y;

                        if(x+1 > max_x)
                                max_x = x+1;
                        if(y+1 > max_y)
                                max_y = y+1;

#if DEBUG
                        printf("Found [%d,%d]\n", x, y);
#endif
                }

                // Draw those coordinates       
                for(int i = 0; i < nLines-1; i++)
                        draw_line(waterfall, cords[i].x, cords[i].y, cords[i+1].x, cords[i+1].y);
        }
       

        int sand_start = 500; // Starting x point of the sand
        int sand_x = sand_start;
        int sand_y = 0;
        bool finish = false; // True if the simulation has ended
        int placed_grains_p1 = 0;

        int animation_upper_y  = 0;
        int animation_lower_y  = 50;
        int animation_increment  = 50;

        /********PART 1**************
         * Count placed sand grains till one falls behind the lowest rock (aka. the infinite void)
         * ***************************/ 
        while(!finish){
                if(sand_y == animation_lower_y){
                        animation_upper_y = animation_lower_y-5;
                        animation_lower_y += animation_increment-5;
                }
#ifdef ANIMATION
                waterfall[sand_y][sand_x] = '+';
                print_waterfall(waterfall, animation_upper_y , animation_lower_y, min_x-10 , max_x+10, 1);
#endif
               waterfall[sand_y][sand_x] = '.'; 
                if(waterfall[sand_y+1][sand_x] == '.'){
                        sand_y++;
                }else if(waterfall[sand_y+1][sand_x-1] == '.'){
                        sand_y++;
                        sand_x--;
                }else if(waterfall[sand_y+1][sand_x+1] == '.'){
                        sand_y++;
                        sand_x++;
                }else{
                        waterfall[sand_y][sand_x] = 'o';
                        sand_y = 0;
                        sand_x = sand_start;
                        placed_grains_p1++;
                }

                if(sand_y >= max_y)
                        finish = true;
        }
        
        /********PART 2**************
         * Now there's a floor at the bottom, so count grains till one is placed in the generating point [x=500, y=0]
         * ***************************/ 

        for(int i = 0; i < BUFFER_SIZE; i++)
                for(int j = 0; j < BUFFER_SIZE; j++)
                        if(waterfall[i][j] == 'o' || waterfall[i][j] == '+')
                                waterfall[i][j] = '.';

        // Put the infinite barrier at the bottom
        int barrier_y = max_y + 1;
        for(int i=0; i < BUFFER_SIZE; i++)
                waterfall[barrier_y][i] = '#';

        max_y = barrier_y+1;
        max_x += 10;

        // Reset the variables for another simulation
        sand_start = 500;
        sand_x = sand_start;
        sand_y = 0;
        finish = false;
        int placed_grains_p2 = 0;

        animation_upper_y  = 0;
        animation_lower_y  = 50;
        animation_increment  = 50;


        while(!finish){
                if(sand_y == animation_lower_y){
                        animation_upper_y = animation_lower_y-5;
                        animation_lower_y += animation_increment-5;
                }
#ifdef ANIMATION
                waterfall[sand_y][sand_x] = '+';
                print_waterfall(waterfall, animation_upper_y , animation_lower_y, min_x-10 , max_x+10, 1);
#endif
               waterfall[sand_y][sand_x] = '.';
                if(waterfall[sand_y+1][sand_x] == '.'){
                        sand_y++;
                }else if(waterfall[sand_y+1][sand_x-1] == '.'){
                        sand_y++;
                        sand_x--;
                }else if(waterfall[sand_y+1][sand_x+1] == '.'){
                        sand_y++;
                        sand_x++;
                }else{
                        waterfall[sand_y][sand_x] = 'o';
                        placed_grains_p2++;
                        if(sand_y == 0)
                                finish = true;
                        sand_y = 0;
                        sand_x = sand_start;
                }

                
        }

        printf("[Part 1] Total placed grains: %d\n", placed_grains_p1);
        printf("[Part 2] Total placed grains: %d\n", placed_grains_p2);

        return 0;
}
