/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "command.h"
#include "../logger/logger.h"
#include "../constants.h"

void exec_command(Command* command){
    //Are we running in print command only? If so then return straight away
    if (RUN_WO_EXEC) return;
    //Print out each command if the user has specified in an action line
    if(command->output_command == true){
        info("%s\n",command->command);
    }
    //Create a pipe for the output of the command
    int pfd[2];
    pipe(pfd);
    //Start the execution of the command
    switch (fork()){
        case -1:
            //We can't create a new process :( so exit as it is fatal
            error("ERROR: We have failed to create a process (possible resource overallocation).\n Please restart Bake! with more resources.");
            exit(EXIT_FAILURE);
        case 0:
            //Close the unused write end
            close(pfd[0]);
            //Send the stdout to the pipe
            dup2(pfd[1],STDOUT_FILENO);
            //Close the read end
            close(pfd[1]);
            //Start the process on the child
            execl(getenv("SHELL")!=NULL?getenv("SHELL"):"/bin/bash","bash","-c",command->command,NULL);
        default: {
            int status;
            //Wait for the process to finish
            wait( &status );
            //Close the write end of the pipe
            close(pfd[0]);
            //If the status of the child did not succeed then exit as we can't build
            int exit_status = WEXITSTATUS(status);
            //Determine if we should error out
            if(exit_status != EXIT_SUCCESS && !command->continue_on_error && !IGNORE_ERRORS){
                //Print that we have failed
                error("ERROR: Command has failed\n");
                error("--- Build Failure ---");
                //This is fatal so we should stop (we are only executing one master target so there is no point in continuing)
                exit(EXIT_FAILURE);
            }
        }
    }
}
