#include "element.h"

Element* innitElement(void){
    #if DEBUG
    printf("\tAllocating memory...\n");
    #endif
    Element* e = malloc(sizeof(Element));
    for(int i=0; i<MAX_BUFFER_SIZE; i++)
        e->subElements[i] = NULL;
        
    e->nSubElements = 0;
    return e;
}

void addSubElement(Element* root, Element* e, const char* name, Type type, int size){
    #if DEBUG
    printf("Inniting element (%s)\n", name);
    #endif
    e->subElements[e->nSubElements] = innitElement();
    
    Element* aux = e->subElements[e->nSubElements];

    aux->type = type;
    strcpy(aux->name, name);
    aux->size = size;
    aux->father = e;
    
    e->nSubElements++;

    getSize(root);

    #if DEBUG 
    printf("Finish!\n");
    #endif
}

void freeElement(Element* e){
    if(e==NULL)
        return;
    for(int i=0; i<MAX_BUFFER_SIZE; i++)
        if(e->subElements[i]!=NULL){
            freeElement(e->subElements[i]);
            e->subElements[i] = NULL;
        }
    free(e);
}

static void printTreeR(Element* e, int nDeepness){
    for(int i=0; i<nDeepness; i++)
        printf("\t");
    printf("- %s (%d, size=%d)\n", e->name, e->type, e->size);
    for(int i=0; i < e->nSubElements; i++)
        printTreeR(e->subElements[i], nDeepness+1);
}

void printTree(Element* e){
    printTreeR(e, 0);
}

int getSize(Element* e){
    if(e==NULL)
        return 0;
    int result = 0;
    if(e->type == eFILE)
        result = e->size;
    for(int i=0; i<e->nSubElements; i++)
        result += getSize(e->subElements[i]);
    e->size = result;
    return result;
}

Element* findSmallestOver(Element* smallest, int nSize){
    if(smallest==NULL)
        return smallest;
    Element* result = smallest;
    for(int i=0; i<smallest->nSubElements; i++){
        Element* aux = findSmallestOver(smallest->subElements[i], nSize);
        if(aux!=NULL && aux->size >= nSize && aux->size <= result->size && aux->type == DIRECTORY)
            result = aux;
    }
    return result;
}

Element* getSubElement(Element* e, const char* name){
    for(int i=0; i<e->nSubElements; i++)
        if(e->subElements[i]!=NULL && strcmp(e->subElements[i]->name, name)==0)
            return e->subElements[i];
    return NULL; 
}