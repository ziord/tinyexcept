cmake_minimum_required(VERSION 3.15)
project(tinyexcept C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

include_directories(tests)

add_executable(tinyexcept main.c tests/defs.c tests/test_tinyexcept.c)