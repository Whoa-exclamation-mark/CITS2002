/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#ifndef BAKE_HISTORY_H
#define BAKE_HISTORY_H

#include <stdbool.h>
#include <time.h>
#include "../datatypes.h"

bool is_url(char *);

bool is_file(char *);

bool is_file_up_to_date(char*,Target*);

bool is_url_up_to_date(char*, Target*);

bool is_up_to_date(char*, Target*);

bool is_file_or_url(char *);

struct timespec* get_file_date(char*);

#endif //BAKE_HISTORY_H
