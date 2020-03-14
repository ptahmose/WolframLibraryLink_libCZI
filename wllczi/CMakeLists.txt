# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
#cmake_minimum_required (VERSION 3.8)

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
						src/errorhelper.cpp)

set_target_properties(wllczi PROPERTIES CXX_STANDARD 11)

add_dependencies(wllczi rapidjson)

target_link_libraries(wllczi PRIVATE libCZIStatic JxrDecodeStatic)

target_include_directories(wllczi PRIVATE ${Mathematica_INCLUDE_DIR})

#target_link_libraries(wllczi PRIVATE "C:/Program Files/Wolfram Research/Mathematica/12.0/SystemFiles/Links/WSTP/DeveloperKit/Windows-x86-64/CompilerAdditions/wstp64i4.lib")

target_compile_definitions(wllczi PRIVATE WLLCZI_EXPORTS)

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

#set_target_properties(SqliteImgDoc PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
#set_target_properties(wllczi PROPERTIES CXX_STANDARD 11 CXX_EXTENSIONS OFF)

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
						src/errorhelper.cpp)

set_target_properties(wllczi_static PROPERTIES CXX_STANDARD 11)

add_dependencies(wllczi_static rapidjson)

target_include_directories(wllczi_static PRIVATE ${Mathematica_INCLUDE_DIR})

target_link_libraries(wllczi_static PRIVATE libCZIStatic JxrDecodeStatic)

#target_link_libraries(wllczi_static PRIVATE "C:/Program Files/Wolfram Research/Mathematica/12.0/SystemFiles/Links/WSTP/DeveloperKit/Windows-x86-64/CompilerAdditions/wstp64i4.lib")

target_compile_definitions(wllczi_static PRIVATE _WLLCZISTATICLIB)

if(WOLFRAMLIBRARYLINK_MINT32)	
	target_compile_definitions(wllczi_static PRIVATE MINT_32=1)
endif()



# TODO: Add tests and install targets if needed.
