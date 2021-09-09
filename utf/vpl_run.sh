#!/bin/bash

. common_script.sh
check_program g++
mv vpl_evaluate.cpp vpl_evaluate.cpp_
mv functions.h functions.h_
mv structures.h structures.h_
mv tests.h tests.h_
get_source_files cpp h hpp
mv vpl_evaluate.cpp_ vpl_evaluate.cpp
mv functions.h_ functions.h
mv structures.h_ structures.h
mv tests.h_ tests.h

g++ -std=c++17 -fno-diagnostics-color -o vpl_execution $SOURCE_FILES -lm -lutil
