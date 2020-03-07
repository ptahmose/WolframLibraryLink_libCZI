cmake_minimum_required(VERSION 2.8)

project(libCZI)

IF(UNIX)
# linking with 'thread' is necessary if we use std::thread and related under Linux it seems
#   otherwise - the program simply crashes (no build-error)
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -fPIC -D_FILE_OFFSET_BITS=64")
   SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__ANSI__ -fPIC -D_FILE_OFFSET_BITS=64")
ENDIF(UNIX)

option(LIBCZI_BUILD_DYNLIB "Build dynamic library 'libCZI'." ON)
option(LIBCZI_BUILD_STATICLIB "Build static library 'libCZI'." ON)

add_subdirectory(JxrDecode)
add_subdirectory(libCZI)

option(LIBCZI_BUILD_CZICMD "Build application 'CZICmd'." ON)
if (LIBCZI_BUILD_CZICMD)
    add_subdirectory(CZICmd)
endif(LIBCZI_BUILD_CZICMD)
