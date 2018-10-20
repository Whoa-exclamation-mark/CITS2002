//
// Created by Campbell Millar on 20/9/18.
//
#include "../datatypes.h"
#include "../constants.h"


#ifndef BAKE_PHASER_H
#define BAKE_PHASER_H

void setCommands(char*);

bool is_target(char*);

Target* find_target(char *);

extern Target* data[MAX_TARGETS];

#endif //BAKE_PHASER_H
