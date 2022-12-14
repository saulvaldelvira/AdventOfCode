#pragma once
#ifndef ELEMENT_H
#define ELEMENT_H

#include "puzzle7.h"

#define TOTAL_SPACE 70000000
#define UPDATE_SIZE 30000000

typedef enum eType{
    eFILE, DIRECTORY
} Type;

/**
 * @brief Represents an element in the device
 */
typedef struct sElement{
    char    name[MAX_BUFFER_SIZE];
    Type    type;
    int     size;
    struct sElement* subElements[MAX_BUFFER_SIZE];
    struct sElement* father;
    int     nSubElements;
} Element;

/**
 * @brief allocs space for an element and sets all it's sub-elements to NULL
 */
Element* innitElement(void);

/**
 * @brief Adds a sub-element.
 * 
 * @param root  the root of the whole tree we are adding to. Needed for updating the size after.
 * @param e     elemento to add to
 */
void addSubElement(Element* root, Element* e, const char* name, Type type, int size);

/**
 * @brief frees an element and all it's sub-elements recursivelly
 */
void freeElement(Element* e);

/**
 * @brief   returns the pointer to the sub-element with the specified name.
 *          If it can't fine it, returns null
 */
Element* getSubElement(Element* e, const char* name);

/**
 * @brief Prints the whole tree
 */
void printTree(Element* e);

/**
 * @brief Updates the size of the element and returns it
 */
int getSize(Element* e);

/**
 * @brief Finds the smallest element that has a size >= nSize.
 * 
 * @param smallest the starting point of the search
 */
Element* findSmallestOver(Element* smallest, int nSize);
#endif