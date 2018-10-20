/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "helpers.h"
#include "../logger/logger.h"


void* my_malloc(size_t size){
    //Do the memory allocation
    void* alloc = malloc(size);
    //Error out if it has failed allocating memory
    if(alloc == NULL){
        error("ERROR: Attempted to allocate %i byte of memory but failed.", (int) size);
        exit(EXIT_FAILURE);
    }
    //Return pointer to the allocated memory
    return alloc;
}

void* my_calloc(size_t size){
    //Do the memory allocation and zero it out
    void* alloc = calloc(size,size);
    //Error out if it has failed allocating memory
    if(alloc == NULL){
        error("ERROR: Attempted to allocate %i byte of memory but failed.", (int) size);
        exit(EXIT_FAILURE);
    }
    //Return pointer to the allocated memory
    return alloc;
}

char** get_file_string(char* file){
    //Check if the file exists (Bakefile) and if it doesn't try bakefile instead
    if(strcmp("Bakefile",file) == 0 && access(file, F_OK)!=-1){
        file = "bakefile";
    }
    //Create a pointer for the file struct
    FILE *_file;
    //used for # of lines
    int i = 0;
    //Used in importing whole file into memory
    long size;
    char *string;

    //Open file and start importing it
    //Todo ask if I should just use BUF_SIZE
    if((_file = fopen(file,"r"))!=NULL){
        //Go to the end of the file
        fseek(_file, 0, SEEK_END);
        //Get length of file
        size = ftell(_file)+1;
        //Go back to start
        fseek(_file, 0, SEEK_SET);

        //Allocate memory for the string
        string = my_malloc((size_t) (size + 1));
        //Read in the whole file
        fread(string, (size_t) size, 1, _file);
        //Close the file
        fclose(_file);
    } else{
        //Error out if we cannot open the file
        error("ERROR: Could not open the %s in read mode. Please make sure that the file exists and rerun Bake!\n",file);
        exit(EXIT_FAILURE);
    }

    //Iterate over string and count each \n
    while (*string) i += *(string++) == '\n';

    //Allocate a new array of strings with length lines in the file + 1 (null byte)
    char **finalString = my_calloc((size_t) (i + 1));

    //Create a buffer for reading each line in
    char buff[BUFSIZ] = {0};
    //Index of the final string
    int v = 0;
    if((_file = fopen(file,"r"))!=NULL){
        //Get each line
        while (fgets(buff, sizeof(buff), _file)){
            //Allocate memory with length of the string
            finalString[v] = my_calloc(strlen(buff));
            //Copy from buffer into the allocated string
            strcpy(finalString[v],buff);
            //If the string has a \n at the end then
            if(finalString[v][strlen(finalString[v])-1]=='\n')
                //Remove it
                finalString[v][strlen(finalString[v])-1]='\0';
            //Increment index
            v++;
        }
    } else{
        //Fail if we cannot open file
        error("ERROR: Could not open the %s in read mode. Please make sure that the file exists and rerun Bake!\n",file);
        exit(EXIT_FAILURE);
    }
    //Null terminate array
    finalString[v] = NULL;
    //Close file
    fclose(_file);
    //Return pointer to array
    return finalString;
}

void space_strip(char* string, char* dest){
    //Get length of string
    int length = (int) strlen(string);

    //Return if it has no length
    if(length==0){
        dest="\0";
        return;
    }

    //Initialise counters for distance to string we want
    int spaceNumBegin = 0;
    int spaceNumEnd = 0;


    //from beginning
    for(int i = 0; i<length;i++){
        //Continue until we hit the string
        if(*(string+i) == ' ' || *(string+i) == '\t'){
            spaceNumBegin++;
            continue;
        }
        //End when we find start of sub string
        break;
    }

    //from end
    for(int i = length-1; i>0;--i){
        //Continue until we hit the string
        if(*(string+i) == ' '||*(string+i) == '\t'){
            spaceNumEnd++;
            continue;
        }
        //End when we find end of sub string
        break;
    }


    //strip space from left and right side
    int i = spaceNumBegin;
    //iterate through and copy string
    for(; i<length-spaceNumEnd; i++){
        dest[i-spaceNumBegin] = *(string+i);
    }
    //Null terminate the new string
    dest[i-spaceNumBegin]='\0';
}

void remove_from_array(void** array, void** needle){
    //Get index of the needle
    long index = needle-array;
    //Get length of null terminated array
    int length = 0;
    void** or_p = array;
    while (*array++) length++;
    //Reset pointer
    array = or_p;
    //Copy array without needle
    for(long i = index; i < length+1; ++i) {
        array[i] = array[i + 1];
    };
};

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = (int) strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = (int) strlen(with);

    // count the number of replacements needed
    ins = orig;
    tmp = strstr(ins, rep);
    for (count = 0; tmp; ++count) {
        ins = tmp + len_rep;
        tmp = strstr(ins, rep);
    }

    tmp = result = my_malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = (int) (ins - orig);
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

void push_on_array(void** array, void* item){
    //Get to end of array (null terminated)
    while (*array){
        array++;
    }
    //Make end equal to item (since we used malloc we don't need to null the next pointer - be careful about this!)
    *array = item;
}

