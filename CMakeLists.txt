cmake_minimum_required(VERSION 2.8)

project(PFA)

set(NAME_LIB "common")
set(NAME_CLIENT "client")
set(NAME_SERVER "server")
set(NAME_MASTERSERVER "masterServer")
set(TEST_EXEC_LIB "UnitTesterLib")
set(TEST_EXEC_CLIENT "UnitTesterClient")
set(TEST_EXEC_SERVER "UnitTesterServer")
set(PROTO_DIR "message")
set(PROTO_SRCS "")
set(PROTO_HDRS "")

set(INCLUDE_DIRECTORY "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc/"
					  "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/inc/"
					  "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/inc/"
						"${PROJECT_SOURCE_DIR}/${NAME_MASTERSERVER}/inc/")
set(SRC_DIRECTORY "${PROJECT_SOURCE_DIR}/${NAME_LIB}/src/"
				  "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/src/"
				  "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/src/"
					"${PROJECT_SOURCE_DIR}/${NAME_MASTERSERVER}/src/")
set(DOC_PATH "${PROJECT_SOURCE_DIR}/doc")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(CppUnit)
find_package(Threads)
find_package(SFML 2.1 COMPONENTS system window graphics audio)
find_package(Boost 1.55.0 COMPONENTS system program_options REQUIRED)
find_package(Box2D REQUIRED)
find_package(ENet)
find_package(Protobuf)
find_package(Sqlite3)

INCLUDE(CMakeDependentOption)
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
INCLUDE(DemangleCppABI)
INCLUDE(CheckApplyCompilerFlags)
# --- global compiler flags ---
if (UNIX)
  COMPILER_FLAGS(FLAGS     "-pipe" "-Wall" "-Wextra"
			   "--std=c++1y"
			   "-Wchar-subscripts" "-Winit-self" "-Wswitch-enum"
			   "-Wstrict-aliasing=3"
			   "-Wundef" "-Wno-endif-labels"
			   "-Wshadow" "-Wno-builtin-macro-redefined"
			   "-Wmissing-declarations"
			   "-Wno-switch-enum"
		FLAGS_RELEASE  "-march=native" "-O2"
		FLAGS_DEBUG    "-DNDEBUG" "-g3"
			   )
elseif (MSVC)
  COMPILER_FLAGS(FLAGS "/wd4251" # needs to have dll-interface to be used by clients
			   "/wd4275" # exported class derived from non-exported class
			   )
endif()

# --- user options ---
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not." OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Decide if a coverage report should be generated." ON "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(VALGRIND_CHECK "Decide if valgrind should be executed with the test binary to check for memory leaks." ON "BUILD_TESTS" OFF)
OPTION(CODE_METRICS "Decide if code's metrics should be generated." OFF)
OPTION(CODE_CHECK "Decide if code should be checked for common errors." OFF)
OPTION(GENERATE_DOC "Decide if documentation should be generated." OFF)

# --- global definitions ---
DEFINE_PROJECT_VERSION(PROJECT 0 0 1 "snapshot")
CURRENT_DATE(PROJECT_BUILD_DATE)

