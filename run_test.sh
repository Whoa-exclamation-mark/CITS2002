#!/usr/bin/env bash

# Ensure that we have bake
echo "Making project"
make
# Run bake on our project (ensuring that nothing changes)
echo "Running on build project"
./bake
# Run on the blank project
echo "Running on build project (blank)"
rm -rf *.o
./bake
# Now run bake on haversine
echo "Running on haversine"
./bake -C ./test/haversine/
# Clean haversine
echo "Cleaning on haversine"
./bake -C ./test/haversine/ clean
# Now run bake on hashtable
echo "Running on hashtable"
./bake -C ./test/hashtable/
# Clean hashtable
echo "Cleaning on hashtable"
./bake -C ./test/hashtable/ clean
# Clean bake
echo "Cleaning"
./bake clean
# End
