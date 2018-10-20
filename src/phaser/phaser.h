/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include "../datatypes.h"
#include "../constants.h"


#ifndef BAKE_PHASER_H
#define BAKE_PHASER_H

/**
 * Set the commands array with the relevant commands in order
 */
void setCommands(char*);

/**
 * Check if a string is a name of a target
 * @return true if it is
 */
bool is_target(char*);

/**
 * Find a target given it's name
 * @return the target pointer or null if it can't find the target
 */
Target* find_target(char *);

/**
 * Array of all the targets in the bakefile
 */
extern Target* data[MAX_TARGETS];

#endif //BAKE_PHASER_H
