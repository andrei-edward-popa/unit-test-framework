# **Unit Test Framework written in C++ for Moodle VPL**

In order to use this project, you need to know that files for creating tests are named tests.h and vpl_evaluate.cases, and the 'student' file is named main.cpp.

## **Requirement**:
Compiler that supports C++17 features.

## **Usage**: 
If you are in project root directory, you need to run the following commands:
```
make run
./vpl_execution
```
If you want to run UI Test Creater, run the following command:
```
make test
```
If you want to remove all files created, run the following command:
```
make mrproper
```
I changed the following for this version:
- Added 2 new keywords for use in vpl_evaluate.cases: REQUIRES AND DEPENDENCIES. 
    - REQUIRES keyword requires that a unit test defined in tests.h file need to be passed for take grade on the test.
    - DEPENDENCIES keyword use REQUIRES and DEPENDENCIES from another test to reduce copy-pasting code.  
- Unit tests need to be written in tests.h file, a file with a specific format. You can gete a documentation of this format in the file itself.
- Files functions.h, structures.h and vpl_evaluate.cpp are the backend of this framework.