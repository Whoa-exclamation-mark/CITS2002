/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdbool.h>

#ifndef BAKE_DATATYPES_H
#define BAKE_DATATYPES_H

struct target {
    char* name;
    char* raw_dependencies;
    char** raw_commands;
    struct target** dependencies;
    char** file_dependencies;
    bool rebuild;
};

typedef struct target Target;

struct command {
    Target* parent;
    char* command;
    bool output_command;
    bool continue_on_error;
    bool should_run;
};

typedef struct command Command;

struct variable{
    char* name;
    char* value;
};

typedef struct variable Variable;



#endif //BAKE_DATATYPES_H
