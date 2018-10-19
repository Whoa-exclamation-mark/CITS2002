//
// Created by Campbell Millar on 20/9/18.
//

#include <stdbool.h>

#ifndef BAKE_DATATYPES_H
#define BAKE_DATATYPES_H

struct target {
    char* name;
    char** raw_commands;
    struct target* dependencies;
    struct command* commands;
    bool has_failed;
};

typedef struct target Target;

struct command {
    //THIS IS NOT AN ARRAY JUST ONLY A POINTER TO THE TARGET ITSELF!
    Target* parent;
    char* command;
    //This is a boolean!
    bool output_command;
    bool continue_on_error;
};

typedef struct command Command;

struct variable{
    char* name;
    char* value;
};

typedef struct variable Variable;



#endif //BAKE_DATATYPES_H
