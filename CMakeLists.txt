cmake_minimum_required(VERSION 3.15)
project(Grafos C)

set(CMAKE_C_STANDARD 99)

add_executable(Grafos main.c Library/grafos.c Library/grafos.h Library/minHeap.c Library/minHeap.h)
