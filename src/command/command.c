//
// Created by Campbell Millar on 28/9/18.
//

#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void exec_command(Command* command){
    //Print out command if we are wanting to
    if(command->output_command == true){
        printf("%s\n",command->command);
    }
    //Start the execution of the command
    switch (fork()){
        case -1:
            printf("We have failed to create a process (possible resource overallocation).\n Please restart Bake! with more resources.");
            exit(EXIT_FAILURE);
        case 0:
            //Start the process on the child
            execl(getenv("SHELL")!=NULL?getenv("SHELL"):"/bin/bash","bash","-c",command->command,NULL);
        default: {
            int status;
            //Wait for the process to finish
            wait( &status );
            //If the status of the child did not succeed then exit as we can't build
            int exit_status = WEXITSTATUS(status);

            if(exit_status != EXIT_SUCCESS && !command->continue_on_error){
                printf("ERROR: Command has failed\n");
                printf("--- Build Failure ---");
                exit(EXIT_FAILURE);
                //command.parent->has_failed=true;
            }
        }
    }
}