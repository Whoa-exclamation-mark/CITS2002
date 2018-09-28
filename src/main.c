#include <stdio.h>
#include "lexicon/lexicon.h"
#include "phaser/phaser.h"
#include "runner/runner.h"
#include <stdlib.h>
#include "datatypes.h"
#include "helpers/helpers.h"


int main() {

    printf("test: %s\n",space_strip("  test  12   "));

    //need to figure out args and find the make file
    Target* targets = lexianate(get_file_string("/Users/campbell/CLionProjects/bake!/test/Bakefile"));
    Command* commands = getCommands(targets);
    run_commands(commands);
    exit(EXIT_SUCCESS);
}