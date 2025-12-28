cmake_minimum_required(VERSION 3.18)

# DMOD_DIR handling: use provided path, environment variable, or fetch from git
if(NOT DEFINED DMOD_DIR)
    if(DEFINED ENV{DMOD_DIR})
        set(DMOD_DIR $ENV{DMOD_DIR})
    else()
        # DMOD_DIR not provided, fetch DMOD using FetchContent
        message(STATUS "DMOD_DIR not set, fetching DMOD from git repository")
        
        include(FetchContent)
        
        # Allow override of DMOD git repository and branch
        if(NOT DEFINED DMOD_GIT_REPOSITORY)
            set(DMOD_GIT_REPOSITORY "https://github.com/choco-technologies/dmod.git")
        endif()
        
        if(NOT DEFINED DMOD_GIT_TAG)
            set(DMOD_GIT_TAG "develop")
        endif()
        
        FetchContent_Declare(
            dmod
            GIT_REPOSITORY ${DMOD_GIT_REPOSITORY}
            GIT_TAG        ${DMOD_GIT_TAG}
        )
        
        FetchContent_MakeAvailable(dmod)
        
        # Set DMOD_DIR to the fetched source directory
        FetchContent_GetProperties(dmod)
        if(NOT dmod_POPULATED)
            FetchContent_Populate(dmod)
        endif()
        set(DMOD_DIR ${dmod_SOURCE_DIR})
        
        message(STATUS "DMOD fetched to: ${DMOD_DIR}")
    endif()
endif()

# Allow DMOD_MODE to be set via command line or default to DMOD_MODULE
if(NOT DEFINED DMOD_MODE)
    set(DMOD_MODE "DMOD_MODULE" CACHE STRING "DMOD build mode (DMOD_MODULE or DMOD_SYSTEM)" FORCE)
else()
    set(DMOD_MODE "${DMOD_MODE}" CACHE STRING "DMOD build mode (DMOD_MODULE or DMOD_SYSTEM)" FORCE)
endif()

set(DMOD_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")

include(${DMOD_DIR}/paths.cmake)

project(dmfsi)

dmod_setup_external_module()

set(DMOD_MODULE_NAME dmfsi)
set(DMOD_MODULE_VERSION "1.0")
set(DMOD_AUTHOR_NAME "DMOD DMFSI Team")
set(DMOD_STACK_SIZE 1024)
set(DMOD_PRIORITY 1)
set(DMOD_MANUAL_LOAD OFF)

if(DMOD_SYSTEM)
    # In DMOD_SYSTEM mode, build as a regular static library
    add_library(${DMOD_MODULE_NAME} STATIC
        src/dmfsi.c
    )
    
    # Create interface library for consistency with MODULE mode
    add_library(${DMOD_MODULE_NAME}_if INTERFACE)
    
    target_include_directories(${DMOD_MODULE_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/inc
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    target_include_directories(${DMOD_MODULE_NAME}_if
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/inc
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    target_link_libraries(${DMOD_MODULE_NAME} PUBLIC dmod)
    target_link_libraries(${DMOD_MODULE_NAME}_if INTERFACE ${DMOD_MODULE_NAME})
    
    # Generate the _defs.h file for interface definitions
    to_snake_case(${DMOD_MODULE_NAME} DMOD_MODULE_NAME_SNAKE_CASE)
    set(DMOD_MODULE_TYPE "Library")
    configure_file(${DMOD_SCRIPTS_DIR}/api.h.in ${CMAKE_CURRENT_BINARY_DIR}/${DMOD_MODULE_NAME_SNAKE_CASE}_defs.h)
    
else()
    # In DMOD_MODULE mode, build as a DMF module using dmod_add_library
    dmod_add_library(${DMOD_MODULE_NAME} ${DMOD_MODULE_VERSION}
        src/dmfsi.c
    )
    
    # Make the interface available to other modules
    target_include_directories(${DMOD_MODULE_NAME}_if 
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/inc
    )
endif()

# Optionally build examples
if(DMOD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
