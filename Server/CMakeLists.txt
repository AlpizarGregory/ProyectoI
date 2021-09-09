cmake_minimum_required(VERSION 3.20)
project(Breakout)

set(CMAKE_CXX_STANDARD 14)

add_executable(Breakout code/socketServer.cpp code/socketServer.h)
