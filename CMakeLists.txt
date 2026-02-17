cmake_minimum_required(VERSION 4.0)
project(LO21 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(LO21
        hash.c
        hash.h
        inference.c
        inference.h
        kb.c
        kb.h
        list.c
        list.h
        main.c
        rule.c
        rule.h
        utils.c
        utils.h
        tests.c
        tests.h)
