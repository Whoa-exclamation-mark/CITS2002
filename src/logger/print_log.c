/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include "logger.h"

void error(char* format,...){
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

void warn(char* format,...){
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    va_end(argptr);
}

void info(char* format,...){
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stdout, format, argptr);
    va_end(argptr);
}

