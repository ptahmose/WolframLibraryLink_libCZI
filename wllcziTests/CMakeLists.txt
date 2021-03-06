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

target_include_directories(wllcziTests PRIVATE ${RAPIDJSON_INCLUDE_DIR})

target_include_directories(wllcziTests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../wllczi/>)

target_include_directories(wllcziTests PRIVATE ${Mathematica_INCLUDE_DIR} ${LIBCZI_INCLUDE_DIR})

target_link_libraries(wllcziTests PRIVATE  wllczi_static)

# Link test executable against gtest & gtest_main
if ("${GTest_FOUND}")
  target_link_libraries(wllcziTests PRIVATE  ${GTEST_BOTH_LIBRARIES})
else()
    target_link_libraries(wllcziTests PRIVATE gtest_main gmock)
endif()

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


add_test(wllcziTests wllcziTests)