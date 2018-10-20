#include <stdio.h>
#include "lexicon/lexicon.h"
#include "phaser/phaser.h"
#include "runner/runner.h"
#include <stdlib.h>
#include "datatypes.h"
#include "helpers/helpers.h"
#include "history/history.h"


int main() {
    //need to figure out args and find the make file
    lexianate(get_file_string("Bakefile"));
    setCommands("clean");
    //is_up_to_date_url("http://teaching.csse.uwa.edu.au/units/CITS2002/project/dependency-5m",NULL);
    //is_up_to_date("calculation.c",NULL);
    run_commands();

    printf("--- Build Success ---");
    exit(EXIT_SUCCESS);
}