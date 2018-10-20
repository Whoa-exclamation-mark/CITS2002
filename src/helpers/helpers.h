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
 * A helper function to allocate memory but check if it has been allocated, else exit as fatal issue
 * @return pointer to allocated space
 */
void* my_malloc(size_t);

/**
 * A helper function to allocate memory and zero it out but check if it has been allocated, else exit as fatal issue
 * @return pointer to allocated space
 */
void* my_calloc(size_t);

/**
 * Get a full file in a string array without \n
 * @return the array of strings
 */
char** get_file_string(char*);

/**
 * Strip white space from strings (" " and "\t"). It requires input string and a destination buffer
 */
void space_strip(char*, char*);

/**
 * Removes a given pointer from an array
 */
void remove_from_array(void**,void**);

/**
 * Replaces in a string with a needle and the replacement
 * @return the changed string (address changes!)
 */
char* str_replace(char*,char*,char*);

/**
 * Push a given item onto a null terminated array. WARNING - This has no checks for the length of the array. Be careful!
 */
void push_on_array(void**, void*);


#endif //BAKE_HELPERS_H
