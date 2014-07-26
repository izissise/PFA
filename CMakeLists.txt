cmake_minimum_required(VERSION 2.6)

project(PFA)

# The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

# All source files using globs
file(
  GLOB_RECURSE
  source_files
  src/*
)

file(
  GLOB_RECURSE
  include_files
  inc/*
)

include_directories("inc/")

add_executable(
# executable name
  pfa
# source files
  ${source_files}
  ${include_files}
)

if (UNIX)
  message("Setting GCC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Winit-self -Wunused-function -pipe")
elseif(APPLE)
  message("Setting Clang flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Winit-self -Wunused-function -pipe")
elseif(WIN)
  message(status "Setting MSVC flags") # http://msdn.microsoft.com/en-us/library/19z1t1wy.aspx
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc-")
endif()
message("** CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
