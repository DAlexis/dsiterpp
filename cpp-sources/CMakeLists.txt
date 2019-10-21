cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project("ode-cpp-lite" CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -pg -g -lprofiler -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g -DNDEBUG -funroll-loops -ffast-math -lprofiler")

## This macro creates variable ${PROJECT_NAME}_INCLUDE_DIRS with list of 
# include directories exported by project
macro(export_include_dirs)
    set(${PROJECT_NAME}_INCLUDE_DIRS
        ${ARGV}
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
endmacro()

# This macro creates variable ${PROJECT_NAME}_LIBRARY with list of libraries exported 
# by project
macro(export_libs)
    set(${PROJECT_NAME}_LIBRARY
        ${ARGV}
        CACHE INTERNAL "${PROJECT_NAME}: used and exported libs" FORCE)
endmacro()

add_subdirectory(ode-cpp-lite)

# To enable ctest usage
enable_testing()
include(detect-gtest.cmake)
if(GTEST_FOUND)
    add_subdirectory(unit-tests)
    #add_subdirectory(functional-tests)
endif()

