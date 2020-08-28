cmake_minimum_required(VERSION 3.16)
project(md5 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")

# Uncomment to build with debug enabled. This will print out each message block that gets processed
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")


add_executable(md5 src/main.c)