cmake_minimum_required(VERSION 3.16)
project(PF-RING C CXX)

# C = C lauguage / CXX = C++
#setting standard liberary, C; users can use one of the following libraries when users require
set(CMAKE_C_FLAGS "-std=gnu99")
#set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

#setting standard liberary, C++
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# pthread shall be included
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

#passing the macro variable into the C/C++ codes
add_compile_definitions(_CmockaTest_)
#add_compile_definitions(_CmockaWinPFRINGTest_)

# Copy the dependent files of the project (cfg files or files users want to open)
configure_file(${CMAKE_SOURCE_DIR}/Imports/Exes/pfcount ${CMAKE_BINARY_DIR}/Imports/Exes/pfcount COPYONLY)

add_subdirectory(Tests)
add_subdirectory(Sources)