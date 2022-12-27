#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#ifdef DEBUG
        #define FILENAME "test.txt"
#else
        #define FILENAME "input.txt"
#endif

struct Coordinates{
        int x, y;
};

typedef struct Coordinates Sensor;
typedef struct Coordinates Beacon;

typedef struct Pair{
        Sensor s;
        Beacon b;
} Pair;

/**
* Gets a line from the file
*/
int get_line(char dest[], FILE *source, int max){
        int c;
        int n = 0;
        while (n < max){
                c = getc(source);
                if (c == EOF){
                        dest[n] = '\0';
                        return EOF;
                }else if (c == '\n'){
                        dest[n] = '\0';
                        return 1;
                }else {
                        dest[n++] = (char) c;
                }
        }
        return 0;
}

/**
* Calculates Manhatan distance between the two given Coordinates
*/
int distance(struct Coordinates p_1, struct Coordinates p_2){
        int x = abs(p_1.x - p_2.x);
        int y = abs(p_1.y - p_2.y);
        return x + y;
}

int main(){
        FILE *file = fopen(FILENAME, "rt");
        if (!file){
                fprintf(stderr, "Error: could not open the file\n");
                return -1;
        }
        
        // Count number of lines (aka. number of pairs of Sensor<->Beacon)
        char line[512];
        int next = 1;
        int n_sensors = 0;
        while (next != EOF){
                next = get_line(line, file, 512);
                if (line[0] != '\n' && line[0] != '\0'){
                        n_sensors++;
                }
        }

        rewind(file);

        Pair array[n_sensors];

        int min_x = INT_MAX;
        int max_x = INT_MIN;
        
        // Parse every Sensor
        for (int i = 0; i < n_sensors; i++){
                get_line(line, file, 512);
                char *p = line;
                // Skip to the first number
                while (!isdigit(*p) && (*p) != '-'){
                       p++;
                }
                array[i].s.x = atoi(p);

                // Skip the current number
                while (isdigit(*p) || (*p) == '-'){
                       p++;
                }
                // And jump to the next one
                while (!isdigit(*p) && (*p) != '-'){
                       p++;
                }
                array[i].s.y = atoi(p);

                while (isdigit(*p) || (*p) == '-'){
                       p++;
                }
                while (!isdigit(*p) && (*p) != '-'){
                       p++;
                }

                array[i].b.x = atoi(p);

                while (isdigit(*p) || (*p) == '-'){
                       p++;
                }
                while (!isdigit(*p) && (*p) != '-'){
                       p++;
                }

                array[i].b.y = atoi(p);

#if DEBUG
                printf("Sensor [%-2d,%-2d] --> Beacon[%-2d, %-2d]\n", array[i].s.x, array[i].s.y, array[i].b.x, array[i].b.y);
#endif
                // Check if this Sensor's range is lower than the min and/or greater than the max
                int min = array[i].s.x - distance(array[i].s, array[i].b);
                int max = array[i].s.x + distance(array[i].s, array[i].b);

                if (min < min_x){
                        min_x = min;
                }
                if (max > max_x){
                        max_x = max;
                }

        }

#if DEBUG
        printf("Min x: %d || Max x: %d\n", min_x, max_x);
#endif

        /**
        * PART 1 : Just check every point in the row and if it's inside at least 
        *          one sensor's range, mark it as an imposible spot for another beacon  
        */
        int sum = 0;
        struct Coordinates aux;
#if DEBUG
        aux.y = 10;
#else
        aux.y = 2000000;
#endif
        for (int i = min_x; i <= max_x; i++){
               aux.x = i;
               for (int j = 0; j < n_sensors; j++){
                        if (distance(aux, array[j].s) <= distance(array[j].b, array[j].s) && distance(aux, array[j].b) != 0){
                                sum++;
#if DEBUG
                                printf("Found a spot --> [%d,%d]\n", aux.x, aux.y);
#endif
                                break;
                        }
               }
        }

        printf("[Part 1] In the %d'th row, there are %d spots where it can't be another beacon\n", aux.y, sum);

        /**
        * PART 2: Check outside perimeters
        * What i did in this part was, for every sensor :
        *       1) Calculate it's radius (distance to it's beacon + 1)
        *       2) Iterate the perimeter (starting in the uppper point and going clockwise)
        *                .......*.......
        *                ......*.*......
        *                .....*...*.....
        *                ....*.....*....
        *                ...*.......*...
        *                ..*....S...B*..
        *                ...*.......*...
        *                ....*.....*...
        *                .....*...*.....
        *                ......*.*......
        *                .......*.......
        *
        *       3) For every point in the perimeter, check if it's outside every sensor's range
        *       4) If we found such beacon, calculate the frequency and break the loop.
        *
        * I know it's kind ugly to copy paste the same loop 4 times but, it works, and it works REALLY fast so let it slide this time :p
        */
        uint64_t freq = 0;
#if DEBUG
        int max = 20;
#else
        int max = 4000000;
#endif
        int found = 0;

         uint64_t x, y;
         for (int i = 0; i < n_sensors && !found; i++){
                int radius = distance(array[i].s, array[i].b) + 1;
                
                // We start in the upper point [x , y + radius]
                x = array[i].s.x;
                y = array[i].s.y + radius;
                
                // Going right and down
                while (x <= array[i].s.x + radius && y >= array[i].s.y && !found){
                        if(x >= 0 && x <= max && y >= 0 && y <= max){ // if it's within ranges [0-0] and [4000000-4000000]
                                int covered = 0;
                                for (int j = 0; j < n_sensors; j++){
                                        if (distance((struct Coordinates){x,y}, array[j].s) <= distance(array[j].b, array[j].s)){
                                                covered = 1; // if the point is inise at least one sensor's range, mark it as "covered"
                                        }
                                        
                                }
                                if (!covered){ // if the point is not covered by any sensor, it's our distress beacon
                                        freq = x * 4000000 + y;
                                        found = 1;
                                        break;
                                }
                        }
                        x++; // In the last iteration this will be changed anyway, so later we
                        y--; // will adjust them here behind so the next loop starts in x = (array[i].s.x + radius ) and y = (array[i].s.y)
                }

                if(found) break; // if we found the beacon, break
                
                // Adjust the x and y
                x--;
                y++;
        
                // Going left and down: from [x , y + radius] to [x , y - radius] 
                while (x >= array[i].s.x && y >= array[i].s.y - radius && !found){
                        if(x >= 0 && x <= max && y >= 0 && y <= max){
                                int covered = 0;
                                for (int j = 0; j < n_sensors; j++){
                                        if (distance((struct Coordinates){x,y}, array[j].s) <= distance(array[j].b, array[j].s)){
                                                covered = 1;
                                        }
                                        
                                }
                                if (!covered){
                                        freq = x * 4000000 + y;
                                        found = 1;
                                        break;
                                }
                        }
                        x--;
                        y--;
                }

                if(found) break;

                x++;
                y++;
                
                // Going left and up: from [x , y - radius] to [x - radius , y] 
                while (x >= array[i].s.x - radius && y <= array[i].s.y && !found){
                        if(x >= 0 && x <= max && y >= 0 && y <= max){
                                int covered = 0;
                                for (int j = 0; j < n_sensors; j++){
                                        if (distance((struct Coordinates){x,y}, array[j].s) <= distance(array[j].b, array[j].s)){
                                                covered = 1;
                                        }
                                        
                                }
                                if (!covered){
                                        freq = x * 4000000 + y;
                                        found = 1;
                                        break;
                                }
                        }
                        x--;
                        y++;
                }

                if(found) break;

                x++;
                y--;
                
                // Going right and up: from [x - radius , y] to [x , y + radius] again. 
                while (x <= array[i].s.x && y <= array[i].s.y + radius && !found){
                        if(x >= 0 && x <= max && y >= 0 && y <= max){
                                int covered = 0;
                                for (int j = 0; j < n_sensors; j++){
                                        if (distance((struct Coordinates){x,y}, array[j].s) <= distance(array[j].b, array[j].s)){
                                                covered = 1;
                                        }
                                        
                                }
                                if (!covered){
                                        freq = x * 4000000 + y;
                                        found = 1;
                                        break;
                                }
                        }
                        x++;
                        y++;
                }
                
                // We completed the perimeter and found no distress beacon, go for the next sensor.
         }

        printf("[Part 2] The distress beacon [%lu,%lu] has a frequency of %lu\n",x, y, freq);


        fclose(file);
        return 0;
}
