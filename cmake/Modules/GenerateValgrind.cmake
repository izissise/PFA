# - Generate a valgrind documentation for a project.
# The function GENERATE_VALGRIND is provided to create a "doc" target that
# generates a valgrind documentation (currently only as HTML report).
#
# GENERATE_VALGRIND(TARGETS "Binary to execute in valgrind")
#
#     TARGETS: The binaries to execute 
#
# This function can always be called, even if no valgrind was found. Then no
# target is created.
#
# Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

GET_FILENAME_COMPONENT(GENERATE_VALGRIND_MODULE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

INCLUDE(ParseArguments)
FIND_PACKAGE(Valgrind)

FUNCTION(GENERATE_VALGRIND)
    IF(VALGRIND_FOUND)
        # argument parsing
        PARSE_ARGUMENTS(ARG "TARGETS" "" ${ARGN})

        foreach(target ${ARG_TARGETS})
          SET(VALGRIND_REPORT_FILE${target} "${target}-valgrind.xml")
          ADD_CUSTOM_COMMAND(OUTPUT "${VALGRIND_REPORT_FILE${target}}"
                             DEPENDS ${target}
                             COMMAND ${VALGRIND_EXECUTABLE} ARGS --xml=yes "--xml-file=${VALGRIND_REPORT_FILE${target}}" ${target}
                             WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
          list(APPEND deptarget "${VALGRIND_REPORT_FILE${target}}")
        endforeach()
        ADD_CUSTOM_TARGET(valgrind DEPENDS ${deptarget} VERBATIM)
        
        MESSAGE(STATUS "Generated valgrind target (valgrind).")
    
    ENDIF()

ENDFUNCTION()
