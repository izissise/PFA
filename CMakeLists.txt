project(PFA)

cmake_minimum_required(VERSION 2.6)

ENABLE_TESTING()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(CppUnit REQUIRED)

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

add_library(
# executable name
  pfa SHARED
# source files
  ${source_files}
  ${include_files}
)

add_executable(
# executable name
  pfaexe
# source files
  main.cpp
)

target_link_libraries(pfaexe pfa)

file(
  GLOB_RECURSE
  test_files
  test/*
)

add_executable(
# executable name
  UnitTester
# source files
  ${test_files}
)

target_link_libraries(UnitTester ${CPPUNIT_LIBRARY} pfa dl)

# Add coverage options for CI
if(COVERAGE)
	set_target_properties(pfa pfaexe UnitTester PROPERTIES
						  COMPILE_FLAGS "-fprofile-arcs -ftest-coverage"
						  LINK_FLAGS "-coverage")
endif(COVERAGE)

# The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

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
