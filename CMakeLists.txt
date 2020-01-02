cmake_minimum_required(VERSION 3.13)
project(mini-db C)

include_directories(include)
file(GLOB SOURCES "src/*.c")
file(GLOB PARSING "parsing/*.c")

add_executable(mini ${SOURCES} ${PARSING})
