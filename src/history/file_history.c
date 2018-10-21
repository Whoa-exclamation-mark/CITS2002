/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include "history.h"
#include "../helpers/helpers.h"

bool is_file_up_to_date(char* filename, Target* target){
    //Minus the file modified with the target and if it is greater than 0 then we need to recompile it
    return get_file_date(filename)->tv_sec - get_file_date(target->name)->tv_sec>=0;
}

struct timespec* get_file_date(char* filename){
    //Make a struct location for stat
    struct stat* attrib = my_malloc(sizeof(struct stat));
    //Get stat
    stat(filename, attrib);
    //Get modified time spec
    return &(attrib->st_mtimespec);
}
