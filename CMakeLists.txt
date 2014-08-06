cmake_minimum_required(VERSION 2.8)

project(Project)

set(NAMELIB "pfa")
set(NAMEEXEC "pfaexe")

set(INCLUDE_DIRECTORY "${NAMELIB}/inc")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(CppUnit)
find_package(SFML 2.1 REQUIRED system window graphics network audio)

INCLUDE(EnableCppExtras)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(InstallFilesRecursive)
INCLUDE(EnableCoverageReport)
INCLUDE(EnableSlocCount)
INCLUDE(GenerateDoxygen)
INCLUDE(GenerateCppcheck)
INCLUDE(DefineProjectVersion)
INCLUDE(CurrentDate)

# --- user options ---
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not" ON)

# --- global definitions ---
DEFINE_PROJECT_VERSION(PROJECT 0 0 1 "snapshot")
CURRENT_DATE(PROJECT_BUILD_DATE)

# --- global compiler flags ---
INCLUDE(PedanticCompilerWarnings)

# decide how to do name demangling
CHECK_INCLUDE_FILE_CXX("cxxabi.h" HAVE_CXXABI_H)
IF(HAVE_CXXABI_H)
  ADD_DEFINITIONS(-DDEMANGLE_GCC)
ELSEIF(MSVC)
  ADD_DEFINITIONS(-DDEMANGLE_MSVC)
ELSE()
  MESSAGE(WARNING "No demangling solution found for the system.")
ENDIF()

# All source files using globs
file(
  GLOB_RECURSE
  lib_source_files
  ${NAMELIB}/src/*
)

file(
  GLOB_RECURSE
  lib_include_files
  ${INCLUDE_DIRECTORY}/*
)

file(
  GLOB_RECURSE
  exec_source_files
  ${NAMEEXEC}/src/*
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "Config.h.in"
  "${INCLUDE_DIRECTORY}/Config.h"
)

include_directories("${INCLUDE_DIRECTORY}/")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/${INCLUDE_DIRECTORY}/")

add_library(
# executable name
  ${NAMELIB} STATIC
# source files
  ${lib_source_files}
  ${lib_include_files}
)

add_executable(
# executable name
  ${NAMEEXEC}
# source files
  ${exec_source_files}
)

target_link_libraries(${NAMEEXEC} ${NAMELIB})

file(
  GLOB_RECURSE
  test_files
  test/*
)

if(NOT CPPUNIT_FOUND)
  message(WARNING "CppUnit library can't be found, Unit test won't be compiled")
endif()

# --- enable unit tests if desired ---
if(BUILD_TESTS AND CPPUNIT_FOUND)

  add_executable(
  # executable name
    UnitTester
  # source files
   ${test_files}
  )

  include_directories(${CPPUNIT_INCLUDE_DIRS})
  target_link_libraries (UnitTester ${CPPUNIT_LIBRARY} pfa dl)

  set_target_properties(pfa pfaexe UnitTester PROPERTIES
  				       COMPILE_FLAGS "--coverage"
				       LINK_FLAGS "-coverage")

  # --- coverage report ---
  ENABLE_COVERAGE_REPORT(TARGETS ${RSC_NAME}
                         TESTS  ${RSC_TEST_NAME}
                         FILTER ${test_files})

  ENABLE_TESTING()

ENDIF()

if (SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries (pfa ${SFML_LIBRARIES})
endif()

# --- cppcheck ---
GENERATE_CPPCHECK(SOURCES src test
                  "${CMAKE_CURRENT_BINARY_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/test"
                  ENABLE_IDS style
                  INLINE_SUPPRESSION)
