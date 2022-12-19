#define DEBUG 0
#include "../../include/util.h"
#include <string.h>
#include <stdlib.h>



int cycle_length;
int divisors[MAX_BUFFER_SIZE];
int divisiors_length;

struct Monkey{
    int id;
    int items[MAX_BUFFER_SIZE];
    int nItems;
    char operator;
    char operand[MAX_BUFFER_SIZE];
    int divisible_test;
    int true_case,
        false_case;
    int nInspections;
};

void innit_monkey(char line[], FILE *file, struct Monkey monkeys[], int *mLenght, int *status){

    #if DEBUG
        printf("Inniting monkey\n");
    #endif

    *status = fgetline(line, file, MAX_BUFFER_SIZE);//the first line if just the monkey lenght
    *status = fgetline(line, file, MAX_BUFFER_SIZE);
    monkeys[*mLenght].id = *mLenght;
    char aux[MAX_BUFFER_SIZE];
    int pos = 0;
    #if DEBUG
        printf("\tGetting items\n");
    #endif

    while(get_number(line, aux, pos++)!=-1){
        monkeys[*mLenght].items[monkeys[*mLenght].nItems++] = atoi(aux);
    }
    *status = fgetline(line, file, MAX_BUFFER_SIZE);

    #if DEBUG
        printf("\tGetting operand\n");
    #endif

    char op;
    skip_multiple(line, aux, "*+/-" , 4, &op);
    monkeys[*mLenght].operator = op;
    aux[0] = ' ';
    no_whitespace(aux, aux);
    strcpy(monkeys[*mLenght].operand, aux);

    #if DEBUG
        printf("\tGetting divisibility\n");
    #endif
    *status = fgetline(line, file, MAX_BUFFER_SIZE);
    get_number(line, aux, 0);
    monkeys[*mLenght].divisible_test = atoi(aux);

    for(int i=0; i<divisiors_length; i++){
        if(divisors[i]==monkeys[*mLenght].divisible_test)
            continue;
        cycle_length *= monkeys[*mLenght].divisible_test;
        divisors[divisiors_length++] = monkeys[*mLenght].divisible_test;
        break;
    }

    if(divisiors_length==0){
        //printf("New divisior %d\n", monkeys[*mLenght].divisible_test);
        cycle_length *= monkeys[*mLenght].divisible_test;
        divisors[divisiors_length++] = monkeys[*mLenght].divisible_test;
    }

    *status = fgetline(line, file, MAX_BUFFER_SIZE);
    get_number(line, aux, 0);
    monkeys[*mLenght].true_case = atoi(aux);


    *status = fgetline(line, file, MAX_BUFFER_SIZE);
    get_number(line, aux, 0);
    monkeys[*mLenght].false_case = atoi(aux);


    monkeys[*mLenght].nInspections = 0;

    //printf("\tnew = old %c %s\n", monkeys[*mLenght].operator, monkeys[*mLenght].operand);

    (*mLenght)++;

    

    #if DEBUG
        printf("Finish\n");
    #endif
}

void print_monkey(struct Monkey monkey){
    printf("Monkey Nº%d\n\t", monkey.id);
    for(int i=0; i<monkey.nItems; i++)
        printf("%d ", monkey.items[i]);
    printf("\n");
}

void turn(struct Monkey monkeys[], int nMonkey){
    struct Monkey *current = &monkeys[nMonkey];
    #if DEBUG
    printf("\nMonkey %d:\n", current->id);
    #endif
    while(current->nItems>0){
        #if DEBUG
        printf("\tMonkey inspects an item with a worry level of %d.\n", current->items[0]);
        #endif
        current->nInspections++;

        /**
         * To prevent the values to overfow, we store them into 
         *          a long. Also, we perform the modulus of the value and the least common
         *          multiplier so the value stays in the [INT_MIN,INT_MAX] range without 
         *          affecting the result of the modulus of that number and the divisors of the monkeys. 
         * 
         * See: https://www.reddit.com/r/adventofcode/comments/zizi43/2022_day_11_part_2_learning_that_it_was_that/
         * 
         */
        long tmp = current->items[0];
        tmp %= cycle_length;
        int op;
        if(strncmp(current->operand, "old", 3)==0)
            op = current->items[0];
        else
            op = atoi(current->operand);


        switch (current->operator)
        {
        case '+':
            tmp = tmp + op;
            break;
        case '*':
            tmp = tmp * op;
            break;
        case '-':
            tmp = tmp - op;
            break;
        case '/':
            tmp = tmp / op;
            break;
        default:
            break;
        }
        #if DEBUG
        printf("\tnew worry level: %d.\n", current->items[0]);
        #endif
        tmp %= cycle_length;
        
        current->items[0] = tmp;
        if(current->items[0]%current->divisible_test == 0){
            #if DEBUG
            printf("\tSending %d to monkey %d\n", current->items[0], current->true_case);
            #endif
            monkeys[current->true_case].items[monkeys[current->true_case].nItems] = current->items[0];
            monkeys[current->true_case].nItems++;
        }else{
            #if DEBUG
            printf("\tSending %d to monkey %d\n", current->items[0], current->false_case);
            #endif
            monkeys[current->false_case].items[monkeys[current->false_case].nItems] = current->items[0];
            monkeys[current->false_case].nItems++;
        }

        for(int i=0; i<current->nItems-1; i++)
            current->items[i] = current->items[i+1];
        current->nItems--;
        #if DEBUG
        printf("\n");
        #endif
    }
}

int main(){
    FILE *file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }
    
    char line[MAX_BUFFER_SIZE];

    struct Monkey monkeys[MAX_BUFFER_SIZE];
    int monkeys_lenght = 0;

    int status = 1;
    cycle_length = 1;
    divisiors_length = 0;
    
    while (status != EOF){
        innit_monkey(line, file, monkeys, &monkeys_lenght, &status);
        if(status != EOF){
            status = fgetline(line, file, MAX_BUFFER_SIZE);
            #if DEBUG
                    printf("This sould be empty (%s)\n", line);
            #endif
        }

    }

    printf("Cycle length: %d\n", cycle_length);
    for(int i=0; i<divisiors_length; i++)
        printf("%d ", divisors[i]);
    printf("\n");
    //return 0;

    int nRounds = 10000;

    for(int i=0; i<nRounds; i++){
        #if DEBUG
        printf("***********TURN %d*********\n", i+1);
        #endif
        for(int j=0; j<monkeys_lenght; j++)
            turn(monkeys, j);
    }
        
    int max1 = 0,
        max2 = 0;
    for(int i=0; i<monkeys_lenght; i++){
        printf("%d\n", monkeys[i].nInspections);
        if(monkeys[i].nInspections>max1){
            max2 = max1;
            max1 = monkeys[i].nInspections;
        }else if(monkeys[i].nInspections>max2)
            max2 = monkeys[i].nInspections;
    }
    printf("Most active %d and %d\n",max1, max2);
    printf("Level of Monkey Business: %ld\n", ((long)max1)*max2);

    return 1;
}