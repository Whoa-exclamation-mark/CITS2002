/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexicon.h"
#include "../helpers/helpers.h"
#include "../phaser/phaser.h"
#include "../constants.h"
#include "../logger/logger.h"

//Defining variables array (nulling it all)
Variable* variables[MAX_VAR] = {0};

bool is_target_type(char * string){
    //Get if it has a : in it
    char* dependencies = strchr(string,':');
    //Make sure that dependencies exists (has a :) and first character is not a \t
    return dependencies!=NULL && string[0]!='\t';
}

void sanitize(char** strings){
    //Assign a variable as the starting pointer
    char** original_pointer = strings;
    //Length of the string array
    int length = 0;
    //Get the length of the array
    while (*strings) {
        length++;
        strings++;
    }
    //Move back one on the array
    strings--;
    //Go backwards through the array until the starting pointer
    //This is for the multi-line actions
    while((strings != original_pointer)){
        //Check if the string is not null/blank and the last last character is a \
        //
        if(*strings!= NULL && *strings[0]!='\0' && *(*strings+strlen(*strings)-1)=='\\') {
            //Remove the \
            //
            *(*strings+strlen(*strings)-1)='\0';
            //Create a buffer for the two lines together
            char * data = my_calloc(BUFSIZ*2);
            //Strip any spaces on the next line
            space_strip(*(strings+1),*(strings+1));
            //Combine lines together
            sprintf(data,"%s%s",*strings,*(strings+1));
            //Make the new buffer the data for the line
            *strings = data;
            //Remove the the next line from the array
            remove_from_array((void **) original_pointer, (void **) (strings + 1));
        }
        //Move down the array
        strings--;
    }
    //Reset the string pointer
    strings = original_pointer;

    //Find any comment and blank lines and remove
    while(*strings) {
        //Make a new buffer for space strip (needs to be double BUFSIZ because it could be an combined line)
        char *strip_str = my_calloc(BUFSIZ * 2);
        //Strip the spaces
        space_strip(*strings, strip_str);
        //Check if the line has a # at the start
        if (strip_str[0] == '#') {
            //Null it out
            *strings[0] = '\0';
        }
        //Check if we have a blank line
        if (strip_str[0] == '\0' || *strings[0] == '\0') {
            //Remove the line from the array
            remove_from_array((void **) original_pointer, (void **) strings);
        } else {
            //Move if we didn't have a blank line
            strings++;
        }
    }
}

void find_variables(char** strings){
    //Go through each line and if line has a tab to start then disc. line else see if it has an equals sign
    //Assign a variable as the starting pointer
    char ** orignal_str = strings;
    //Index for the variables
    int i=0;
    while(*strings){
        //Find a = in the string
        char * subStr = strstr(*strings, "=");
        //don't look at lines with \t in it and it has a = in it
        if(subStr!=NULL && *strings[0]!='\t'){
            //String before the =
            char pre_str[subStr-*strings+1];
            //Copy out the string
            strncpy(pre_str,*strings,subStr-*strings);
            //Terminate the string
            pre_str[subStr-*strings] = '\0';
            //Strip the string buffer
            char* strip_str = my_calloc(BUFSIZ*2);
            //Strip the string
            space_strip(pre_str,strip_str);
            //Strip after the = buffer
            char* strip_str_1 = my_calloc(BUFSIZ*2);
            //Strip after the =
            space_strip((subStr+1),strip_str_1);
            //Null and allocate the variables in the array
            variables[i] = my_calloc(sizeof(Variable));
            //Copy the var name
            variables[i]->name = strdup(strip_str);
            //Copy the var value
            variables[i]->value = strdup(strip_str_1);
            //Remove the line from the string array
            remove_from_array((void **) orignal_str, (void **) strings);
            //Increment var index
            i++;
        } else{
            //Increment the string array
            strings++;
        }
    }
}

