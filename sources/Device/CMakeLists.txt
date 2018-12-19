cmake_minimum_required(VERSION 3.0)

project (Device56)

file(GLOB SOURCES src/*.cpp src/Data/*.cpp)

add_executable(Device56 ${SOURCES})
