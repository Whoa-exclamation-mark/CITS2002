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

char *TARGET;

bool IGNORE_ERRORS = false;

bool RUN_WO_EXEC = false;

bool RUN_INTERP = false;

bool SILENT = false;

void usage(char option){
    info("OVERVIEW: %s %s\n",APP_NAME,VERSION);
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
    exit(option!=NULL?EXIT_FAILURE:EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    //need to figure out args and find the make file

    int  opt;
    char *file_name = "Bakefile";

    opterr	= 0;
    while((opt = getopt(argc, argv, OPTLIST)) != -1)   {
        switch(opt){
            case 'C':
                chdir(optarg);
                break;
            case 'f':
                file_name = strdup(optarg);
                break;
            case 'i':
                IGNORE_ERRORS = !IGNORE_ERRORS;
                break;
            case 'n':
                RUN_WO_EXEC = !RUN_WO_EXEC;
                break;
            case 'p':
                RUN_INTERP = !RUN_INTERP;
                break;
            case 's':
                SILENT = !SILENT;
                break;
            case 'h':
                usage(NULL);
                break;
            default:{
                usage(opt);
                break;
            }
        }
    }

    argc  -= optind;
    argv  += optind;
    TARGET = argv[argc-1];
    lexianate(get_file_string(file_name));
    setCommands(argc == 0 ? NULL : TARGET);

    run_commands();
    info("--- Build Success ---\n");
    exit(EXIT_SUCCESS);
}