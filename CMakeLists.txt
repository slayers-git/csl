cmake_minimum_required (VERSION 3.12)
project (csl)

set (HEADERS
    ./pvector/pvector.h 
    ./cthings/platform.h
    ./cthings/standard.h 
    ./cthings/attribute.h
    ./cthings/begin_code.h
    ./cthings/die.h
    ./cthings/c89.h)

install (FILES ${HEADERS} DESTINATION include/csl)
