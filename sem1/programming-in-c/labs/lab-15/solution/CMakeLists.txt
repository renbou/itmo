cmake_minimum_required(VERSION 3.16)
project(itmo_is_2020_c_labs C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wpedantic")

set(renarc_sources
        main.c argument_parser.c linear_map.c errors.c renarclib.c)

add_executable(renarc ${renarc_sources})