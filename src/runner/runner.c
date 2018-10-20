/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdio.h>
#include <unistd.h>
#include "runner.h"
#include "../history/history.h"
#include "../phaser/phaser.h"
#include "../command/command.h"
#include "../logger/logger.h"

//Manages the exec of the commands (not running itself)

Command* commands[MAX_ACTIONS]={0};

void set_parents_update(Target * target){
    Target** targets = data;

    while (*targets){
        Target ** children = (*targets)->dependencies;
        while (*children){
            if(*children == target){
                (*targets)->rebuild = true;
            }
            children++;
        }
        targets++;
    }

}

void run_commands(){
    Command** command_index = commands;

    while (*command_index){
        char ** files = (*command_index)->parent->file_dependencies;

        bool should_run_command = *files == NULL;


        Target* target = (*command_index)->parent;

        while (*files){
            should_run_command = is_up_to_date(*files,target)
                    || access(target->name, F_OK)==-1
                    || should_run_command
                    || target->rebuild;
            files++;
        }

        (*command_index)->should_run = should_run_command;

        if(should_run_command){
            set_parents_update(target);
        }

        command_index++;
    }

    command_index = commands;
    //todo make this a arg option
    while (*command_index){
        Command* command = *command_index;
        if(command->should_run && RUN_WO_EXEC)
            info("%s%s%s\n",command->command, command->continue_on_error?" (no error)":"", command->output_command?" (out)":"");
        command_index++;
    }

    command_index = commands;

    while (*command_index){
        if((*command_index)->should_run) exec_command(*command_index);
        command_index++;
    }
}