# All source files using globs
file(
  GLOB_RECURSE
  lib_source_files
  ${NAME_LIB}/src/*
)

file(
  GLOB_RECURSE
  lib_include_files
  ${NAME_LIB}/inc/*
)

file(
  GLOB_RECURSE
  client_source_files
  ${NAME_CLIENT}/src/*
)

file(
  GLOB_RECURSE
  client_include_files
  ${NAME_CLIENT}/inc/*
  )

file(
  GLOB_RECURSE
  server_source_files
  ${NAME_SERVER}/src/*
)

file(
  GLOB_RECURSE
  server_include_files
  ${NAME_SERVER}/inc/*
)

file(
  GLOB_RECURSE
  proto_files
  ${PROTO_DIR}/proto/*.proto
)

file(
	GLOB_RECURSE
	masterserver_source_files
	${NAME_MASTERSERVER}/src/*
)

file(
	GLOB_RECURSE
	masterserver_include_files
	${NAME_MASTERSERVER}/inc/*
)


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${NAME_LIB}/inc/Config.h.in"
  "${NAME_LIB}/inc/Config.h"
)

if(PROTOBUF_FOUND)

  execute_process( COMMAND protoc --proto_path "${PROJECT_SOURCE_DIR}/${PROTO_DIR}/proto" --cpp_out "${PROJECT_SOURCE_DIR}/${PROTO_DIR}/cpp" ${proto_files} )

  file(
  GLOB_RECURSE
  PROTO_SRCS
  ${PROTO_DIR}/cpp/*.cc
  )

  foreach(source_file ${PROTO_SRCS})
    set_source_files_properties(${source_file} PROPERTIES COMPILE_FLAGS -Wno-shadow)
  endforeach(source_file)

  file(
  GLOB_RECURSE
  PROTO_HDRS
  ${PROTO_DIR}/cpp/*.h
  )

  include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/${PROTO_DIR}/cpp/")

endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}/${NAME_LIB}/inc/")

add_library(
# executable name
  ${NAME_LIB} STATIC
# source files
  ${lib_source_files}
  ${lib_include_files}
  ${PROTO_SRCS}
  ${PROTO_HDRS}
)

set_property(TARGET ${NAME_LIB} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

add_executable(
# executable name
  ${NAME_CLIENT}
# source files
  ${client_source_files}
  ${client_include_files}
)

set_property(TARGET ${NAME_CLIENT} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/inc")
set_property(TARGET ${NAME_CLIENT} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/inc/Gui")
set_property(TARGET ${NAME_CLIENT} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

add_executable(
  ${NAME_SERVER}
# source files
  ${server_source_files}
  ${server_include_files}
)

set_property(TARGET ${NAME_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/inc")
set_property(TARGET ${NAME_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

add_executable(
	${NAME_MASTERSERVER}
# source files
	${masterserver_source_files}
	${masterserver_include_files}
)

set_property(TARGET ${NAME_MASTERSERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_MASTERSERVER}/inc")
set_property(TARGET ${NAME_MASTERSERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

target_link_libraries(${NAME_CLIENT} ${NAME_LIB})
target_link_libraries(${NAME_SERVER} ${NAME_LIB})
target_link_libraries(${NAME_MASTERSERVER} ${NAME_LIB})

if (THREADS_FOUND)
  target_link_libraries (${NAME_LIB} ${CMAKE_THREAD_LIBS_INIT})
endif()

if (SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries (${NAME_CLIENT} ${SFML_LIBRARIES})
endif()

if (Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
   target_link_libraries (${NAME_LIB} ${Boost_LIBRARIES})
endif()

if (BOX2D_FOUND)
   include(${BOX2D_INCLUDE_DIR})
   target_link_libraries (${NAME_LIB} ${BOX2D_LIBRARIES})
endif()

if (ENET_FOUND)
   include_directories(${ENet_INCLUDE_DIRS})
   target_link_libraries(${NAME_LIB} ${ENet_LIBRARIES})
endif()

if (PROTOBUF_FOUND)
   include_directories(${PROTOBUF_INCLUDE_DIRS})
   target_link_libraries(${NAME_LIB} ${PROTOBUF_LIBRARIES})
endif()

if (SQLITE3_FOUND)
  include_directories(${SQLITE3_INCLUDE_DIR})
  target_link_libraries (${NAME_MASTERSERVER} ${SQLITE3_LIBRARIES})
endif()

# --- enable unit tests if desired ---
if(BUILD_TESTS)
  if(NOT CPPUNIT_FOUND)
    MESSAGE(WARNING "CppUnit library can't be found, Unit test won't be compiled")
  else()
    file(
      GLOB_RECURSE
      test_files_lib
      test/common/*
    )

    file(
      GLOB_RECURSE
      test_files_client
      test/client/*
    )

    file(
      GLOB_RECURSE
      test_files_server
      test/server/*
    )

   # list(REMOVE_ITEM ALL_SRCS ${ui} ${f}.cpp ${f}Driver.cpp)

    add_executable(
      ${TEST_EXEC_LIB}
      ${test_files_lib}
    )
    set_property(TARGET ${TEST_EXEC_LIB} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

    set(client_source_files_test ${client_source_files})
    list(REMOVE_ITEM client_source_files_test "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/src/main.cpp")
    add_executable(
      ${TEST_EXEC_CLIENT}
      ${test_files_client} ${client_source_files_test}
      ${client_include_files}
    )

    set_property(TARGET ${TEST_EXEC_CLIENT} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")
    set_property(TARGET ${TEST_EXEC_CLIENT} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/inc")

    set(server_source_files_test ${server_source_files})
    list(REMOVE_ITEM server_source_files_test "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/src/main.cpp")
    add_executable(
      ${TEST_EXEC_SERVER}
      ${test_files_server} ${server_source_files_test}
      ${server_include_files}
    )
    set_property(TARGET ${TEST_EXEC_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")
    set_property(TARGET ${TEST_EXEC_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/inc")

    if(BUILD_TESTS_COVERAGE)
      # --- coverage report ---
      ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXEC_LIB} ${TEST_EXEC_CLIENT} ${TEST_EXEC_SERVER} TESTS ${test_files})
    endif()

    if(VALGRIND_CHECK)
      # --- Valgrind --- #
      GENERATE_VALGRIND(TARGETS "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC_LIB}" "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC_CLIENT}" "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC_SERVER}")
    endif()

    set(TEST_REPORT_FILE "cpptestresults.xml")
    # Execute ${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC} binary to execute tests
    ADD_CUSTOM_COMMAND(OUTPUT "${TEST_EXEC_LIB}-${TEST_REPORT_FILE}"
                           DEPENDS ${TEST_EXEC_LIB}
                           COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC_LIB}" ARGS "--xml-file=${TEST_EXEC_LIB}-${TEST_REPORT_FILE}"
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    ADD_CUSTOM_COMMAND(OUTPUT "${TEST_EXEC_CLIENT}-${TEST_REPORT_FILE}"
                           DEPENDS ${TEST_EXEC_CLIENT}
                           COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC_CLIENT}" ARGS "--xml-file=${TEST_EXEC_CLIENT}-${TEST_REPORT_FILE}"
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    ADD_CUSTOM_COMMAND(OUTPUT "${TEST_EXEC_SERVER}-${TEST_REPORT_FILE}"
                           DEPENDS ${TEST_EXEC_SERVER}
                           COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC_SERVER}" ARGS "--xml-file=${TEST_EXEC_SERVER}-${TEST_REPORT_FILE}"
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    ADD_CUSTOM_TARGET(launchtest DEPENDS "${TEST_EXEC_LIB}-${TEST_REPORT_FILE}" "${TEST_EXEC_CLIENT}-${TEST_REPORT_FILE}" "${TEST_EXEC_SERVER}-${TEST_REPORT_FILE}")

    include_directories(${CPPUNIT_INCLUDE_DIRS})
    target_link_libraries(${TEST_EXEC_SERVER} ${CPPUNIT_LIBRARY} ${NAME_LIB})
    target_link_libraries(${TEST_EXEC_CLIENT} ${CPPUNIT_LIBRARY} ${NAME_LIB})
    target_link_libraries(${TEST_EXEC_LIB} ${CPPUNIT_LIBRARY} ${NAME_LIB})

    target_link_libraries(${TEST_EXEC_CLIENT} ${NAME_LIB})
    target_link_libraries(${TEST_EXEC_SERVER} ${NAME_LIB})

    if (SFML_FOUND)
      include_directories(${SFML_INCLUDE_DIR})
      target_link_libraries (${TEST_EXEC_CLIENT} ${SFML_LIBRARIES})
    endif()

  endif()
endif()

#Extra targets:
if(CODE_CHECK)
  # --- cppcheck ---
  GENERATE_CPPCHECK(SOURCES ${SRC_DIRECTORY}
                            "${PROJECT_SOURCE_DIR}/test"
                            ENABLE_IDS style
                            INLINE_SUPPRESSION)
endif()

if(CODE_METRICS)
  # --- cccc ---
  GENERATE_CCCC(SOURCES ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test")
  # --- SlocCount --- #
  ENABLE_SLOCCOUNT(FOLDERS ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test")
endif()

if(GENERATE_DOC)
  # --- Doxygen --- #
  GENERATE_DOXYGEN(SOURCES ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test" ${DOC_PATH}
                        VERSION "${PROJECTVERSION}")
endif()

#SET_TARGET_PROPERTIES(${NAME_CLIENT} PROPERTIES COMPILE_FLAGS "-pg" LINK_FLAGS "-pg")
