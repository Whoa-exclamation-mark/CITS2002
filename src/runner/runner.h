//
// Created by Campbell Millar on 20/9/18.
//

#include "../datatypes.h"
#include "../constants.h"

#ifndef BAKE_RUNNER_H
#define BAKE_RUNNER_H

void run_commands();

void set_parents_update(Target*);

extern Command* commands[MAX_ACTIONS];

#endif //BAKE_RUNNER_H
