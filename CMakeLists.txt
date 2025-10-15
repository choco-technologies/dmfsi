cmake_minimum_required(VERSION 3.18)

# DMOD_DIR must be set (either via command line or environment variable)
if(NOT DEFINED DMOD_DIR)
    if(DEFINED ENV{DMOD_DIR})
        set(DMOD_DIR $ENV{DMOD_DIR})
    else()
        message(FATAL_ERROR "DMOD_DIR is not set. Please set it to the path of the DMOD repository")
    endif()
endif()

set(DMOD_MODE "DMOD_MODULE" CACHE STRING "Mode" FORCE)
set(DMOD_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

include(${DMOD_DIR}/paths.cmake)

project(dmod_fsi)

dmod_setup_external_module()

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

# Optionally build examples
if(DMOD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