void replace_variable(char** strings){
    //Walk through string array
    while(*strings){
        //Make sure that we have the original string pointer
        char * org_str = *strings;
        //Walk through each string
        while (**strings){
            //Find a $(
            if(**strings == '$' && *(*strings+1) == '('){
                //Declare a value for replacement
                char* value;
                //Find end of of $(NAME)
                char * subStr = strstr(*strings, ")");
                //If we can't find an end then we don't need to care about this
                if(subStr == NULL)
                    continue;
                //Allocate space for the finding of the var name
                char * var_name = my_calloc(subStr-*strings-1);
                //Copy the var name
                strncpy(var_name, &(*(*strings+2)),subStr-*strings-2);
                //Define the start of the variables array
                Variable **var = variables;
                //Say that we haven't found the variable yet
                bool found = false;
                //The replacement string
                char* str_rep = my_calloc(strlen(var_name)+4);
                //Build the string
                strcat(str_rep,"$(");
                strcat(str_rep,var_name);
                strcat(str_rep,")");
                //Go to end of var array
                while (*var) var++;
                //Go back one (as we are at the null byte)
                var--;
                //Iterate through the var array
                while (*var){
                    //Get the name of the variable
                    char * name = strdup((*var)->name);
                    //Compare with the built up name
                    if(strcmp(name, var_name)==0){
                        //We have a match to set the value and say that we have found it
                        value = strdup((*var)->value);
                        found = true;
                        //End the loop
                        break;
                    }
                    //Decrement the var pointer
                    var--;
                };
                //Special variables
                if(strcmp(var_name,"PID") == 0){
                    //Make a buffer for the pid
                    char buffer[sizeof(int) * 4 + 1];
                    //Move the pid into the buffer
                    sprintf(buffer, "%d", getpid());
                    //Copy the buffer into the value
                    value = strdup(buffer);
                    //Set we have found the var
                    found = true;
                }
                else if(strcmp(var_name,"PPID") == 0){
                    //Make a buffer for the ppid
                    char buffer[sizeof(int) * 4 + 1];
                    //Move the ppid into the buffer
                    sprintf(buffer, "%d", getppid());
                    //Copy the buffer into the value
                    value = strdup(buffer);
                    //Set we have found the var
                    found = true;
                }
                else if(strcmp(var_name,"PWD") == 0){
                    //Make a buffer for the cwd
                    char cwd[PATH_MAX];
                    //Move the cwd into the buffer
                    getcwd(cwd,PATH_MAX);
                    //Copy the buffer into the value
                    value = strdup(cwd);
                    //Set we have found the var
                    found = true;
                }
                else if(strcmp(var_name,"RAND") == 0){
                    //Make a buffer for the rand
                    char buffer[sizeof(int) * 4 + 1];
                    //Move the rand into the buffer
                    sprintf(buffer, "%d", rand());
                    //Copy the buffer into the value
                    value = strdup(buffer);
                    //Set we have found the var
                    found = true;
                }
                else if(!found && getenv(var_name)!=NULL){
                    //Check if we have something in the environment variables
                    value = getenv(var_name);
                    found = true;
                }else if(!found){
                    //We haven't found any var :(
                    error("\033[31mERROR: Unknown Variable %s\033[0m", var_name);
                    value = "";
                }
                //Replace the value and the $(NAME)
                org_str = str_replace(org_str,str_rep,value);
            }
            //Move onto the next char
            (*strings)++;
        }
        //Reset the string
        *strings = org_str;
        //Move onto the next string
        strings++;
    }
}

void phase(char** strings){
    //Target index
    int i=0;
    //Iterate through strings
    while (*strings){
        //Check if the string is not starting with a \t
        int num_command = 0;
        if(*strings[0]!='\t'){
            //Find a : in the string
            char* dependencies = strchr(*strings,':');
            //Check if we have found something
            if(dependencies!=NULL){
                //Assign space to the target
                data[i] = my_calloc(sizeof(Target));
                //Get the name of the target
                char* char_raw = strndup(*strings,dependencies-*strings);
                //Assign space for space strip
                char* char_data = my_calloc(strlen(char_raw));
                //Space strip name
                space_strip(char_raw,char_data);
                //Assign the name pointer to data
                data[i]->name = char_data;

                //Get the dependencies
                char_raw = strdup(dependencies+1);
                //Allocate spaces for space strip
                char_data = my_calloc(strlen(char_raw));
                //Space strip dependencies
                space_strip(char_raw,char_data);
                //Assign the pointer
                data[i]->raw_dependencies = char_data;

                //Get raw commands (assign a base location for strings)
                char** sub_point = strings+1;

                //Check if we don't have a \t as the fist character
                while (*sub_point!=NULL&&*sub_point[0]=='\t'&&!is_target_type(*sub_point)){
                    //Increment pointer
                    sub_point++;
                }

                //Number of commands present
                num_command = (int) (sub_point - strings-1);

                //Assign space for the array
                data[i]->raw_commands = my_calloc((size_t) sizeof(char *)*(num_command + 1));

                //Increment strings as it is pointing one far back
                strings++;

                //Check if we are not on a target line (bug with 0 commands)
                if(!is_target_type(*strings)) {
                    //Iterate through each command
                    int j = 0;
                    for (; j < num_command; ++j) {
                        //Assign space for the space strip n*2 -> wrapping lines
                        char *strip_str = my_calloc(BUFSIZ * 2);
                        //Space strip the line
                        space_strip(*(strings + j), strip_str);
                        //Add it into the raw commands
                        data[i]->raw_commands[j] = strip_str;
                    }
                    data[i]->raw_commands[j] = NULL;
                }

                //Allocate space for dependencies
                data[i]->dependencies = my_calloc(MAX_TARGETS* sizeof(char*));

                //Set the rebuildity to be false
                data[i]->rebuild = false;

                //Allocate space for file dependencies
                data[i]->file_dependencies = my_calloc(BUFSIZ*2);

                //Increment index by 1
                i++;
            }
        };
        //Move the pointer by number of commands
        strings+= num_command;
    }
}

void lexianate(char** strings){
    //Run sanitize to remove issues in the string
    sanitize(strings);
    //Find the variables
    find_variables(strings);
    //Replace the variables
    replace_variable(strings);
    //Phase the string into a struct
    phase(strings);
}
