#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef DEBUG
        #define FILENAME "test.txt"
#else
        #define FILENAME "input.txt"
#endif

typedef struct Valve {
        int rate;
        struct Valve **adjacency;
} Valve;

Valve* valve_init(int rate, int n_adj){
        Valve v;
        v.rate = rate;
        v.adjacency = calloc(n_adj, sizeof(Valve*));
        return &v;
}

void valve_free(Valve *v){
        for()
}

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

int main(){
        FILE *file = fopen(FILENAME, "rt");
        if (!file){
                fprintf(stderr, "Error: could not open the file\n");
                return -1;
        }

        // Count number of valves
        char line[512];
        int next = 1;
        int n_valves = 0;
        while (next != EOF){
                next = get_line(line, file, 512);
                if (line[0] != '\n' && line[0] != '\0'){
                        n_valves++;
                }
        }
}
