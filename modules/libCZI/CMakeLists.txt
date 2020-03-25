FetchContent_GetProperties(libCZI)

if(NOT libCZI_POPULATED)
  message(STATUS "Fetching libCZI")

  FetchContent_Populate(libCZI)

  message(STATUS *********************************************************)
  message(STATUS ${libczi_SOURCE_DIR})
  message(STATUS *********************************************************)
  #get_cmake_property(_variableNames VARIABLES)
  #foreach (_variableName ${_variableNames})
  #  message(STATUS "${_variableName}=${${_variableName}}")
  #endforeach()
  set(LIBCZI_INCLUDE_DIR "${libczi_SOURCE_DIR}/Src/libCZI")
  message(STATUS ${LIBCZI_INCLUDE_DIR})
  message(STATUS *********************************************************)

  #set(LIBCZI_INCLUDE_DIR ${libczi_SOURCE_DIR}/Src/libCZI)
  # Set relevant vars
#  set(IMGUI_DIR ${imgui_SOURCE_DIR})
#  set(IMGUI_SFML_FIND_SFML CACHE BOOL FALSE)

#  add_subdirectory(${imgui-sfml_SOURCE_DIR} ${imgui-sfml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

  set(LIBCZI_INCLUDE_DIR "${libczi_SOURCE_DIR}/Src/libCZI")