#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

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

        char line[512];
        int next = 1;
        int n_lines = 0;
        while (next != EOF){
                next = get_line(line, file, 512);
                if (line[0] != '\n' && line[0] != '\0'){
                        n_lines++;
                }
        }

        rewind(file);

        Pair array[n_lines];

        int min_x = INT_MAX;
        int max_x = INT_MIN;
        for (int i = 0; i < n_lines; i++){
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

        // PART 1
        int sum = 0;
        struct Coordinates aux;
#if DEBUG
        aux.y = 10;
#else
        aux.y = 2000000;
#endif
        for (int i = min_x; i <= max_x; i++){
               aux.x = i;
               for (int j = 0; j < n_lines; j++){
                        if (distance(aux, array[j].s) <= distance(array[j].b, array[j].s) && distance(aux, array[j].b) != 0){
                                sum++;
#if DEBUG
                                printf("Found a spot --> [%d,%d]\n", aux.x, aux.y);
#endif
                                break;
                        }
               }
        }

        printf("In the %d'th row, there are %d spots where it can't be another beacon\n", aux.y, sum);

        // PART 2
        int freq;
#if DEBUG
        int max = 20;
#else
        int max = 4000000;
#endif
        int found;
        struct Coordinates aux;

        for (int y = 0; y <= max; y++){
                aux.y = y;
                for (int x = 0; x <= max; x++){
                        aux.x = x;
                        found = 1;
                        for (int j = 0; j < n_lines; j++){
                                if (distance(aux, array[j].s) > distance(array[j].b, array[j].s)){
                                        continue;
                                }
                                found = 0;
                        }
                        if (found){
                                freq = x * 4000000 + y;
                                printf("Distress beacon [%d,%d]\n", x, y);
                                break;
                        }
                }
                if (found) break;
         }

        printf("The distress beacon has a frequency of %d\n", freq);


        fclose(file);
        return 0;
}
