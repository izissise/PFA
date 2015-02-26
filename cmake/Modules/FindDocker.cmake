# - Try to find the DOCKER utility
# Will define:
#
# DOCKER_EXECUTABLE - DOCKER executable
# DOCKER_VERSION - version of DOCKER
#
# Possible hints:
# DOCKER_ROOT - root directory of DOCKER installation
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

INCLUDE(FindPackageHandleStandardArgs)

# find server executable
FIND_PROGRAM(VALGRIND_EXECUTABLE docker
             HINTS ${DOCKER_ROOT}/bin
                   ${DOCKER_ROOT})

# get the version
SET(DOCKER_VERSION "")
IF(DOCKER_EXECUTABLE)
    EXECUTE_PROCESS(COMMAND ${DOCKER_EXECUTABLE} --version OUTPUT_VARIABLE DOCKER_VERSION_OUT)
    STRING(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" DOCKER_VERSION "${DOCKER_VERSION_OUT}")
ENDIF()

FIND_PACKAGE_HANDLE_STANDARD_ARGS(DOCKER DEFAULT_MSG DOCKER_EXECUTABLE DOCKER_VERSION)

# only visible in advanced view
MARK_AS_ADVANCED(DOCKER_EXECUTABLE DOCKER_VERSION)
