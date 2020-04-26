cmake_minimum_required(VERSION 3.15)
project(Grafos C)

set(CMAKE_C_STANDARD 99)

add_executable(Grafos main.c libraries/grafos.c libraries/grafos.h libraries/minHeap.c libraries/minHeap.h)
