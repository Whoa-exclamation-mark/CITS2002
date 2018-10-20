/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#ifndef BAKE_STACK_H
#define BAKE_STACK_H

#include <stdbool.h>
/**
 * The structure of a item on a stack
 */
typedef struct item
{
    void* content;
    struct item *next;
} item;
typedef item *stack;

/**
 * Method to create a linked stack
 * @return pointer to the created stack
 */
stack *create();

/**
 * Checks if the stack is empty (takes in a stack)
 * @return boolean based on emptyness of stack
 */
bool is_empty(stack*);

/**
 * Peaks on the fist element of a stack (takes in a stack)
 * @return value of the top most item
 */
void* peak(stack*);

/**
 * Push on a given stack (takes in a stack and value)
 */
void push(stack*, void*);

/**
 * Pops on the fist element of a stack (takes in a stack)
 * @return value of the top most item
 */
void* pop(stack*);

#endif //BAKE_STACK_H
