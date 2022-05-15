cmake_minimum_required(VERSION 3.0.0)
project(lab-1 VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(task_a src/a.cpp)
add_executable(task_b src/b.cpp)
add_executable(task_c src/c.cpp)
add_executable(task_d src/d.cpp)
add_executable(task_e src/e.cpp)
add_executable(task_f src/f.cpp)
add_executable(task_h src/h.cpp)
add_executable(task_i src/i.cpp)