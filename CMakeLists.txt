cmake_minimum_required(VERSION 3.18)

# Path to DMOD directory (can be overridden via command line or environment)
if(NOT DEFINED DMOD_DIR)
    message(FATAL_ERROR "DMOD_DIR is not set. Please set it to the path of the DMOD repository")
endif()

# Include DMOD paths
include(${DMOD_DIR}/paths.cmake)

# Project configuration
project(dmod-fsi VERSION 1.0 LANGUAGES C)

set(DMOD_MODULE_NAME fsi)
set(DMOD_MODULE_VERSION "1.0")
set(DMOD_AUTHOR_NAME "DMOD FSI Team")
set(DMOD_STACK_SIZE 1024)
set(DMOD_PRIORITY 1)
set(DMOD_MANUAL_LOAD OFF)

dmod_add_library(${DMOD_MODULE_NAME} ${DMOD_MODULE_VERSION}
    src/fsi.c
)

# Make the interface available to other modules
target_include_directories(${DMOD_MODULE_NAME}_if 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Add examples subdirectory
add_subdirectory(examples/ramfs)
