cmake_minimum_required(VERSION 3.13)

set(CMAKE_CONFIGURATION_TYPES "ASAN;MSAN;USAN" CACHE STRING "" FORCE)

# General compile and link options
set(COMPILE_OPTS -Wall -Wextra -Werror -pedantic -pedantic-errors)
set(LINK_OPTS "")

# Sanitizers options
if (CMAKE_BUILD_TYPE MATCHES ASAN)
    list(APPEND COMPILE_OPTS -fsanitize=address -fno-omit-frame-pointer
        -fno-inline -fno-sanitize-recover=all)
    list(APPEND LINK_OPTS -fsanitize=address)
endif()
if (CMAKE_BUILD_TYPE MATCHES MSAN)
    list(APPEND COMPILE_OPTS -fsanitize=memory
        -fno-omit-frame-pointer -fsanitize-memory-track-origins=2
        -fno-sanitize-recover=all)
    list(APPEND LINK_OPTS -fsanitize=memory
        -fsanitize-memory-track-origins=2)
endif()
if (CMAKE_BUILD_TYPE MATCHES USAN)
    list(APPEND COMPILE_OPTS
        -fsanitize=undefined,float-cast-overflow,float-divide-by-zero
        -fno-omit-frame-pointer -fno-sanitize-recover=all
        -fsanitize-recover=alignment)
    list(APPEND LINK_OPTS
        -fsanitize=undefined,float-cast-overflow,float-divide-by-zero)
endif()

# Configure clang-tidy
if (${USE_CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

function(setup_warnings TARGET)
    # Warnings
    target_compile_options(${TARGET} PUBLIC -Wold-style-cast)
    target_compile_options(${TARGET} PUBLIC -Wnull-dereference)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${TARGET} PUBLIC -Wduplicated-branches)
        target_compile_options(${TARGET} PUBLIC -Wduplicated-cond)
        target_compile_options(${TARGET} PUBLIC -Wsuggest-override)
        target_compile_options(${TARGET} PUBLIC -Wuseless-cast)
        target_compile_options(${TARGET} PUBLIC -Wreturn-local-addr)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${TARGET} PUBLIC -Wreturn-stack-address)
        target_compile_options(${TARGET} PUBLIC -Wloop-analysis)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        target_compile_options(${TARGET} PUBLIC -Wreturn-stack-address)
        target_compile_options(${TARGET} PUBLIC -Wloop-analysis)
    endif()
endfunction(setup_warnings)

set(PROJECT_NAME monte_carlo_pi)
project(${PROJECT_NAME})

# Set up the compiler flags
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inlcude directories
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Separate executable: main
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Compile source files into a library
add_library(monte_carlo_pi_lib ${SRC_FILES})
target_compile_options(monte_carlo_pi_lib PUBLIC ${COMPILE_OPTS})
target_link_options(monte_carlo_pi_lib PUBLIC ${LINK_OPTS})
setup_warnings(monte_carlo_pi_lib)

# Main is separate
add_executable(monte_carlo_pi ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_compile_options(monte_carlo_pi PRIVATE ${COMPILE_OPTS})
target_link_options(monte_carlo_pi PRIVATE ${LINK_OPTS})
setup_warnings(monte_carlo_pi)

# linking Main against the library
target_link_libraries(monte_carlo_pi monte_carlo_pi_lib)
