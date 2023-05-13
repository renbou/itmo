cmake_minimum_required(VERSION 3.16)
project(itmo_is_2020_c_labs C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS -Ofast)

set(bmp_sources
        main.c img.c utils.c errors.c)

add_executable(itmo_is_2020_c_lab14 ${bmp_sources})
