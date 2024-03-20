cmake_minimum_required(VERSION 3.8)

project(Test_Technique_Sym)
set(CMAKE_CXX_STANDARD 20)


#source code path
file(GLOB MAIN_SRC "src/main.cpp")
file(GLOB SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES ${MAIN_SRC}) #${SOURCES} = all .cpp without main.cpp
file(GLOB HEADERS "src/*.hpp")


#creation of executable 
add_executable(app ${MAIN_SRC} ${SOURCES} ${HEADERS})
target_link_libraries(app stdc++fs)
#bonus : in order to use execution library
#target_link_libraries(app stdc++fs tbb)            


#Catch2 config
set(Catch2_DIR /usr/local/includes/Catch2)                  # may be commented if you don't want to use test
find_package(Catch2 REQUIRED)                               # may be commented if you don't want to use test

#tests path
file(GLOB TESTS "tests/*.cpp")                              # may be commented if you don't want to use test
include(Catch)                                              # may be commented if you don't want to use test

#creation of the test executable
add_executable(test ${TESTS} ${SOURCES} ${HEADERS})         # may be commented if you don't want to use test
target_link_libraries(test Catch2::Catch2 stdc++fs)         # may be commented if you don't want to use test