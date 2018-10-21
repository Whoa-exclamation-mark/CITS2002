# Bake!
## About
This is a CITS2002 project on making (or baking...) a project.

It is 69186414! - 114262411 times better than make!

## Building
### Constants
Be aware that there is `constants.h` in `src/` which contains hard limits for actions, targets, and variables. If you have any issues try to change these limits first.
### Compiling
This project uses CMake, make, and Bake! to build. Either call cmake, make, or bake in the source directory to build the project. There is also `clean` to remove any compiled object files and the executable.

## Bakefile
Bake! will try to find either Bakefile or bakefile in the directory in which you are calling from. You can also switch directory with the `-C` argument or specify a file with the `-f` argument. See below for more of the usage.

## Usage
```
USAGE: bake [options] target

OPTIONS:
        -C              Set working directory of Bake! (default to current one)
        -f              Specify the file that Bake! should be using (default to (B/b)akefile in current directory)
        -i              Ignore all errors from any actions
        -n              Do not run the actions only display the actions on screen (out = we would display command on screen, no error = continue on error)
        -p              Do not run the actions only display the phased file
        -s              Run Bake! in silent mode
        -h              Show this message again
```