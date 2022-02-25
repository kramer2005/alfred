cmake_minimum_required(VERSION 3.10)

project(alfred VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -std=c99 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall -Wextra -Werror -pedantic -std=c99")

add_library(alfred SHARED
    src/server.c
)

include_directories(include)

target_include_directories(alfred PUBLIC
    include
)

set_target_properties(alfred PROPERTIES 
    PUBLIC_HEADER "include/alfred.h"
)

install(TARGETS alfred
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)