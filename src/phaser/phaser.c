//
// Created by Campbell Millar on 20/9/18.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "phaser.h"
#include "../adt/stack.h"
#include "../helpers/helpers.h"
#include "../history/history.h"
#include "../runner/runner.h"

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
                    //printf("%s \n", striped);
                    if(is_target(striped)){
                        push_on_array((void **) (*trg_point)->dependencies, find_target(striped));
                    } else if(is_file_or_url(striped)){
                        push_on_array((void **) (*trg_point)->file_dependencies, striped);
                    } else{
                        printf("ERROR: %s not a target or file!\n", striped);
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
            //printf("%s\n",*raw_commands);
            Command* command = calloc(sizeof(command), sizeof(command));
            //todo check the @ and other junk

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

    }
    return num_command;
}

void setCommands(char* target){

    if(!is_target(target)){
        printf("ERROR: Target %s is not a valid target", target);
        exit(EXIT_FAILURE);
    }

    attach_dependencies(target);

    stack* command_stack = create();

    int num_command = attach_commands(target, command_stack);



    if(num_command > MAX_ACTIONS){
        printf("ERROR: Number of actions exceed the hard limit of %d",MAX_ACTIONS);
        exit(EXIT_FAILURE);
    };

    while(!is_empty(command_stack)){
        Command* command = pop(command_stack);
        push_on_array((void **) commands, command);
    }

}