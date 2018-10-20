/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include "history.h"

bool is_file_up_to_date(char* filename, Target* target){
    return get_file_date(filename)->tv_sec - get_file_date(target->name)->tv_sec>=0;
};

struct timespec* get_file_date(char* filename){
    struct stat attrib;
    stat(filename, &attrib);
    return &(attrib.st_mtimespec);
}
