cmake_minimum_required(VERSION 3.16)
project(itmo_is_2020_c_labs C)

set(CMAKE_C_STANDARD 11)

set(id3v2_sources
        config.c utils.c main.c id3v2.c errors.c)

add_executable(id3v2 ${id3v2_sources})