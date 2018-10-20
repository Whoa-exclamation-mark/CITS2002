/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include "logger.h"
#include "../constants.h"

void error(char* format,...){
    //Check if we are in silent mode
    if(SILENT)
        return;
    //Now print the log to stderr if we aren't in silent mode
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

void warn(char* format,...){
    //Check if we are in silent mode
    if(SILENT)
        return;
    //Now print the log to stdout if we aren't in silent mode
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    va_end(argptr);
}

void info(char* format,...){
    //Check if we are in silent mode
    if(SILENT)
        return;
    //Now print the log to stdout if we aren't in silent mode
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    va_end(argptr);
}

