/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#ifndef BAKE_HISTORY_H
#define BAKE_HISTORY_H

#include <stdbool.h>
#include <time.h>
#include "../datatypes.h"

/**
 * Check if a given string represents a url
 * @return if it is a url
 */
bool is_url(char *);

/**
 * Check if a given string represents a file
 * @return if it is a file
 */
bool is_file(char *);

/**
 * Check if we need to update a file
 * @return if file time > than target time
 */
bool is_file_up_to_date(char*,Target*);

/**
 * Check if we need to update a url
 * @return if url time > than target time
 */
bool is_url_up_to_date(char*, Target*);

/**
 * Check if a given file/url is up to date
 * @return if time > than target time
 */
bool is_up_to_date(char*, Target*);

/**
 * Checks if given string is a url or file
 * @return if it is a url/file
 */
bool is_file_or_url(char *);

/**
 * get file date
 * @return file date in a timespec
 */
struct timespec* get_file_date(char*);

#endif //BAKE_HISTORY_H
