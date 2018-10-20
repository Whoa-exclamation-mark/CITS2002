/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "stack.h"
#include "../logger/logger.h"
#include "../helpers/helpers.h"

/**
 *  Implementation pop/push at top
 * -----    -----    -----    ------
 * | 1 | -> | 2 | -> | 3 | -> |null|
 * -----    -----    -----    ------
 */

stack *create()
{
    stack *s;
    //Create a part of memory for the stack
    s = (stack *) my_calloc(sizeof(stack));
    //The top of the stack should be null
    *s = NULL;
    //Return the stack
    return s;
}

bool is_empty(stack *s)
{
    //Check if the top is null (empty)
    return( (*s==NULL) ? true : false );
}

void *peak(stack *s)
{
    //Check if we are empty (as we can't peak an empty stack)
    if(is_empty(s)==true)
    {
        error("ERROR: Tried to peak at top of stack\n");
        exit(1);
    }
    //Return the item
    return &((*s)->content);
}

void push(stack *s, void *object)
{
    //Create a new link for the item
    item *new_item;
    new_item = (item *) my_calloc(sizeof(item));
    //Attach the previous link to the new link
    new_item->content=object;
    new_item->next=*s;
    //Make the stack start from the new item
    *s=new_item;
}

void* pop(stack *s)
{
    //Store the item when we remove it from the list
    item *memo;
    //Ensure that we are not poping an empty stack
    if(is_empty(s)==true)
    {
        error("ERROR: Tried to pop on an empty stack\n");
        exit(1);
    }
    //Make the temp var the current item
    memo=*s;
    //Move the stack up
    *s=memo->next;
    //Return the content of the poped item
    return memo->content;
}