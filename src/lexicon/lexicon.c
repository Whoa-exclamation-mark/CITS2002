//
// Created by Campbell Millar on 20/9/18.
//

#include <string.h>
#include "lexicon.h"
#include <stdio.h>
#include <stdlib.h>
#include "../helpers/helpers.h"
#include <unistd.h>
#include <limits.h>
#include "../phaser/phaser.h"

Variable* variables[100] = {0};

void sanitize(char** strings){

    char** original_pointer = strings;

    int length = 0;

    while (*strings) {
        length++;
        strings++;
    }

    strings--;
    while((strings != original_pointer)){
        if(*strings!= NULL && *strings[0]!='\0' && *(*strings+strlen(*strings)-1)=='\\') {
            *(*strings+strlen(*strings)-1)='\0';
            char * data = calloc(BUFSIZ*2,BUFSIZ*2);
            space_strip(*(strings+1),*(strings+1));
            sprintf(data,"%s%s",*strings,*(strings+1));
            *strings = data;
            remove_from_array(original_pointer,strings+1);
        }
        strings--;
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
        //todo figure out what to do with action lines that have things like echo 1+1=2
        char * org_str = *strings;
        while (**strings){
            if(**strings == '$' && *(*strings+1) == '('){
                char* value;
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
                    char * name = strdup((*var)->name);
                    if(strcmp(name, var_name)==0){
                        value = strdup((*var)->value);
                        found = true;
                        break;
                    }
                    var++;
                };
                if(strcmp(var_name,"PID") == 0){
                    char buffer[sizeof(int) * 4 + 1];
                    sprintf(buffer, "%d", getpid());
                    value = strdup(buffer);
                    found = true;
                }
                else if(strcmp(var_name,"PPID") == 0){
                    char buffer[sizeof(int) * 4 + 1];
                    sprintf(buffer, "%d", getppid());
                    value = strdup(buffer);
                    found = true;
                }
                else if(strcmp(var_name,"PWD") == 0){
                    char cwd[PATH_MAX];
                    getcwd(cwd,PATH_MAX);
                    value = strdup(cwd);
                    found = true;
                }
                else if(strcmp(var_name,"RAND") == 0){
                    char buffer[sizeof(int) * 4 + 1];
                    sprintf(buffer, "%d", rand());
                    value = strdup(buffer);
                    found = true;
                }
                else if(!found && getenv(var_name)!=NULL){
                    value = getenv(var_name);
                    found = true;
                }else if(!found){
                    printf("Error: Unknown Variable %s", var_name);
                    value = "";
                }
                org_str = str_replace(org_str,str_rep,value);
            }
            (*strings)++;
        }
        *strings = org_str;
        strings++;
    }
    //replace the found variables
}

void phase(char** strings){
    //Target* data[100]={0};
    int i=0;
    while (*strings){
        if(*strings[0]!='\t'){
            char* dependencies = strchr(*strings,':');
            if(dependencies!=NULL){
                data[i] = calloc(sizeof(Target), sizeof(Target));
                //todo space strip
                char* char_raw = strndup(*strings,dependencies-*strings);
                char* char_data = calloc(sizeof(char_raw), sizeof(char_raw));
                space_strip(char_raw,char_data);
                data[i]->name = char_data;

                char_raw = strdup(dependencies+1);
                char_data = calloc(sizeof(char_raw), sizeof(char_raw));
                space_strip(char_raw,char_data);
                data[i]->raw_dependencies = char_data;

                char** sub_point = strings;

                while (*sub_point[0]!='\t'){
                    sub_point++;
                }

                int num_command = (int) (sub_point - strings);

                data[i]->raw_commands = calloc(num_command+1,num_command+1);

                //we are assuming that all targets have at least 1 command
                strings++;
                for (int j = 0; j < num_command; ++j) {
                    char *strip_str = calloc(BUFSIZ * 2, BUFSIZ * 2);
                    space_strip(*(strings+j), strip_str);
                    data[i]->raw_commands[j] = strip_str;
                }

                i++;
            }
        };
        strings++;
    }

    //return data;
    //Convert into struct type
}

void lexianate(char** strings){
    sanitize(strings);
    find_variables(strings);
    replace_variable(strings);
    //todo get this to be operated by a command line arg


    phase(strings);

    //printf("%s-%s\n", data[0]->name, data[0]->raw_dependencies);

    while (*strings!=NULL){
        printf("%s\n",*strings); strings++;
    };
}