cmake_minimum_required(VERSION 2.8)

project(Project)

set(NAME_LIB "pfa")
set(NAME_EXEC "pfaexe")
set(TEST_EXEC "UnitTester")

set(INCLUDE_DIRECTORY "${NAME_LIB}/inc")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(CppUnit)
find_package(SFML 2.1 COMPONENTS system window graphics network audio)

INCLUDE(CMakeDependentOption)
INCLUDE(EnableCppExtras)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(InstallFilesRecursive)
INCLUDE(EnableCoverageReport)
INCLUDE(EnableSlocCount)
INCLUDE(GenerateDoxygen)
INCLUDE(GenerateCppcheck)
INCLUDE(GenerateCccc)
INCLUDE(GenerateValgrind)
INCLUDE(DefineProjectVersion)
INCLUDE(CurrentDate)

# --- user options ---
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not." ON)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Decide if a coverage report should be generated." ON "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(VALGRIND_CHECK "Decide if valgrind should be executed with the test binary to check for memory leaks." ON "BUILD_TESTS" OFF)
OPTION(CODE_METRICS "Decide if code's metrics should be generated." ON)
OPTION(CODE_CHECK "Decide if code should be checked for common errors." ON)
OPTION(GENERATE_DOC "Decide if documentation should be generated." ON)

# --- global definitions ---
DEFINE_PROJECT_VERSION(PROJECT 0 0 1 "snapshot")
CURRENT_DATE(PROJECT_BUILD_DATE)

# --- global compiler flags ---
INCLUDE(SuperPedanticCompilerWarnings)

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
  ${NAME_LIB}/src/*
)

file(
  GLOB_RECURSE
  lib_include_files
  ${INCLUDE_DIRECTORY}/*
)

file(
  GLOB_RECURSE
  exec_source_files
  ${NAME_EXEC}/src/*
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${INCLUDE_DIRECTORY}/Config.h.in"
  "${INCLUDE_DIRECTORY}/Config.h"
)

include_directories("${INCLUDE_DIRECTORY}/")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/${INCLUDE_DIRECTORY}/")

add_library(
# executable name
  ${NAME_LIB} STATIC
# source files
  ${lib_source_files}
  ${lib_include_files}
)

add_executable(
# executable name
  ${NAME_EXEC}
# source files
  ${exec_source_files}
)

target_link_libraries(${NAME_EXEC} ${NAME_LIB})

file(
  GLOB_RECURSE
  test_files
  test/*
)

# --- enable unit tests if desired ---
if(BUILD_TESTS)
  if(NOT CPPUNIT_FOUND)
    message(WARNING "CppUnit library can't be found, Unit test won't be compiled")
  else()
    add_executable(
    # executable name
      ${TEST_EXEC}
    # source files
     ${test_files}
    )

    if(BUILD_TESTS_COVERAGE)
      # --- coverage report ---
      ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXEC} TESTS ${test_files})
    endif()

    if(VALGRIND_CHECK)
      # --- Valgrind --- #
      GENERATE_VALGRIND(BIN "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC}")
    endif()

    set(TEST_REPORT_FILE "cpptestresults.xml")
    # Execute ${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC} binary to execute tests
    ADD_CUSTOM_COMMAND(OUTPUT ${TEST_REPORT_FILE}
                           DEPENDS ${TEST_EXEC}
                           COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC}" ARGS "--xml-file=${TEST_REPORT_FILE}"
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    ADD_CUSTOM_TARGET(test DEPENDS ${TEST_REPORT_FILE})

    include_directories(${CPPUNIT_INCLUDE_DIRS})
    target_link_libraries (${TEST_EXEC} ${CPPUNIT_LIBRARY} ${NAME_LIB})

  endif()
endif()

if (SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries (${NAME_LIB} ${SFML_LIBRARIES})
endif()

#Extra targets:
if(CODE_CHECK)
  # --- cppcheck ---
  GENERATE_CPPCHECK(SOURCES "${PROJECT_SOURCE_DIR}/${NAME_EXEC}/src" "${PROJECT_SOURCE_DIR}/${NAME_LIB}/src"
                            "${PROJECT_SOURCE_DIR}/test"
                            ENABLE_IDS style
                            INLINE_SUPPRESSION)
endif()

if(CODE_METRICS)
  # --- cccc ---
  GENERATE_CCCC(SOURCES "${PROJECT_SOURCE_DIR}/${NAME_EXEC}/src" "${PROJECT_SOURCE_DIR}/${NAME_EXEC}/inc"
                        "${PROJECT_SOURCE_DIR}/${NAME_LIB}/src" "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc"
                        "${PROJECT_SOURCE_DIR}/test")
  # --- SlocCount --- #
  ENABLE_SLOCCOUNT(FOLDERS "${PROJECT_SOURCE_DIR}/${NAME_EXEC}/src" "${PROJECT_SOURCE_DIR}/${NAME_EXEC}/inc"
                        "${PROJECT_SOURCE_DIR}/${NAME_LIB}/src" "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc"
                        "${PROJECT_SOURCE_DIR}/test")
endif()

if(GENERATE_DOC)
  # --- Doxygen --- #
  GENERATE_DOXYGEN(SOURCES "${PROJECT_SOURCE_DIR}/${NAME_EXEC}/src" "${PROJECT_SOURCE_DIR}/${NAME_EXEC}/inc"
                        "${PROJECT_SOURCE_DIR}/${NAME_LIB}/src" "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc"
                        "${PROJECT_SOURCE_DIR}/test"
                        VERSION "${PROJECTVERSION}")
endif()
