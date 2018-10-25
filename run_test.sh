#!/usr/bin/env bash

# Ensure that we have bake
make
# Run bake on our project
./bake
# Now run bake on haversine
./bake -C ./test/haversine/
# Clean haversine
./bake -C ./test/haversine/ clean
# Now run bake on hashtable
./bake -C ./test/hashtable/
# Clean hashtable
./bake -C ./test/hashtable/ clean
# Clean bake
./bake clean
# End
