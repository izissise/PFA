# - Generate a cccc documentation for a project.
# The function GENERATE_CCCC is provided to create a "cccc" target that
# performs static code analysis using the cccc utility program.
#
# GENERATE_CCCC(SOURCES <sources to check...>
#                   [SUPPRESSION_FILE <file>]
#                   [ENABLE_IDS <id...>]
#                   [TARGET_NAME <name>]
#                   [INCLUDES <dir...>]
#                   [INLINE_SUPPRESSION])
#
# Generates a target "cccc" that executes cccc on the specified sources.
# Sources may either be file names or directories containing files where all
# C++ files will be parsed automatically. Use directories whenever possible
# because there is a limitation in arguments to pass to the cccc binary.
# SUPPRESSION_FILE may be give additionally to specify suppressions for#
# cccc. The sources mentioned in the suppression file must be in the same
# format like given for SOURCES. This means if you specified them relative to
# CMAKE_CURRENT_SOURCE_DIR, then the same relative paths must be used in the
# suppression file.
# ENABLE_IDS allows to specify which additional cccc check ids to execute,
# e.g. all or style. They are combined with AND.
# With TARGET_NAME a different name for the generated check target can be
# specified. This is useful if several calles to this function are made in one
# CMake project, as otherwise the target names collide.
# Additional include directories for the cccc program can be given with
# INCLUDES.
# If INLINE_SUPPRESSION is set, cccc inline-suppression comments are parsed.
#
# cccc will be executed with CMAKE_CURRENT_SOURCE_DIR as working directory.
#
# This function can always be called, even if no cccc was found. Then no
# target is created.
#
# Copyright (C) 2011 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
#
# This file may be licensed under the terms of the
# GNU Lesser General Public License Version 3 (the ``LGPL''),
# or (at your option) any later version.
#
# Software distributed under the License is distributed
# on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
# express or implied. See the LGPL for the specific language
# governing rights and limitations.
#
# You should have received a copy of the LGPL along with this
# program. If not, go to http://www.gnu.org/licenses/lgpl.html
# or write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The development of this software was supported by:
#   CoR-Lab, Research Institute for Cognition and Robotics
#     Bielefeld University

GET_FILENAME_COMPONENT(GENERATE_CCCC_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

FIND_PACKAGE(Cccc)

FUNCTION(GENERATE_CCCC)

    IF(CCCC_FOUND)
    
        PARSE_ARGUMENTS(ARG "SOURCES" ${ARGN})
        
        SET(TARGET_NAME "cccc")
        SET(TARGET_NAME_SUFFIX "")
        # parse target name
        LIST(LENGTH ARG_TARGET_NAME TARGET_NAME_LENGTH)
        IF(${TARGET_NAME_LENGTH} EQUAL 1)
            SET(TARGET_NAME ${ARG_TARGET_NAME})
            SET(TARGET_NAME_SUFFIX "-${ARG_TARGET_NAME}")
        ENDIF()
        
        SET(CCCC_CHECKFILE "${CMAKE_BINARY_DIR}/cccc-files${TARGET_NAME_SUFFIX}")
        SET(CCCC_REPORT_FILE "${CMAKE_BINARY_DIR}/cccc-report${TARGET_NAME_SUFFIX}.xml")
        SET(CCCC_WRAPPER_SCRIPT "${CMAKE_BINARY_DIR}/cccc${TARGET_NAME_SUFFIX}.cmake")
    
        # write a list file containing all sources to check for the call to
        # cccc
        SET(SOURCE_ARGS "")
        FOREACH(SOURCE ${ARG_SOURCES})
            SET(SOURCE_ARGS "${SOURCE_ARGS} \"${SOURCE}\"")
        ENDFOREACH()
        
        # prepare a cmake wrapper to write the stderr output of cccc to
        # the result file
        
        FILE(WRITE ${CCCC_WRAPPER_SCRIPT}
"
EXECUTE_PROCESS(COMMAND \"${CCCC_EXECUTABLE}\" ${SOURCE_ARGS}
                RESULT_VARIABLE CCCC_EXIT_CODE
                ERROR_VARIABLE ERROR_OUT
                WORKING_DIRECTORY \"${CMAKE_CURRENT_SOURCE_DIR}\")
IF(NOT CCCC_EXIT_CODE EQUAL 0)
    MESSAGE(FATAL_ERROR \"Error executing cccc for target ${TARGET}, return code: \${CCCC_EXIT_CODE}\")
ENDIF()
IF(ERROR_OUT)
    MESSAGE(\"Detected errors:\\n\${ERROR_OUT}\")
ENDIF()
FILE(WRITE \"${CCCC_REPORT_FILE}\" \"\${ERROR_OUT}\")
"
            )
            
        ADD_CUSTOM_TARGET(${TARGET_NAME} ${CMAKE_COMMAND} -P "${CCCC_WRAPPER_SCRIPT}"
                          COMMENT "Generating cccc result ${TARGET_NAME}")
                          
        MESSAGE(STATUS "Generating cccc target with name ${TARGET_NAME}")

    ENDIF()

ENDFUNCTION()
