/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include "../datatypes.h"
#include "../constants.h"


#ifndef BAKE_PHASER_H
#define BAKE_PHASER_H

void setCommands(char*);

bool is_target(char*);

Target* find_target(char *);

extern Target* data[MAX_TARGETS];

#endif //BAKE_PHASER_H
