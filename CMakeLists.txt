# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.11)

set(WLLCZI_MAJOR 0)
set(WLLCZI_MINOR 1)
set(WLLCZI_PATCH 3)
set(WLLCZI_EXT "alpha")

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/Mathematica/CZIReader/PacletInfo.wl.in"  "${CMAKE_CURRENT_SOURCE_DIR}/Mathematica/CZIReader/PacletInfo.wl")

if(WIN32)
  # use "static C-runtime" -> https://stackoverflow.com/questions/14172856/compile-with-mt-instead-of-md-using-cmake
  # Note: this requires CMAKE version 3.15+
  cmake_policy(SET CMP0091 NEW)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif(WIN32)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules" ${CMAKE_MODULE_PATH})

project ("WolframLibraryLink_libCZI")

# include RapidJSON -> https://www.jibbow.com/posts/rapidjson-cmake/
include("${CMAKE_SOURCE_DIR}/modules/rapidJSON.cmake")

message(STATUS ${CMAKE_MODULE_PATH} )

if(WLLCZI_MATHEMATICAHEADERS)
  set(Mathematica_INCLUDE_DIR ${WLLCZI_MATHEMATICAHEADERS})
else()
  find_package(Mathematica)
endif(WLLCZI_MATHEMATICAHEADERS)

message(STATUS "Mathematica_INCLUDE_DIR: ${Mathematica_INCLUDE_DIR}")
message(STATUS "Mathematica_WolframLibrary_LIBRARIES: ${Mathematica_WolframLibrary_LIBRARIES}")

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

option(WLLCZI_BUILD_UNITTESTS "Build unit-tests 'wllcziTests'." ON)
if (WLLCZI_BUILD_UNITTESTS)
  # GTest
  enable_testing()

  set(GTEST_MSVC_SEARCH "MT") # we want to have the "static runtime library" version
  find_package(GTest QUIET)
  if ("${GTest_FOUND}")
      message(STATUS "Found GTest, using it.")
      include_directories(${GTEST_INCLUDE_DIRS})
  else()
      message(STATUS "Did not find package GTest, will attempt to fetch it locally.")

      include(FetchContent)

      FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
        CMAKE_ARGS     "DUSE_STATIC_RT=ON"
      )

  #[[    FetchContent_MakeAvailable(googletest)     #Requires CMake 3.16+ ]]

      FetchContent_GetProperties(googletest)
      if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
      endif()
  endif()

  add_subdirectory("wllcziTests")
endif(WLLCZI_BUILD_UNITTESTS)
