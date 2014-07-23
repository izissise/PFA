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
