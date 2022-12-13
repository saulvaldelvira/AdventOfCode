#include "puzzle7.h"
#include "element.h"

FILE* file;
char    line[MAX_BUFFER_SIZE];

/**
 * @brief   Used to buffer a line. If the valid value is 1, it means the contained line
 *          should be the new one to read.
 * 
 */
struct Buffer {
    char    buffer[MAX_BUFFER_SIZE];
    int     valid;
} buffer;

Element *root,      //root element
        *working;   //current working element

int fgetline(char dest[], FILE* f,  int max){
    //If the buffer stores a valid line, return it
    if(buffer.valid){
        #if DEBUG
        printf("Retrieved from buffer!\n");
        #endif
        strcpy(dest, buffer.buffer);
        buffer.valid = 0;
        return 1;
    }
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

void list(){
    #if DEBUG
        printf("\n");
    #endif
    int state = 1;
    char    aux1[MAX_BUFFER_SIZE],
            aux2[MAX_BUFFER_SIZE];
    while(state != EOF){
        state = fgetline(line, file, MAX_BUFFER_SIZE);

        /**
         * @brief if it ends reading the list and catches a new command, buffers it and returns.
         * 
         */
        if(line[0]==COMMAND){
            #if DEBUG
            printf("Buffered %s\n", line);
            #endif
            strcpy(buffer.buffer, line);
            buffer.valid = 1;
            return;
        }

        //Get the type (or numeric value if file)
        int i;
        for(i=0; line[i]!=' '; i++)
            aux1[i] = line[i];
        aux1[i] = '\0';
        
        #if DEBUG
        printf("\t%s ", aux1);
        #endif
        
        //get the name
        int j;
        for(j=0, i++; line[i]!='\0' && line[i]!=' ' && line[i]!='\n'; i++, j++)
            aux2[j] = line[i];
        aux2[j] = '\0';

        #if DEBUG
        printf("\t%s\n", aux2);
        #endif

        if(strcmp(aux1, "dir")==0)
            addSubElement(root, working, aux2, DIRECTORY, 0);
        else
            addSubElement(root, working, aux2, eFILE, atoi(aux1));
        

    }
}

void processCommand(){
    char    command[MAX_BUFFER_SIZE],
            arg[MAX_BUFFER_SIZE];
    int n, i;

    //Get the command
    for(i=2, n=0; line[i]!=' ' && line[i]!='\n' && line[i]!='\0'; i++)
        command[n++] = line[i];
    command[n] = '\0';

    #if DEBUG
    printf("Processing command \"%s\"", command);
    #endif

    if(strcmp(command, "ls")==0)
        list();
    else if(strcmp(command, "cd")==0){
        //if cd, get the argument
        #if DEBUG
        printf(" Argument: ");
        #endif

        for(i++, n=0; line[i]!='\0' && line[i]!=' ' && line[i]!='\n'; i++, n++)
            arg[n] = line[i];
        arg[n] = '\0';

        #if DEBUG
        printf("\"%s\"\n", arg);
        #endif

        if(strcmp(arg, "..")==0)
            working = working->father;
        else
            working = changeDir(working, arg);


    }
    #if DEBUG
    printf("\n");
    #endif
}

/**
 * @brief Finds the sum of the sizes of all dirs with a size >= max
 */
int findDirs(Element* e, int max){
    if(e==NULL ||e->type == eFILE)
        return 0;
    int result = 0;
    getSize(e);
    if(e->size<=max)
        result += e->size;
    for(int i=0; i<e->nSubElements; i++)
        result += findDirs(e->subElements[i], max);
    return result;
}

int main(){
    file = fopen(FILENAME, "r");
    if(!file){
        fprintf(stderr, "Error: could not find filename %s\n", FILENAME);
        return -1;
    }

    //Sets up root and working
    root = innitElement();
    strcpy(root->name, "/");
    root->type = DIRECTORY;
    root->father = root;
    root->size=0;
        //the first command is "cd /""
    working = innitElement();
    working->subElements[0] = root;
    working->nSubElements++;
    strcpy(working->subNames[0], "/");


    buffer.valid = 0;
    int state = 1;

    while(state != EOF){
        state = fgetline(line, file, MAX_BUFFER_SIZE);
        printf("%s\n", line);
        processCommand();
    }

    printTree(root);


    int need = UPDATE_SIZE - (TOTAL_SPACE - root->size);
    Element* sma = findSmallestOver(root, need);

    printf("The smallest element to free at least %d blocks is %s with %d\n",need, sma->name,  sma->size);

    freeElement(root);
    freeElement(working);
    root = NULL;
    working = NULL;
    fclose(file);
    return 1;
}

