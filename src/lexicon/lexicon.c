//
// Created by Campbell Millar on 20/9/18.
//

#include "lexicon.h"

char** sanitize(char** strings){
    //remove any issues from the string

    //Remove any blank lines

    //Move any lines that have \ in them onto one line


}

void find_variables(char** strings){
    //Go through each line and if line has a tab to start then disc. line else see if it has an equals sign

    //place inside var variables
}

char** replace_variable(char** strings){
    //replace the found variables
}

Target* phase(char** strings){

    //Convert into struct type
}

Target* lexianate(char** strings){
    strings = sanitize(strings);
    find_variables(strings);
    strings = replace_variable(strings);

    return phase(strings);
}