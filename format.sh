#!/bin/bash
# C/C++ formatting

clang-format -i main.cpp planets.h planets.cpp # format files in place

if [[ "$1" == "override-default-config" ]]; then 
    clang-format --style=LLVM --dump-config > .clang-format   # create a config to edit
fi