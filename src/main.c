/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "lexicon/lexicon.h"
#include "phaser/phaser.h"
#include "runner/runner.h"
#include "helpers/helpers.h"
#include "constants.h"
#include "logger/logger.h"

#define	OPTLIST "C:f:inpsh"

//To declare TARGET constant
char *TARGET;
//To declare IGNORE_ERRORS constant
bool IGNORE_ERRORS = false;
//To declare RUN_WO_EXEC constant
bool RUN_WO_EXEC = false;
//To declare RUN_INTERP constant
bool RUN_INTERP = false;
//To declare SILENT constant
bool SILENT = false;

void usage(char option){
    //Print out usage infomation
    info("OVERVIEW: %s %s\n",APP_NAME,VERSION);
    //If this was because of a unknown option then print that they don't know what they are doing
    if(option!=NULL) error("ERROR: Unknown option %c",option);
    info("\nUSAGE: bake [options] target\n\n");
    info("OPTIONS:\n"
           "\t-C\t\tSet working directory of Bake! (default to current one)\n"
           "\t-f\t\tSpecify the file that Bake! should be using (default to (B/b)akefile in current directory)\n"
           "\t-i\t\tIgnore all errors from any actions\n"
           "\t-n\t\tDo not run the actions only display the actions on screen (out = we would display command on screen, no error = continue on error)\n"
           "\t-p\t\tDo not run the actions only display the phased file\n"
           "\t-s\t\tRun Bake! in silent mode\n"
           "\t-h\t\tShow this message again\n");
    //If they don't know what they where doing then it is a fatal error but if it was just usage info then not fatal
    exit(option!=NULL?EXIT_FAILURE:EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    //Define the option flags
    int  opt;
    //Define a default file name
    char *file_name = "Bakefile";
    //Define the opterr variable for getopt
    opterr	= 0;
    while((opt = getopt(argc, argv, OPTLIST)) != -1){
        //Switch on the opt
        switch(opt){
            //If it is a C then change directory
            case 'C':
                chdir(optarg);
                break;
            //If it is a f then set the file name
            case 'f':
                file_name = strdup(optarg);
                break;
            //If it is a i then toggle the ignore error flag
            case 'i':
                IGNORE_ERRORS = !IGNORE_ERRORS;
                break;
            //If it is a n then toggle the RUN_WO_EXEC flag
            case 'n':
                RUN_WO_EXEC = !RUN_WO_EXEC;
                break;
            //If it is a p then toggle the RUN_INTERP flag
            case 'p':
                RUN_INTERP = !RUN_INTERP;
                break;
            //If it is s then toggle the SILENT flag
            case 's':
                SILENT = !SILENT;
                break;
            //If it is h then print help usage
            case 'h':
                usage(NULL);
                break;
            //Else they are a numpty and we need to print usage
            default:{
                usage(opt);
                break;
            }
        }
    }

    //Change the # of arguments to the amount less than the ones that we just phased
    argc  -= optind;
    //Move the option array to the # that we just phased
    argv  += optind;
    //Set the target variable to the last item on argv
    TARGET = argv[argc-1];
    //Start lexianating the file
    lexianate(get_file_string(file_name));
    //Start the phasing but send through null as the target if argc (we don't want to use the path)
    setCommands(argc == 0 ? NULL : TARGET);
    //Run the commands
    run_commands();
    //We have finished all good!
    info("--- Build Success ---\n");
    //Exit with success!
    exit(EXIT_SUCCESS);
}