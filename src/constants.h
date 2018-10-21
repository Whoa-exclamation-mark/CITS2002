/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#ifndef BAKE_CONSTANTS_H
#define BAKE_CONSTANTS_H

/**
 * This is the maximum amount of variables that Bake! can use - change to be larger if needed
 */
#define MAX_VAR 100

/**
 * This is the maximum amount of targets that Bake! can use - change to be larger if needed
 */
#define MAX_TARGETS 100

/**
 * This is the maximum amount of actions that Bake! can use - change to be larger if needed
 */
#define MAX_ACTIONS 100

/**
 * This is the app name of Bake!
 */
#define APP_NAME "Bake!"
/**
 * This is the version number (a - alpha, b - beta, rc - release candidate, r - release)
 */
#define VERSION "1.0.0rc"

/**
 * This is the target that was specified in the evocation of the command
 */
extern char* TARGET;

/**
 * This is a flag to ignore all errors specified in the evocation of the command
 */
extern bool IGNORE_ERRORS;

/**
 * This is a flag to run Bake! but not to execute any commands (only print) specified in the evocation of the command
 */
extern bool RUN_WO_EXEC;

/**
 * This is a flag to run Bake! in interp mode (only print out what Bake! has interpreted) specified in the evocation of the command
 */
extern bool RUN_INTERP;

/**
 * This is a flag to run in silent mode specified in the evocation of the command
 */
extern bool SILENT;

#endif //BAKE_CONSTANTS_H
