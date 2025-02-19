cmake_minimum_required(VERSION 3.15)
project(hackw C)

set(CMAKE_C_STANDARD 11)

add_executable(hackw main.c hackw.c)

add_executable(get8key get8key.c)
