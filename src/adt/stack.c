//
// Created by Campbell Millar on 19/10/18.
//

#include <stdlib.h>
#include "stack.h"
#include "stdbool.h"
#include "stdio.h"


stack *create()
{
    stack *s;
    s = (stack *) calloc(sizeof(stack), sizeof(stack));
    //if(s==NULL)
    //{
    //    printf("OUT OF MEMORY!\n");
    //    exit(1);
    //}

    *s = NULL;
    return s;
}

bool is_empty(stack *s)
{
    return( (*s==NULL) ? true : false );
}

void *peak(stack *s)
{
    if(is_empty(s)==true)
    {
        printf("TRIED TO FIND THE TOP OF AN EMPTY STACK!\n");
        exit(1);
    }

    return &((*s)->content);
}

void push(stack *s, void *object)
{
    item *new_item;
    new_item = (item *) calloc(sizeof(item),sizeof(item));
    //if(new_item==NULL)
    //{
    //    printf("OUT OF MEMORY!\n");
    //    exit(1);
    //}

    new_item->content=object;
    new_item->next=*s;
    *s=new_item;
}

void* pop(stack *s)
{
    item *memo;

    if(is_empty(s)==true)
    {
        printf("TRIED TO POP AN EMPTY STACK!\n");
        exit(1);
    }

    memo=*s;
    *s=memo->next;

    return memo->content;
    //free(memo);
}