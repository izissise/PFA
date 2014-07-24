cmake_minimum_required(VERSION 2.6)

project(PFA)

# All source files using globs
file(
        GLOB_RECURSE
        source_files
        src/*
)

add_executable(
# executable name
	  pfa
# source files
	${source_files}
)

#if (UNIX)
#  message("Setting GCC flags")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Winit-self -Wunused-function -pipe")
#else()
#  message(status "Setting MSVC flags")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc-")
#endif()
#message("** CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")