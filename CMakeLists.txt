cmake_minimum_required(VERSION 2.6)
project(geometryindex)

add_executable(geometryindex main.cpp)

install(TARGETS geometryindex RUNTIME DESTINATION bin)
