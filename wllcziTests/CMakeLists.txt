# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable( wllcziTests 
                    WLLFunctionsTests.cpp  
                    "CziUtilitiesTest.cpp" 
                    "CziReaderTest.cpp" 
                    "testutilities.cpp"
                    "testutilities.h")

add_dependencies(wllcziTests rapidjson)

target_include_directories(wllcziTests PRIVATE ${GTEST_INCLUDE_DIRS} wllczi)

target_include_directories(wllcziTests PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../wllczi/>
)
target_include_directories(wllcziTests PRIVATE ${Mathematica_INCLUDE_DIR} ${LIBCZI_INCLUDE_DIR})


# Link test executable against gtest & gtest_main
target_link_libraries(wllcziTests PRIVATE ${GTEST_BOTH_LIBRARIES} wllczi_static)

target_compile_definitions(wllcziTests PRIVATE WLLCZITESTSAMPLEFILEDIR=${CMAKE_CURRENT_SOURCE_DIR}/samples)

if(UNIX AND NOT APPLE)
   # Linux
   target_link_libraries(wllcziTests PRIVATE pthread dl)
elseif(UNIX AND APPLE)
   # Mac OS
   target_link_libraries(wllcziTests PRIVATE pthread)
endif()

if(WOLFRAMLIBRARYLINK_MINT32)	
	target_compile_definitions(wllcziTests PRIVATE MINT_32=1)
endif()


##target_link_libraries(SqliteImgDocTests PRIVATE SQLiteCpp)

#[[
# Cygwin seems to need some special treatment -> https://stackoverflow.com/questions/18784112/googletest-1-6-with-cygwin-1-7-compile-error-fileno-was-not-declared-in-this
 if(NOT CYGWIN)
    set_target_properties(SqliteImgDocTests PROPERTIES
               CXX_STANDARD 11
               CXX_EXTENSIONS OFF)
else()
    set_target_properties(SqliteImgDocTests PROPERTIES
               CXX_STANDARD 11
               CXX_EXTENSIONS ON)
endif()
]]

add_test( wllcziTests  wllcziTests  )