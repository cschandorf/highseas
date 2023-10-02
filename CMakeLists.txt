#set version of cmake
cmake_minimum_required(VERSION 3.10)

#define the project and version
project(battleship VERSION 1.0.0.0)

#Minimum C++ standards
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

#add source files to a list
set(SOURCE_FILES utils.cpp error.cpp vessel.cpp vessel_test.cpp gameboard.cpp main.cpp)

#find Catch2 libraries
find_package(Catch2 3 REQUIRED)

#The executable to build 
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#Link the target against Catch2
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)
