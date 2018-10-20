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

//need to create tree and then order commands correctly

Target* data[MAX_TARGETS]={0};

bool is_target(char* name){
    Target** trg_point = data;
    while (*trg_point){
        if(strcmp((*trg_point)->name, name)==0)
            return true;
        trg_point++;
    }
    return false;
}

Target* find_target(char* name){
    if(!is_target(name)) return NULL;
    Target** trg_point = data;
    while (*trg_point){
        if(strcmp((*trg_point)->name, name)==0)
            return *trg_point;
        trg_point++;
    }
    return NULL;
}

void attach_dependencies(char* target){
    Target** trg_point = data;

    stack* depen_stack = create();

    push(depen_stack,target);

    while(!is_empty(depen_stack)){
        char* curr_target = pop(depen_stack);
        while (*trg_point){
            if(strcmp((*trg_point)->name, curr_target) == 0){
                char* token;
                char* phase_dependencies = strdup((*trg_point)->raw_dependencies);
                token = strtok(phase_dependencies, " ");

                while( token != NULL ) {
                    char* striped = calloc(strlen(token),strlen(token));
                    space_strip(token, striped);
                    if(is_target(striped)){
                        push_on_array((void **) (*trg_point)->dependencies, find_target(striped));
                    } else if(is_file_or_url(striped)){
                        push_on_array((void **) (*trg_point)->file_dependencies, striped);
                    } else{
                        error("ERROR: %s not a target or file!\n", striped);
                    }
                    push(depen_stack, striped);
                    token = strtok(NULL, " ");
                }
            }
            trg_point++;
        }
        trg_point = data;


    }
}

int attach_commands(char* target, stack* command_stack){
    int num_command=0;

    //Target** trg_point = data;

    stack* depen_stack = create();

    push(depen_stack,find_target(target));

    while(!is_empty(depen_stack)){

        Target* data_target = pop(depen_stack);

        char** raw_commands = data_target->raw_commands;

        while (*raw_commands){
            Command* command = calloc(sizeof(command), sizeof(command));

            char* modifiers = strndup(*raw_commands,3);
            modifiers[2] = '\0';

            int num_modifiers = 0;

            if(strchr(modifiers,'@') != NULL){
                command->output_command = false;
                num_modifiers++;
            } else {
                command->output_command = true;
            }
            if(strchr(modifiers,'-') != NULL){
                command->continue_on_error = true;
                num_modifiers++;
            } else{
                command->continue_on_error = false;
            }

            command->command = *(raw_commands)+num_modifiers;
            command->parent = data_target;



            push(command_stack, command);
            num_command++;
            raw_commands++;
        }

        Target** children = data_target->dependencies;

        while (*children){
            push(depen_stack,*children);
            children++;
        }

        if(RUN_INTERP){
            info("%s : ", data_target->name);
            Target** curr_children = data_target->dependencies;
            while (*curr_children){
                info("%s ",(*curr_children)->name);
                curr_children++;
            }
            char ** files = data_target->file_dependencies;
            while (*files){
                info(*files);
                files++;
            }
            info("\n");
            char** commands = data_target->raw_commands;
            while (*commands){
                info("\t%s\n",*commands);
                commands++;
            }
        }
    }
    return num_command;
}

void setCommands(char* target){

    if(target == NULL) target = data[0]->name;

    if(!is_target(target)){
        error("ERROR: Target %s is not a valid target", target);
        exit(EXIT_FAILURE);
    }

    attach_dependencies(target);

    //Target** target_data = data;



    stack* command_stack = create();

    int num_command = attach_commands(target, command_stack);

    if(RUN_INTERP){
        info("--- Interpreted Success ---");
        exit(EXIT_SUCCESS);
    }

    if(num_command > MAX_ACTIONS){
        error("ERROR: Number of actions exceed the hard limit of %d",MAX_ACTIONS);
        exit(EXIT_FAILURE);
    };

    while(!is_empty(command_stack)){
        Command* command = pop(command_stack);
        push_on_array((void **) commands, command);
    }

}