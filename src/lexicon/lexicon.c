//
// Created by Campbell Millar on 20/9/18.
//

#include <string.h>
#include "lexicon.h"
#include <stdio.h>
#include <stdlib.h>
#include "../helpers/helpers.h"

Variable* variables[100] = {0};

void sanitize(char** strings){
    //remove any issues from the string

    //find end of array



    char** original_pointer = strings;

    int length = 0;

    while (*strings) {
        length++;
        strings++;
    }

    //strings--;
    //Remove any blank lines

    strings--;
    while((strings != original_pointer)){
        //printf("%s\n", *strings);
        if(*strings!= NULL && *strings[0]!='\0' && *(*strings+strlen(*strings)-1)=='\\') {
            //printf("Found wrapping line\n");
            *(*strings+strlen(*strings)-1)='\0';
            char * data = calloc(BUFSIZ*2,BUFSIZ*2);
            space_strip(*(strings+1),*(strings+1));
            sprintf(data,"%s%s",*strings,*(strings+1));
            *strings = data;
            remove_from_array(original_pointer,strings+1);
        }
        strings--;


        //strings++;
    }

    strings = original_pointer;

    while(*strings) {
        char *strip_str = calloc(BUFSIZ * 2, BUFSIZ * 2);
        space_strip(*strings, strip_str);
        if (strip_str[0] == '#') {
            //printf("Found comment line\n");
            *strings[0] = '\0';
        }
        if (strip_str[0] == '\0' || *strings[0] == '\0') {
            //printf("Found blank line\n");
            remove_from_array(original_pointer, strings);
        } else {
            strings++;
        }
    }

    strings = original_pointer;

}

void find_variables(char** strings){
    //Go through each line and if line has a tab to start then disc. line else see if it has an equals sign
    Variable *var[100] = {0};
    char ** orignal_str = strings;
    int i=0;
    while(*strings){
        //don't look at lines with \t in it
        char * subStr = strstr(*strings, "=");
        if(subStr!=NULL && *strings[0]!='\t'){
            char pre_str[subStr-*strings+1];
            strncpy(pre_str,*strings,subStr-*strings);
            pre_str[subStr-*strings] = '\0';
            char* strip_str = calloc(BUFSIZ*2,BUFSIZ*2);
            space_strip(pre_str,strip_str);
            char* strip_str_1 = calloc(BUFSIZ*2,BUFSIZ*2);
            space_strip((subStr+1),strip_str_1);
            variables[i] = calloc(sizeof(Variable), sizeof(Variable));
            variables[i]->name = strdup(strip_str);
            variables[i]->value = strdup(strip_str_1);
            remove_from_array(orignal_str,strings);
            i++;
        } else{
            strings++;
        }
    }
}

void replace_variable(char** strings){
    char** original_pointer = strings;
    while(*strings){
        char * org_str = *strings;
        while (**strings){
            if(**strings == '$' && *(*strings+1) == '('){
                char * subStr = strstr(*strings, ")");
                if(subStr == NULL)
                    continue;
                char * var_name = calloc(subStr-*strings-1,subStr-*strings-1);
                strncpy(var_name, &(*(*strings+2)),subStr-*strings-2);
                Variable **var = variables;
                bool found = false;
                char* str_rep = calloc(strlen(var_name)+4, strlen(var_name)+4);
                strcat(str_rep,"$(");
                strcat(str_rep,var_name);
                strcat(str_rep,")");
                while (*var){
                    char * value = strdup((*var)->value);
                    char * name = strdup((*var)->name);
                    if(strcmp(name, var_name)==0){
                        org_str = str_replace(org_str,str_rep,value);
                        found = true;
                        break;
                    }
                    var++;
                };
                if(!found && getenv(var_name)!=NULL){
                    org_str = str_replace(org_str,str_rep,getenv(var_name));
                    found = true;
                }else if(!found){
                    printf("Error: Unknown Variable %s", var_name);
                    org_str = str_replace(org_str,str_rep,"");
                }
            }
            (*strings)++;
        }
        *strings = org_str;
        strings++;
    }
    //replace the found variables
}

Target* phase(char** strings){
    return NULL;
    //Convert into struct type
}

Target* lexianate(char** strings){
    sanitize(strings);
    find_variables(strings);
    replace_variable(strings);

    while (*strings!=NULL){
        printf("%s\n",*strings); strings++;
    };

    return phase(strings);
}