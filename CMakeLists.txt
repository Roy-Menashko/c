cmake_minimum_required(VERSION 3.28.3)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled ManageJerries.c
        Jerry.c
        Jerry.h
        Defs.h
        LinkedList.c
        LinkedList.h
        KeyValuePair.c
        KeyValuePair.h
        HashTable.c
        HashTable.h
        MultiValueHashTable.c
        MultiValueHashTable.h
        JerryBoreeMain.c)
