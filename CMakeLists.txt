# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.7)

set(WLLCZI_MAJOR 0)
set(WLLCZI_MINOR 1)
set(WLLCZI_PATCH 2)
set(WLLCZI_EXT "alpha")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules" ${CMAKE_MODULE_PATH})

project ("WolframLibraryLink_libCZI")

# include RapidJSON -> https://www.jibbow.com/posts/rapidjson-cmake/
include("${CMAKE_SOURCE_DIR}/modules/rapidJSON.cmake")
#include_directories(${RAPIDJSON_INCLUDE_DIR})

message(STATUS ${CMAKE_MODULE_PATH} )

find_package(Mathematica)

message(STATUS "Mathematica_INCLUDE_DIR: ${Mathematica_INCLUDE_DIR}")
message(STATUS "Mathematica_WolframLibrary_LIBRARIES: ${Mathematica_WolframLibrary_LIBRARIES}")

#include_directories("C:/Program Files/Wolfram Research/Mathematica/12.0/SystemFiles/IncludeFiles/C")
#include_directories("C:/Program Files/Wolfram Research/Mathematica/12.0/SystemFiles/Links/WSTP/DeveloperKit/Windows-x86-64/CompilerAdditions")


include("${CMAKE_SOURCE_DIR}/modules/libCZI.cmake")
FetchContent_GetProperties(libCZI)
set(LIBCZI_INCLUDE_DIR "${libczi_SOURCE_DIR}/Src/libCZI")
message(STATUS ${LIBCZI_INCLUDE_DIR})

message(STATUS "#######################################################")
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    option(WOLFRAMLIBRARYLINK_MINT32 "Use 32-bit integer as 'mint'" OFF) 
    message(STATUS "'mint' is 64-bit integer")
else()
    option(WOLFRAMLIBRARYLINK_MINT32 "Use 32-bit integer as 'mint'" ON) 
    message(STATUS "'mint' is 32-bit integer")
endif()
message(STATUS "#######################################################")

add_subdirectory ("wllczi")

# GTest
enable_testing()

find_package(GTest QUIET)
if ("${GTest_FOUND}")
    message(STATUS "Found GTest, using it.")
else()
    message(STATUS "Did not find package GTest, will attempt to fetch it locally.")

    include(FetchContent)

    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.10.0
    )

    FetchContent_MakeAvailable(googletest)
#[[
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
      FetchContent_Populate(googletest)
    endif()
	  	#add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
      message(STATUS "*#+#+#+#+#+#+#+#")
      message(STATUS ${googletest_SOURCE_DIR})
      message(STATUS "*#+#+#+#+#+#+#+#")
		  set(GTEST_INCLUDE_DIRS ${googletest_SOURCE_DIR}/googletest/include)
      set(GTEST_BOTH_LIBRARIES ${CMAKE_BINARY_DIR}/lib/libgtest ${CMAKE_BINARY_DIR}/lib/libgtest_main)    
    #endif()
    ]]
endif()

add_subdirectory("wllcziTests")

