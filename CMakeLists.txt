# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.7)

#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/" "${CMAKE_CURRENT_SOURCE_DIR}/modules/")

project ("WolframLibraryLink_libCZI")

# include RapidJSON -> https://www.jibbow.com/posts/rapidjson-cmake/
include("${CMAKE_SOURCE_DIR}/modules/rapidjson.cmake")
include_directories(${RAPIDJSON_INCLUDE_DIR})

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules)

message(STATUS ${CMAKE_MODULE_PATH} )
##FIND_PACKAGE(Mathematica 11.0.0 REQUIRED) 
include("${CMAKE_SOURCE_DIR}/modules/FindMathematica.cmake")

message(STATUS "Mathematica_INCLUDE_DIR")
message(STATUS ${Mathematica_INCLUDE_DIR})
message(STATUS "Mathematica_WolframLibrary_LIBRARIES")
message(STATUS ${Mathematica_WolframLibrary_LIBRARIES})


#include_directories("C:/Program Files/Wolfram Research/Mathematica/12.0/SystemFiles/IncludeFiles/C")
#include_directories("C:/Program Files/Wolfram Research/Mathematica/12.0/SystemFiles/Links/WSTP/DeveloperKit/Windows-x86-64/CompilerAdditions")


set(LIBCZI_BUILD_CZICMD OFF CACHE BOOL "" FORCE)
set(LIBCZI_BUILD_DYNLIB OFF CACHE BOOL "" FORCE)
add_subdirectory(extlibs/libCZI)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libCZI/Src/libCZI)


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
    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
		add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
		set(GTEST_INCLUDE_DIRS ${googletest_SOURCE_DIR}/googletest/include)
		set(GTEST_BOTH_LIBRARIES ${CMAKE_BINARY_DIR}/lib/libgtest.a ${CMAKE_BINARY_DIR}/lib/libgtest_main.a)    
    endif()

endif()

# https://stackoverflow.com/questions/13513905/how-to-set-up-googletest-as-a-shared-library-on-linux
# sudo apt-get install libgtest-dev
# cd /usr/src/gtest
# sudo cmake CMakeLists.txt
# sudo make
# sudo make install
# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
# sudo cp *.a /usr/lib

#[[
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
  message(STATUS ${googletest_SOURCE_DIR})
  message(STATUS ${googletest_BINARY_DIR})
  message(STATUS ${googletest_INCLUDE_DIR})
  include_directories(${googletest_SOURCE_DIR}/googletest/include)
  ##set(GTEST_BOTH_LIBRARIES ${googletest_BINARY_DIR}/googlemock/gtest/libgtest.a ${googletest_BINARY_DIR}/googlemock/gtest/libgtest_main.a)
  set(GTEST_BOTH_LIBRARIES ${CMAKE_BINARY_DIR}/lib/libgtest.a ${CMAKE_BINARY_DIR}/lib/libgtest_main.a)
endif()
]]

##target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

message(STATUS "XYZ")
message(STATUS ${GTEST_INCLUDE_DIRS} )

add_subdirectory("wllcziTests")

#include_directories(${GTEST_INCLUDE_DIRS})

# add_subdirectory ("SqliteImgDocTests")

# add_subdirectory("CmdSqliteImgDoc")

# add_subdirectory("ConvCZI")
