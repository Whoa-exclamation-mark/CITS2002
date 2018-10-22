/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "history.h"
#include "../logger/logger.h"
#include "../helpers/helpers.h"

bool is_url(char* filename){
    //Check if string has http://,https://, or ftp://
    return strstr(filename,"http://") != NULL
           || strstr(filename,"https://") != NULL
           || strstr(filename,"ftp://") != NULL;
}

bool is_url_up_to_date(char* url, Target* target){
    //Create a file pointer
    FILE *fp;

    //Allocate space for the return data
    char data[1035];

    //The command w/o url
    const char * literal = "curl -I -s %s | sed -n -e 's/^.*Last-Modified: //p'";
    //Allocate space for the command
    char* command = my_calloc(strlen(literal)+ strlen(url));
    //Format command from literal
    sprintf(command, literal, url);

    //Open the process in read mode
    fp = popen(command, "r");
    //Check if we have opened a stream
    if (fp == NULL) {
        error("\033[31mERROR: Failed to fetch URL (command failed)\033[0m\n" );
        return false;
    }

    // Read the output
    fgets(data, sizeof(data)-1, fp);

    //Phase time into time_t to be used
    struct tm tm;
    strptime(data, "%a, %d %b %G %T %Z", &tm);
    time_t t = mktime(&tm);

    //Close the stream
    pclose(fp);

    //Find the diff between url date and target date
    return t - get_file_date(target->name)->tv_sec>0;
}
