cmake_minimum_required(VERSION 2.8)

project(PFA)

set(NAME_LIB "common")
set(NAME_CLIENT "client")
set(NAME_SERVER "server")
set(NAME_MASTERSERVER "masterServer")
set(NAME_NETWORKLIB "network")
set(TEST_EXEC_LIB "UnitTesterLib")
set(TEST_EXEC_CLIENT "UnitTesterClient")
set(TEST_EXEC_SERVER "UnitTesterServer")
set(PROTO_DIR "message")

set(INCLUDE_DIRECTORY "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc/"
					  "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/inc/"
					  "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/inc/"
						"${PROJECT_SOURCE_DIR}/${NAME_MASTERSERVER}/inc/"
						"${PROJECT_SOURCE_DIR}/${NAME_NETWORKLIB}/inc/")
set(SRC_DIRECTORY "${PROJECT_SOURCE_DIR}/${NAME_LIB}/src/"
				  "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/src/"
				  "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/src/"
					"${PROJECT_SOURCE_DIR}/${NAME_MASTERSERVER}/src/"
					"${PROJECT_SOURCE_DIR}/${NAME_NETWORKLIB}/src/")
set(DOC_PATH "${PROJECT_SOURCE_DIR}/doc")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(CppUnit)
find_package(Threads)
find_package(SFML 2.1 COMPONENTS system window graphics audio)
find_package(Boost 1.49.0 COMPONENTS system program_options filesystem)
find_package(Box2D)
find_package(Protobuf)
find_package(ENet)
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
			   "-Wno-shadow" "-Wno-builtin-macro-redefined"
			   "-Wmissing-declarations"
			   "-Wno-switch-enum"
		FLAGS_RELEASE  "-march=native" "-O2"
		FLAGS_DEBUG    "-DNDEBUG" "-g3"
			   )
elseif (MSVC)
  COMPILER_FLAGS(FLAGS "/wd4251" # needs to have dll-interface to be used by clients
			           "/wd4275" # exported class derived from non-exported class
								 "/EHsc" # activate C++ style exceptions
			   )
endif()

# --- user options ---
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not." OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Decide if a coverage report should be generated." ON "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(VALGRIND_CHECK "Decide if valgrind should be executed with the test binary to check for memory leaks." ON "BUILD_TESTS" OFF)
OPTION(CODE_METRICS "Decide if code's metrics should be generated." OFF)
OPTION(CODE_CHECK "Decide if code should be checked for common errors." OFF)
OPTION(GENERATE_DOC "Decide if documentation should be generated." OFF)

OPTION(BUILD_SERVER "Does the compilation will build the server ?." ON)
OPTION(BUILD_CLIENT "Does the compilation will build the client ?." ON)
OPTION(BUILD_MASTER_SERVER "Does the compilation will build the master server ?." ON)

# --- global definitions ---
DEFINE_PROJECT_VERSION(PROJECT 1 0 0 "snapshot")
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
	masterserver_source_files
	${NAME_MASTERSERVER}/src/*
)

file(
	GLOB_RECURSE
	masterserver_include_files
	${NAME_MASTERSERVER}/inc/*
)

file(
	GLOB_RECURSE
	networklib_source_files
	${NAME_NETWORKLIB}/src/*
)

file(
	GLOB_RECURSE
	networklib_include_files
	${NAME_NETWORKLIB}/inc/*
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${NAME_LIB}/inc/Config.h.in"
  "${NAME_LIB}/inc/Config.h"
)

#Generate message library
if(BUILD_CLIENT OR BUILD_SERVER OR BUILD_MASTER_SERVER)
  if(NOT Boost_FOUND OR NOT PROTOBUF_FOUND OR NOT ENET_FOUND OR NOT THREADS_FOUND)
   message(SEND_ERROR "Protobuf, Enet, Boost and Thread library are required to build the client, server and master server.")
  endif()

  add_subdirectory(${PROTO_DIR})

  add_library(
		${NAME_NETWORKLIB} STATIC
    ${PROTO_SRC}
    ${PROTO_HEADER}
		${networklib_source_files}
		${networklib_include_files}
  )

  set_target_properties(${NAME_NETWORKLIB} PROPERTIES LINKER_LANGUAGE CXX)
  set_source_files_properties(${PROTO_SRC} ${PROTO_HEADER} PROPERTIES GENERATED TRUE)
	include_directories("${CMAKE_BINARY_DIR}/${PROTO_DIR}")
	include_directories("${PROJECT_SOURCE_DIR}/${NAME_NETWORKLIB}/inc")
  add_dependencies(${NAME_NETWORKLIB} protobuf_compilation)

  include_directories(${Boost_INCLUDE_DIRS})
  include_directories(${ENet_INCLUDE_DIRS})

  target_link_libraries(${NAME_NETWORKLIB} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY})
  target_link_libraries(${NAME_NETWORKLIB} ${ENet_LIBRARIES})
  target_link_libraries(${NAME_NETWORKLIB} ${CMAKE_THREAD_LIBS_INIT})
endif()

#Common library
if(BUILD_CLIENT OR BUILD_SERVER)
  if(NOT BOX2D_FOUND)
      message(SEND_ERROR "Box2D is required to build the client or server.")
  endif()

  add_library(
  # executable name
    ${NAME_LIB} STATIC
  # source files
    ${lib_source_files}
    ${lib_include_files}
  )

  include_directories("${CMAKE_CURRENT_BINARY_DIR}/${NAME_LIB}/inc/")
  set_property(TARGET ${NAME_LIB} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

  include_directories(${BOX2D_INCLUDE_DIR})
  target_link_libraries (${NAME_LIB} ${BOX2D_LIBRARIES})
  target_link_libraries(${NAME_LIB} ${NAME_NETWORKLIB})
endif()

#Client Binary
if(BUILD_CLIENT)
  if (NOT SFML_FOUND)
    message(SEND_ERROR "SFML is required to build the client.")
  endif()

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

  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${NAME_CLIENT} ${SFML_LIBRARIES})
  target_link_libraries(${NAME_CLIENT} ${NAME_LIB})
endif()

#Server Binary
if(BUILD_SERVER)
  add_executable(
    ${NAME_SERVER}
  # source files
    ${server_source_files}
    ${server_include_files}
  )

  set_property(TARGET ${NAME_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/inc")
  set_property(TARGET ${NAME_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

  target_link_libraries(${NAME_SERVER} ${NAME_LIB})
endif()

#Master Server Binary
if(BUILD_MASTER_SERVER)
  if(NOT SQLITE3_FOUND)
    message(SEND_ERROR "SQLite3 is required to build the master server.")
  endif()

  add_executable(
        ${NAME_MASTERSERVER}
  # source files
	${masterserver_source_files}
	${masterserver_include_files}
  )

  set_property(TARGET ${NAME_MASTERSERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_MASTERSERVER}/inc")

  include_directories(${SQLITE3_INCLUDE_DIR})
  target_link_libraries(${NAME_MASTERSERVER} ${SQLITE3_LIBRARIES})
  target_link_libraries(${NAME_MASTERSERVER} ${NAME_NETWORKLIB})
endif()







###############
#Other Targets#
###############


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
