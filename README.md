# Test Technique Sym&Cure

## Content of the repository
- src/ : cotains source code
- test/ : contains the tests
- build/ : directory where you should compile and execute the tests and the app
- texts/ : contains the example given in the exercise and the expected output of the app
- value.csv : csv file given in the exercise
- empty.csv : empty file, used for some tests
- CMakeLists.txt : file for building and compile the project
- Exercice de programmation C++.pdf : the exercise


## to compile 
go to the directroy of the project and in its sub directory build/ 
    cd /path/to/project/build

cmake .
make 

make clean 
to remove the execution file app and test and .o files

## To execute the tests
./test

or 
./test [tag] to execute a specific Test Case

## To execute the app
./ app <filename.txt>

example : 
./app /home/texts/texte.txt 


### distro used
Ubuntu 22.04

### compilator used
gcc