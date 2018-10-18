//
// Created by Campbell Millar on 20/9/18.
//

#include <string.h>
#include "lexicon.h"
#include <stdio.h>
#include <stdlib.h>
#include "../helpers/helpers.h"

char** sanitize(char** strings){
    //remove any issues from the string

    //find end of array

    char** original_pointer = strings;

    int length = 0;

    while (*strings++)
        length++;

    //strings--;
    //Remove any blank lines

    strings--;
    while((strings-- != original_pointer)){
        if(*strings[0]!='\0'&&*(*strings+strlen(*strings)-1)=='\\') {
            printf("Found wrapping line\n");
        }
        //strings++;
    }

    strings = original_pointer;

    while(*strings){
        //todo find why space strip not working
        char* strip_str = calloc(BUFSIZ,BUFSIZ);
        space_strip(*strings, strip_str);
        //printf("%s\n",strip_str);
        if(strip_str[0]=='#'){
            printf("Found comment line\n");
            *strings[0] = '\0';
        }
        if(*strings[0]=='\0'){
            printf("Found blank line\n");
            remove_from_array(original_pointer,strings);
        }else{
            strings++;
        }
        //free(strip_str);
    }
    //Move any lines that have \ in them onto one line

    strings = original_pointer;

    while (*strings!=NULL){
        printf("%s\n",*strings); strings++;
    };

}

void find_variables(char** strings){
    //Go through each line and if line has a tab to start then disc. line else see if it has an equals sign
    Variable var[100] = {{0}};

    int i=0;
    while(*strings++){
        //don't look at lines with \t in it
        if(*strings[0]!='\t'){
            char * subStr = strstr(*strings, "=");
            if(subStr!=NULL){
                char pre_str[subStr-*strings+1];
                strncpy(pre_str,subStr,subStr-*strings);
                pre_str[subStr-*strings] = '\0';
                //var[i].name = space_strip(pre_str);
                //var[i].value = space_strip(subStr);
                i++;
            }
        }
    }
    realloc(var,(i+1)* sizeof(Variable));
    variables = &var[0];
}

char** replace_variable(char** strings){
    char** original_pointer = strings;
    while(*strings++){
        char * org_str = *strings;
        while (**strings++){
            if(**strings == '$' && *strings[1] == '('){
                char * subStr = strstr(*strings, ")");
                if(subStr == NULL)
                    continue;
                char var_name[subStr-*strings-1];
                strncpy(var_name, *(strings+2),subStr-*strings-1);

                Variable *var = variables;
                while (var++){
                    if(strcmp(var->name, var_name)==0){
                        char* str_rep[strlen(var_name)+4];
                        strcat(str_rep,"$(");
                        strcat(str_rep,var_name );
                        strcat(str_rep,")");
                        *strings = str_replace(org_str,var_name,var->value);
                        break;
                    }
                    printf("Error: Unknown Variable %s", var_name);
                };
            }
        }

    }

    //replace the found variables
}

Target* phase(char** strings){

    //Convert into struct type
}

Target* lexianate(char** strings){
    strings = sanitize(strings);
    find_variables(strings);
    strings = replace_variable(strings);

    return phase(strings);
}