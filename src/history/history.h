//
// Created by Campbell Millar on 21/9/18.
//

#ifndef BAKE_HISTORY_H
#define BAKE_HISTORY_H

#include <stdbool.h>
#include "../datatypes.h"

bool is_url(char *);

bool is_up_to_date(char*,Target);

bool is_up_to_date_url(char*);

#endif //BAKE_HISTORY_H
