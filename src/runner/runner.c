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

//Array to store the command data
Command* commands[MAX_ACTIONS]={0};

void set_parents_update(Target * target){
    //Get the target array
    Target** targets = data;
    //Iterate through array
    while (*targets){
        //Get the children of the given target
        Target ** children = (*targets)->dependencies;
        //Iterate through the children
        while (*children){
            //Check if one of the children is the target
            if(*children == target){
                //If so set this target to update
                (*targets)->rebuild = true;
            }
            //Move onto the next child
            children++;
        }
        //Move onto the next target
        targets++;
    }
}

void run_commands(){
    //Get the commands pointer
    Command** command_index = commands;
    //Iterate through the commands array
    while (*command_index){
        //Get the file dependencies
        char ** files = (*command_index)->parent->file_dependencies;
        //Get if this has no file dependencies (i.e. clean)
        bool should_run_command = *files == NULL;
        //Get the command parent
        Target* target = (*command_index)->parent;
        //Iterate over the file dependencies
        while (*files){
            //Check if we should run the command by ensuring it is not up to date,
            //  we have access to it,
            //  we have previously needed to update,
            //  or we need a rebuild because of a child
            should_run_command = is_up_to_date(*files,target)
                    || access(target->name, F_OK)==-1
                    || should_run_command
                    || target->rebuild;
            //Move onto the next file
            files++;
        }
        //Se the command to if we should run
        (*command_index)->should_run = should_run_command;
        //If we need to run then we need to update the parents
        if(should_run_command){
            //Set the parents to rebuild
            set_parents_update(target);
        }
        //Move onto the next command
        command_index++;
    }
    //Reset the command index
    command_index = commands;
    //Iterate back over the command array
    while (*command_index){
        //Get the current command
        Command* command = *command_index;
        //See if we need to run it and if we are in no execute mode
        if(command->should_run && RUN_WO_EXEC)
            //Print out the command
            info("%s \033[1mtags:%s%s\033[0m \n",command->command, command->continue_on_error?"(no error)":"", command->output_command?"(out)":"");
        //Move to next command
        command_index++;
    }
    //Reset the command index
    command_index = commands;
    //Now iterate over the commands array again
    while (*command_index){
        //Execute the command if we need to run it
        if((*command_index)->should_run) exec_command(*command_index);
        //Move to next command
        command_index++;
    }
}
