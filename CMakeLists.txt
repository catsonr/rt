cmake_minimum_required(VERSION 3.16)
project(a.out)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CONFIGURATION}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CONFIGURATION}")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

if(EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s FULL_ES3=1 -s USE_WEBGL2=1")
endif()

file(GLOB_RECURSE PROJECT_SOURCES
     CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.cpp"
     "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(a.out ${PROJECT_SOURCES})

target_include_directories(a.out PRIVATE
  ${CMAKE_SOURCE_DIR}/src/pbrt
  ${CMAKE_SOURCE_DIR}/src/rtiow
  ${CMAKE_SOURCE_DIR}/src/test
)

target_link_libraries(a.out PRIVATE SDL3::SDL3)