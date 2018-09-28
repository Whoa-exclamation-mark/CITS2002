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


    char *orginal_string = my_malloc(strlen (string)+1);
    strcpy(orginal_string, string);

    while (*string) i += *(string++) == '\n';


    char **finalString = my_malloc(i + 1);

    strcpy(string, orginal_string);


    int line_index=0;
    int index = 0;
    int previous_index = 0;
    //O(n^2) impl.
    while(*string){
        if(*string==end_of_line){
            finalString[line_index]=my_malloc(index - previous_index + 2);
            for (int j = previous_index; j < index; ++j) {
                finalString[line_index][j-previous_index] = orginal_string[j];
            }
            line_index++;
            previous_index = index+1;
        };
        string++;
        index++;
    }

    free(orginal_string);
    free(string);
    return finalString;
}


//O(n) imp of removing spaces from the start and end of strings
char* space_strip(char* string){
    int length = (int) strlen(string);

    int spaceNumBegin = 0;
    int spaceNumEnd = 0;


    //from beginning
    for(int i = 0; i<length;i++){
        if(*(string+i) == ' '){
            spaceNumBegin++;
            continue;
        }
        break;
    }

    //from end
    for(int i = length-1; i>0;--i){
        if(*(string+i) == ' '){
            spaceNumEnd++;
            continue;
        }
        break;
    }


    char * new_string = my_malloc((size_t) (length - spaceNumBegin - spaceNumEnd));

    //strip space from left and right side
    for(int i=spaceNumBegin; i<length-spaceNumEnd; i++){
        new_string[i-spaceNumBegin] = *(string+i);
    }

    new_string[length-spaceNumBegin-spaceNumEnd] = '\0';


    return &new_string[0];

}