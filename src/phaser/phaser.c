/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "phaser.h"
#include "../adt/stack.h"
#include "../helpers/helpers.h"
#include "../history/history.h"
#include "../runner/runner.h"
#include "../logger/logger.h"

//Array to store the target data
Target* data[MAX_TARGETS]={0};

bool is_target(char* name){
    //Make a start reference pointer to data
    Target** trg_point = data;
    //Iterate through data
    while (*trg_point){
        //Compare if the target name is the name
        if(strcmp((*trg_point)->name, name)==0)
            //Return true if yes
            return true;
        //Else keep going
        trg_point++;
    }
    //Return false if no matches was found
    return false;
}

Target* find_target(char* name){
    //If the target name is null then return null (we can't find null!)
    if(!is_target(name)) return NULL;
    //Make a start reference pointer to data
    Target** trg_point = data;
    //Iterate through data
    while (*trg_point){
        //Compare if the target name is the name
        if(strcmp((*trg_point)->name, name)==0)
            //Return the pointer if yes
            return *trg_point;
        //Else keep going
        trg_point++;
    }
    //Return null if nothing was found
    return NULL;
}

void attach_dependencies(char* target){
    //Make a start reference pointer to data
    Target** trg_point = data;
    //Create a stack to navigate through
    stack* depen_stack = create();
    //Push the start target on the stack
    push(depen_stack,target);
    //Iterate over stack until we get empty
    while(!is_empty(depen_stack)){
        //Pop top element
        char* curr_target = pop(depen_stack);
        //Iterate over the target data array
        while (*trg_point){
            //Compare the name to the current poped element
            if(strcmp((*trg_point)->name, curr_target) == 0){
                //Define a token for breaking down the string into space separations
                char* token;
                //Copy the raw dependencies to not modify the string
                char* phase_dependencies = strdup((*trg_point)->raw_dependencies);
                //Get first token
                token = strtok(phase_dependencies, " ");
                //Iterate until there is no tokens
                while( token != NULL ) {
                    //Setup for the token to be space striped
                    char* striped = my_calloc(strlen(token));
                    //Space strip the token
                    space_strip(token, striped);
                    //Check if it is a target
                    if(is_target(striped)){
                        //Attach the target as a child
                        push_on_array((void **) (*trg_point)->dependencies, find_target(striped));
                    } else if(is_file_or_url(striped)){
                        //Else check if it is a url/file and push on the file dependencies array instead
                        push_on_array((void **) (*trg_point)->file_dependencies, striped);
                    } else{
                        //Or we didn't find anything :( - just print the error
                        error("\033[31mERROR: %s not a target or file!\n\033[0m", striped);
                    }
                    //Push the dependency onto the stack
                    push(depen_stack, striped);
                    //Get the next token
                    token = strtok(NULL, " ");
                }
                //Free temp var
                free(phase_dependencies);
            }
            //Increment the pointer to the target data
            trg_point++;
        }
        //Reset the target data pointer
        trg_point = data;
    }
    //Free the stack (it should be empty so no need to go over the stack)
    free(depen_stack);
}

int attach_commands(char* target, stack* command_stack){
    //Set num command to be 0
    int num_command=0;
    //Create the dependency stack to do a depth first search
    stack* depen_stack = create();
    //Push the first target on
    push(depen_stack,find_target(target));
    //Iterate until the stack is empty
    while(!is_empty(depen_stack)){
        //Pop the top target off
        Target* data_target = pop(depen_stack);
        //Get the raw commands of the target
        char** raw_commands = data_target->raw_commands;
        //Now iterate over the raw commands
        while (*raw_commands){
            //Allocate memory for the command
            Command* command = my_calloc(sizeof(command));
            //Now see if we have any modifiers - we only care about the first 2 characters - so only get the first 3 to
            //  terminate it
            char* modifiers = strndup(*raw_commands,3);
            //Null terminate the string
            modifiers[2] = '\0';
            //Track the number of modifiers
            int num_modifiers = 0;
            //See if we have @ in our string
            if(strchr(modifiers,'@') != NULL){
                //Set the output to be silent
                command->output_command = false;
                //Since we have found a modifier we need to increment num_modifiers
                num_modifiers++;
            } else {
                //We haven't found an @
                command->output_command = true;
            }
            //See if we have - in our string
            if(strchr(modifiers,'-') != NULL){
                //Set the command to continue on error
                command->continue_on_error = true;
                //Since we have found a modifier we need to increment num_modifiers
                num_modifiers++;
            } else{
                //We haven't found an -
                command->continue_on_error = false;
            }
            //Free temp var
            free(modifiers);

            //Now we need to copy the command into the command struct (without the modifiers)
            command->command = *(raw_commands)+num_modifiers;
            //Set the parent so we can track who own what
            command->parent = data_target;
            //Push the command onto the stack
            push(command_stack, command);
            //Now increase the number of commands we have
            num_command++;
            //Go to the next raw command
            raw_commands++;
        }
        //Find any children of the target
        Target** children = data_target->dependencies;
        //Iterate over the children
        while (*children){
            //Push on the stack
            push(depen_stack,*children);
            //Increment the children array
            children++;
        }
        //Check if we are running in interp mode only
        if(RUN_INTERP){
            //Print the name of the target
            info("%s : ", data_target->name);
            //Get the children of the target
            Target** curr_children = data_target->dependencies;
            //Iterate over the children
            while (*curr_children){
                //Print the children names
                info("%s ",(*curr_children)->name);
                //Move to the next child
                curr_children++;
            }
            //Get the file dependencies
            char ** files = data_target->file_dependencies;
            //Iterate over the file dependencies
            while (*files){
                //Print the file dependencies
                info("%s ",*files);
                //Move to next dependency
                files++;
            }
            //Make a new line
            info("\n");
            //Get the commands
            char** commands = data_target->raw_commands;
            //Iterate over the commands
            while (*commands){
                //Print each command with a tab
                info("\t%s\n",*commands);
                //Move to the next command
                commands++;
            }
        }
    }
    //Free empty stack
    free(depen_stack);
    //Return the number of commands we found
    return num_command;
}

void setCommands(char* target){

    //Check to see if the target is null. If it is then we set the default top target
    if(target == NULL) target = data[0]->name;

    //Check if the specified target is valid
    if(!is_target(target)){
        //Print if not
        error("\033[31mERROR: Target %s is not a valid target\033[0m", target);
        //Exit as this error is fatal
        exit(EXIT_FAILURE);
    }
    //Attach the dependencies to the targets
    attach_dependencies(target);
    //Create a command stack (we need to flip the order - easiest way - should have used a queue...)
    stack* command_stack = create();
    //Attach the commands to the targets and get the number of commands
    int num_command = attach_commands(target, command_stack);
    //Are we running in interp mode?
    if(RUN_INTERP){
        //Print if we have seceded
        info("\033[32m--- Interpreter Success ---\033[0m");
        //Exit
        exit(EXIT_SUCCESS);
    }
    //Check we haven't gone over the max actions
    if(num_command > MAX_ACTIONS){
        //Print if we have
        error("\033[31mERROR: Number of actions exceed the hard limit of %d\033[0m",MAX_ACTIONS);
        //Exit as this is fatal
        exit(EXIT_FAILURE);
    };

    //Flip the stack with another stack :p - cont. until there is nothing on the original stack
    while(!is_empty(command_stack)){
        //Pop the command
        Command* command = pop(command_stack);
        //Push onto the commands array
        push_on_array((void **) commands, command);
    }
    //Free empty stack
    free(command_stack);
}
