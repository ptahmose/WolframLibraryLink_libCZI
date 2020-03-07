# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
#cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (wllczi SHARED
						src/Initialization.cpp
						src/WLLFunctions.cpp)

target_link_libraries(wllczi PRIVATE libCZIStatic JxrDecodeStatic)
#set_target_properties(SqliteImgDoc PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
set_target_properties(wllczi PROPERTIES CXX_STANDARD 11 CXX_EXTENSIONS OFF)



# TODO: Add tests and install targets if needed.
