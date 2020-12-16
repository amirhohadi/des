cmake_minimum_required(VERSION 3.17)
project(DES C)

set(CMAKE_C_STANDARD 99)
include_directories(.)
add_executable(des main.c main.h gopt/gopt.h gopt/gopt.c des/des.c des/des.h des/deshandler.h des/deshandler.c  triple_des/tdeshandler.c triple_des/tdeshandler.h)