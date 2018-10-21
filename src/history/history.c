/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <unistd.h>
#include "history.h"

bool is_file_or_url(char* filename){
    //Check if it is either a file or url
    return is_url(filename) || is_file(filename);
}

bool is_file(char* filename){
    //Try to see if the file exists
    return access(filename, F_OK)!=-1;
}

bool is_up_to_date(char* filename,Target* target){
    //Check that if it is a file
    if(is_file(filename)){
        //Then test the file
        return is_file_up_to_date(filename,target);
    }
    //Else we test the url
    return is_url_up_to_date(filename,target);
}
