/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <unistd.h>
#include "history.h"

bool is_file_or_url(char* filename){
    return is_url(filename) || is_file(filename);
}

bool is_file(char* filename){
    return access(filename, F_OK)!=-1;
}

bool is_up_to_date(char* filename,Target* target){
    if(is_file(filename)){
        return is_file_up_to_date(filename,target);
    }
    return is_url_up_to_date(filename,target);
}