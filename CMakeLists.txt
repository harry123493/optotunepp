cmake_minimum_required(VERSION 3.16.0)

project(optotunepp VERSION 1.0.0 LANGUAGES CXX)

if (MSVC)
    add_compile_options(/W4 /GR)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wformat=2
        -Wno-unused-parameter
        -Wshadow
        -Wwrite-strings
        -Wredundant-decls
        -Wmissing-include-dirs
)
endif()
add_subdirectory(optotunepp)