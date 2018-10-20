//
// Created by Campbell Millar on 19/10/18.
//

#ifndef BAKE_STACK_H
#define BAKE_STACK_H

#include <stdbool.h>

typedef struct item
{
    void* content;
    struct item *next;
} item;

typedef item *stack;

stack *create();

bool is_empty(stack*);

void* peak(stack*);

void push(stack*, void*);

void* pop(stack*);

#endif //BAKE_STACK_H
