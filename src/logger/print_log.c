//
// Created by Campbell Millar on 2/10/18.
//

#include <stdio.h>
#include "logger.h"

#ifndef FILE_LOG

void error(char* string){
    fprintf(stderr,string);
}

void warn(char* string){
    fprintf(stderr,string);
}

void info(char* string){
    fprintf(stdout,string);
}

#endif
