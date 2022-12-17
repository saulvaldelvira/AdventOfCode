#include <stdio.h>

#include <stdlib.h>
#define FILENAME	"input.txt"
#define MAX_BUFFER_SIZE 10000
#define DEBUG		1

#define LEFT	'L'
#define RIGHT	'R'
#define UP	'U'
#define DOWN	'D'

struct Coordinates {
	int x, y;
};

int fgetline(char dest[], FILE* f,  int max){
    int c, nBuffer=0;
   	for(int i=0; i<max; i++){
        c=fgetc(f);
		if(c==EOF){
			dest[nBuffer++] = '\0';
			return EOF;
		}
		else if(c=='\n')
			break;
		else
			dest[nBuffer++]=c;
   	}
    dest[nBuffer++] = '\0';
	return 1;
}

#if DEBUG
	int minXMap = -12,
	maxXMap = 26,
	minYMap = -10,
	maxYMap = 22;
#else
	int minXMap = -51,
	maxXMap = 60,
	minYMap = -11,
	maxYMap = 116;
#endif

/**
 * @brief Prints the rope
 */
void print_rope(struct Coordinates rope[]){
	printf("\n");
	for(int i=minXMap; i<=maxXMap; i++){
		for(int j=minXMap; j<=maxYMap; j++){
			char c = '.';
			if(rope[9].x == j && rope[9].y == i)
				c = 'T';
			for(int k=8; k>=0; k--)
				if(rope[k].x == j && rope[k].y == i)
					c = '0' + k;
			if(rope[0].x == j && rope[0].y == i)
				c = 'H';
			printf("%c", c);
		}
		printf("\n");
	}
	printf("\n");		
}

void printAll(struct Coordinates p[], int l){
	
	for(int i=minXMap; i<=maxXMap; i++){
		for(int j=minXMap; j<=maxYMap; j++){
			char c='.';
			for(int k=0; k<l; k++)
				if(p[k].x==j && p[k].y == i)
					c = '#';
			printf("%c", c);
		}
		printf("\n");
	}
}

void check_pos(struct Coordinates *coord, struct Coordinates positions[], int *lenght){
	for(int i=0; i<*lenght; i++){
		if(positions[i].x == coord->x && positions[i].y == coord->y)
			return;
	}
#if DEBUG
	printf("\tNew position [%d,%d]\n", coord->x, coord->y);
#endif
	if(*lenght >= MAX_BUFFER_SIZE){
		fprintf(stderr, "The buffer for the coordinates got out of bounds\n");
		return;
	}
		
	struct Coordinates c;
	c.x = coord->x;
	c.y = coord->y;
	positions[(*lenght)++] = c;
}

void adjust(struct Coordinates *head, struct Coordinates *tail){
	
	#if DEBUG
		int oldX = (*tail).x,
			oldY = (*tail).y;
		printf("\tHEAD [%d,%d] TAIL[%d,%d]\n", (*head).x,(*head).y, (*tail).x, (*tail).y);
#endif

		if((*head).x - (*tail).x >= 2){
			(*tail).x++;
			if((*head).y - (*tail).y < 0)
				(*tail).y--;
			else if((*head).y - (*tail).y > 0)
				(*tail).y++;
		}
			
		if((*head).y - (*tail).y >= 2){
			(*tail).y++;
			if((*head).x - (*tail).x < 0)
				(*tail).x--;
			else if((*head).x - (*tail).x > 0)
				(*tail).x++;
		}

		if((*head).x - (*tail).x <= -2){
			(*tail).x--;
			if((*head).y - (*tail).y < 0)
				(*tail).y--;
			else if((*head).y - (*tail).y > 0)
				(*tail).y++;
		}
			
		if((*head).y - (*tail).y <= -2){
			(*tail).y--;
			if((*head).x - (*tail).x < 0)
				(*tail).x--;
			else if((*head).x - (*tail).x > 0)
				(*tail).x++;
		}
#if DEBUG
		printf("\tTail from [%d,%d] to [%d,%d]\n\n", oldX, oldY, (*tail).x, (*tail).y);
#endif
}

/**
 * @brief Moves the rope in a direction, a given ammount of times
 */
void move(struct Coordinates rope[], char dir, int amm, struct Coordinates positions[],  int *lenght){
	int aux;
	for(int i=0; i<amm; i++){
		if(dir==UP)
			rope[0].y++;
		else if(dir==DOWN)
			rope[0].y--;
		else if(dir==RIGHT)
			rope[0].x++;
		else if(dir==LEFT)
			rope[0].x--;
		

		for(int i=0; i<9; i++){
			adjust(&rope[i], &rope[i+1]);
		}
		check_pos(&rope[9], positions, lenght);
#if DEBUG
		print_rope(rope);
#endif
	}
}

/**
 * @brief 	Just to check the lowest and highest cords the rope has gone into. So we can adjust the previous
 * 			minXMap maxYmap etc.. up there.
 */
void maxs_mins(int *maxX, int *maxY, int *minX, int *minY, int l, struct Coordinates m[]){
	for(int i=0; i<l; i++){
		if(m[i].x > *maxX)
			*maxX = m[i].x;
		if(m[i].x < *minX)
			*minX = m[i].x;
		if(m[i].y > *maxY)
			*maxY = m[i].y;
		if(m[i].y < *minY)
			*minY = m[i].y;
	}
}

int main(){
//Initializing
	FILE *file = fopen(FILENAME, "r");
    	if(!file){
        	fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        	return -1;
    	}

	struct Coordinates rope[10];

	/*The clean way to do this is definign a macro 
		(#define ROPE_SIZE 9)
		but im tirreeeeed. So leave it like thisss
	*/
	for(int i=0; i<=9; i++){
		rope[i].x = 0;
		rope[i].y = 0;
	}

	char 	line[MAX_BUFFER_SIZE],
			digit[MAX_BUFFER_SIZE];

	char direction;
	int ammount;

	struct Coordinates positions[MAX_BUFFER_SIZE];
	int positions_lenght = 0;

//Loop
	int status = 1;
	while(status != EOF){
		status = fgetline(line, file, MAX_BUFFER_SIZE);

		direction = line[0];
		int i;
		//we just skip the first 2 chars bc they are the direction and a whitespace, i know this is a dirty and opaque way of doing this.
		for(i=0; line[i+2]!='\n' && line[i+2]!='\0'; i++)
			digit[i] = line[i+2];
		digit[i] = '\0';

		ammount = atoi(digit);
		move(rope, direction, ammount, positions,  &positions_lenght);
		

    }

	printAll(positions, positions_lenght);
	
	printf("The tail visits %d different positions\n", positions_lenght);

	fclose(file);
    	return 0;
}
