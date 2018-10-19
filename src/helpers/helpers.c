//
// Created by Campbell Millar on 18/9/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"

void* my_malloc(size_t size){
    void* alloc = malloc(size);
    if(alloc == NULL){
        printf("Attempted to allocate %i byte of memory but failed.", (int) size);
        exit(EXIT_FAILURE);
    }
    return alloc;
}

//Get the full string from the file and then separate it out into array of where \n exists (O(n^2)) time
char** get_file_string(char* file){
    FILE *_file;

    int i = 0;

    long size;
    char *string;

    if((_file = fopen(file,"r"))!=NULL){
        //Go to the end of the file
        fseek(_file, 0, SEEK_END);
        //Get length of file
        size = ftell(_file)+1;
        //Go back to start
        fseek(_file, 0, SEEK_SET);

        string = malloc((size_t) (size + 1));
        fread(string, (size_t) size, 1, _file);
        fclose(_file);
    } else{
        printf("Could not open the Bakefile in read mode. Please make sure that the file exists and rerun Bake!");
        exit(EXIT_FAILURE);
    }

    char *orginal_string = string;

    while (*string) i += *(string++) == '\n';


    char **finalString_2 = calloc(i + 1,i+1);

    string = orginal_string;
    char buff[BUFSIZ] = {0};
    int v = 0;
    if((_file = fopen(file,"r"))!=NULL){
        while (fgets(buff, sizeof(buff), _file)){
            finalString_2[v] = calloc(sizeof(buff),sizeof(buff));
            strcpy(finalString_2[v],buff);
            if(finalString_2[v][strlen(finalString_2[v])-1]=='\n')finalString_2[v][strlen(finalString_2[v])-1]='\0';
            v++;
        }
    }

    finalString_2[v] = NULL;


    return finalString_2;
}


//O(n) imp of removing spaces from the start and end of strings
void space_strip(char* string, char* dest){
    int length = (int) strlen(string);

    if(length==0){
        dest="\0";
        return;
    }

    int spaceNumBegin = 0;
    int spaceNumEnd = 0;


    //from beginning
    for(int i = 0; i<length;i++){
        if(*(string+i) == ' ' || *(string+i) == '\t'){
            spaceNumBegin++;
            continue;
        }
        break;
    }

    //from end
    for(int i = length-1; i>0;--i){
        if(*(string+i) == ' '||*(string+i) == '\t'){
            spaceNumEnd++;
            continue;
        }
        break;
    }

    //printf("%i\n",(length - spaceNumBegin - spaceNumEnd));
    //char * new_string = my_malloc(length - spaceNumBegin - spaceNumEnd);

    //strip space from left and right side
    int i = spaceNumBegin;
    for(; i<length-spaceNumEnd; i++){
        dest[i-spaceNumBegin] = *(string+i);
    }
    dest[i-spaceNumBegin]='\0';
}

void remove_from_array(char** array, char** needle){
    long index = needle-array;
    int length = 0;
    char** or_p = array;
    while (*array++) length++;
    array = or_p;
    for(long i = index; i < length+1; ++i) {
        array[i] = array[i + 1];
    };
};

// You must free the result if result is non-NULL.
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
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}