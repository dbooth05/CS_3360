#!/bin/bash

set -e

# compiles raytracer
echo "Compiling Raytracer"
make

# runs raytracer saving output to img.ppm 
./raytracer > img.ppm

# open img.ppm to view
gimp img.ppm > /dev/null

# if -r flag used removes image after exiting gimp
if [ "$1" == "-r" ]; then
    rm img.ppm
fi

# runs make clean if '-c' flag is used
if [ "$1" == "-c" ]; then
    make clean
fi

# clears terminal
clear