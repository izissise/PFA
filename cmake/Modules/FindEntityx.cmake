# Try to find entityx library
# Once done this will define
#
# ENTITYX_FOUND - true if system found entityx library
# ENTITYX_INCLUDE_DIRS - the entityx include directories
# ENTITYX_LIBRARIES - the libraries needed to use entityx
#
# $ENTITYX_ROOT is an environment variable that can be used to find entityx
#


FIND_PATH(ENTITYX_INCLUDE_DIRS	entityx/entityx.h
				PATHS
				${ENV}${ENTITYX_ROOT}
				/usr/local
				/usr
				PATH_SUFFIXES include
				)

FIND_LIBRARY(ENTITYX_LIBRARIES	NAMES entityx
				PATHS
				${ENV}${ENTITYX_ROOT}
				/usr/local
				/usr
				PATH_SUFFIXES lib
				)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Entityx DEFAULT_MSG ENTITYX_LIBRARIES ENTITYX_INCLUDE_DIRS)

IF(NOT ENTITYX_FOUND)
  MESSAGE(STATUS "Entityx not found")
ENDIF()

MARK_AS_ADVANCED(ENTITYX_LIBRARIES ENTITYX_INCLUDE_DIRS)