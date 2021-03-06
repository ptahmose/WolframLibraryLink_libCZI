﻿# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
#cmake_minimum_required (VERSION 3.8)

if(CMAKE_BUILD_TYPE MATCHES Debug)
 set(wllczi_ENABLE_LOGGING 1)
else()
 set(wllczi_ENABLE_LOGGING 0)
endif()

set(wllczi_LOGLEVEL 1)

set(wllczi_VERSION_MAJOR	${WLLCZI_MAJOR})
set(wllczi_VERSION_MINOR	${WLLCZI_MINOR})
set(wllczi_VERSION_PATCH	${WLLCZI_PATCH})
set(wllczi_VERSION_EXT    ${WLLCZI_EXT})

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/src/wllczi_Config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/wllczi_Config.h"
	ESCAPE_QUOTES @ONLY)

# Add source to this project's executable.
add_library (wllczi SHARED
						src/Initialization.cpp
						src/WLLFunctions.cpp
						src/getLibInfo.h
						src/getLibInfo.cpp
						src/stringReturnHelper.cpp
						src/stringReturnHelper.h
						src/CziInstanceManager.cpp
						src/CziInstanceManager.h
						src/CziReader.cpp
						src/CziReader.h
						src/inc_libCzi.h
						src/errorhelper.h
						src/errorhelper.cpp
						src/WolframLibLinkUtils.h
						src/WolframLibLinkUtils.cpp
						src/MImageHelper.cpp
						src/MImageHelper.h
						src/CziUtilities.cpp
						src/CziUtilities.h 
						"src/dbgprint.cpp"
						"src/dbgprint.h" 
						"src/utils.cpp"
						"src/utils.h" 
						"src/CziReaderSbBlkStore.h"
						"src/CziReaderSbBlkStore.cpp"
						"src/finally.h")

set_target_properties(wllczi PROPERTIES CXX_STANDARD 11)

add_dependencies(wllczi rapidjson)

target_link_libraries(wllczi PRIVATE libCZIStatic JxrDecodeStatic)

target_include_directories(wllczi PRIVATE ${Mathematica_INCLUDE_DIR} ${LIBCZI_INCLUDE_DIR}  ${RAPIDJSON_INCLUDE_DIR})

target_compile_definitions(wllczi PRIVATE WLLCZI_EXPORTS)

target_include_directories(wllczi PRIVATE  ${CMAKE_CURRENT_BINARY_DIR})  # this is necessary so that we can find "wllczi_Config.h" which we created above

if(UNIX AND NOT APPLE)
   # Linux
   target_link_libraries(wllczi PRIVATE pthread dl)
elseif(UNIX AND APPLE)
   # Mac OS
   target_link_libraries(wllczi PRIVATE pthread)
endif()

if(WOLFRAMLIBRARYLINK_MINT32)	
	target_compile_definitions(wllczi PRIVATE MINT_32=1)
endif()

if(WIN32)
 target_compile_definitions(wllczi PRIVATE _WIN32API=1)
endif()

#########################################################################################
# The static wllczi-library is intended to facilitate unit-tests
#########################################################################################

add_library (wllczi_static STATIC
						src/Initialization.cpp
						src/WLLFunctions.cpp
						src/getLibInfo.h
						src/getLibInfo.cpp
						src/stringReturnHelper.cpp
						src/stringReturnHelper.h
						src/CziInstanceManager.cpp
						src/CziInstanceManager.h
						src/CziReader.cpp
						src/CziReader.h
						src/inc_libCzi.h
						src/errorhelper.h
						src/errorhelper.cpp
						src/WolframLibLinkUtils.h
						src/WolframLibLinkUtils.cpp
						src/MImageHelper.cpp
						src/MImageHelper.h
						src/CziUtilities.cpp
						src/CziUtilities.h 
						"src/dbgprint.cpp"
						"src/dbgprint.h" 
						"src/utils.cpp"
						"src/utils.h" 
						"src/CziReaderSbBlkStore.h"
						"src/CziReaderSbBlkStore.cpp")


set_target_properties(wllczi_static PROPERTIES CXX_STANDARD 11)

target_include_directories(wllczi_static PRIVATE ${Mathematica_INCLUDE_DIR} ${LIBCZI_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIR})

target_link_libraries(wllczi_static PRIVATE libCZIStatic JxrDecodeStatic)

target_include_directories(wllczi_static PRIVATE  ${CMAKE_CURRENT_BINARY_DIR})  # this is necessary so that we can find "wllczi_Config.h" which we created above

target_compile_definitions(wllczi_static PRIVATE _WLLCZISTATICLIB)

if(WOLFRAMLIBRARYLINK_MINT32)	
	target_compile_definitions(wllczi_static PRIVATE MINT_32=1)
endif()

if(WIN32)
 target_compile_definitions(wllczi_static PRIVATE _WIN32API=1)
endif()

