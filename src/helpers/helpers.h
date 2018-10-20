/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#ifndef BAKE_HELPERS_H
#define BAKE_HELPERS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define end_of_line '\r'
#else
#define end_of_line '\n'
#endif
/**
 *
 * @param bytes
 * @return
 */
void *my_malloc(size_t bytes);

/**
 *
 * @return
 */
char** get_file_string(char*);

/**
 *
 * @return
 */
void space_strip(char*, char*);

/**
 *
 * @return
 */
void remove_from_array(char**,char**);

char* str_replace(char*,char*,char*);

//todo
void* my_calloc(size_t);

/**
 *
 * @param array
 * @param item
 */
void push_on_array(void** array, void* item);


#endif //BAKE_HELPERS_H
