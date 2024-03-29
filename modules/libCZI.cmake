include(FetchContent)


FetchContent_Declare(
  libCZI
  GIT_REPOSITORY https://github.com/ZEISS/libczi
  GIT_TAG        origin/main  # use the latest in this branch
)

if(NOT libCZI_POPULATED)
  message(STATUS "Fetching libCZI")
  FetchContent_Populate(libCZI)

  set(LIBCZI_BUILD_CZICMD OFF CACHE BOOL "" FORCE)
  set(LIBCZI_BUILD_DYNLIB OFF CACHE BOOL "" FORCE)
  set(LIBCZI_BUILD_UNITTESTS OFF CACHE BOOL "" FORCE)

  add_subdirectory(${libczi_SOURCE_DIR} ${libczi_BINARY_DIR})
endif()

