cmake_minimum_required(VERSION 3.17)
project(DES C)

set(CMAKE_C_STANDARD 99)

add_executable(des main.c main.h gopt/gopt.c gopt/gopt.h)