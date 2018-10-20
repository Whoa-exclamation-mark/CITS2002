/* CITS2002 Project 2018
   Name(s):		Campbell J.H. Millar
   Student number(s):	22510848
 */

#include "../datatypes.h"
#include "../constants.h"

#ifndef BAKE_RUNNER_H
#define BAKE_RUNNER_H

void run_commands();

void set_parents_update(Target*);

extern Command* commands[MAX_ACTIONS];

#endif //BAKE_RUNNER_H
