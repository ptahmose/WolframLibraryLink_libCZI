include(FetchContent)

# Download RapidJSON
#ExternalProject_Add(
#    libCZI
#    PREFIX "vendor/libCZI"
#    GIT_REPOSITORY "https://github.com/ptahmose/libCZI.git"
#    GIT_TAG 3bc9e9094ada28ade93e3da587d3df311e65f44f
#    TIMEOUT 10
#    CMAKE_ARGS
#        -DLIBCZI_BUILD_CZICMD=OFF
#        -DLIBCZI_BUILD_DYNLIB=OFF
#)


#ExternalProject_Get_Property(libCZI libCZIinstalldir)
#set(LIBCZI_INCLUDE_DIR ${libCZIinstalldir}/Src/libCZI)
FetchContent_Declare(
  libCZI
  GIT_REPOSITORY https://github.com/ptahmose/libCZI.git
  GIT_TAG        3bc9e9094ada28ade93e3da587d3df311e65f44f
  #GIT_COMMIT 3bc9e9094ada28ade93e3da587d3df311e65f44f
  #CMAKE_ARGS "-DLIBCZI_BUILD_CZICMD=OFF -DLIBCZI_BUILD_DYNLIB=OFF"
  # ^ or like this - sometimes it's better because FetchContent won't look
  # into remote to see if branch head was updated or not - good for stable
  # tags like 'vX.X' corresponding to releases
)

if(NOT libCZI_POPULATED)
  message(STATUS "Fetching libCZI")
  FetchContent_Populate(libCZI)

  set(LIBCZI_BUILD_CZICMD OFF CACHE BOOL "" FORCE)
  set(LIBCZI_BUILD_DYNLIB OFF CACHE BOOL "" FORCE)

  add_subdirectory(${libczi_SOURCE_DIR} ${libczi_BINARY_DIR})
endif()

#add_subdirectory("${CMAKE_SOURCE_DIR}/modules/libCZI")