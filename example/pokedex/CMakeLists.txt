cmake_minimum_required(VERSION 3.10)

project(pokedex VERSION 1.0 LANGUAGES C)

add_library(pokedex SHARED
    alfred
)

add_executable(pokedex pokedex.c)