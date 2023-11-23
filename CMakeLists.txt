cmake_minimum_required(VERSION 3.26)
project(J1939)


add_subdirectory(tests)
add_library(J1939 J1939.h
        J1939.cpp)