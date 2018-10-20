/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "history.h"
#include "../logger/logger.h"

bool is_url(char* filename){
    return strstr(filename,"http://") != NULL
           || strstr(filename,"https://") != NULL
           || strstr(filename,"ftp://") != NULL;
};

bool is_url_up_to_date(char* url, Target* target){
    FILE *fp;
    char path[1035];

    const char * literal = "curl -I -s %s | sed -n -e 's/^.*Last-Modified: //p'";

    char* command = calloc(strlen(literal)+ strlen(url),strlen(literal)+ strlen(url));

    sprintf(command, literal, url);

    fp = popen(command, "r");
    if (fp == NULL) {
        error("ERROR: Failed to fetch URL (command failed)\n" );
        return false;
    }

    /* Read the output a line at a time - output it. */
    fgets(path, sizeof(path)-1, fp);

    struct tm tm;
    strptime(path, "%a, %d %b %G %T %Z", &tm);
    time_t t = mktime(&tm);

    pclose(fp);

    get_file_date(target->name);
    return t - get_file_date(target->name)->tv_sec>0;
}
