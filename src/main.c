#include <stdio.h>
#include "lexicon/lexicon.h"
#include "phaser/phaser.h"
#include "runner/runner.h"
#include <stdlib.h>
#include "datatypes.h"
#include "helpers/helpers.h"


int main() {
    //need to figure out args and find the make file
    lexianate(get_file_string("/Users/campbell/CLionProjects/bake!/test/Bakefile"));
    Command* commands = getCommands("haversine");
    //run_commands(commands);
    exit(EXIT_SUCCESS);
}