cmake_minimum_required(VERSION 3.18)

set(DMOD_MODULE_NAME ramfs)
set(DMOD_MODULE_VERSION "1.0")
set(DMOD_AUTHOR_NAME "DMOD FSI Team")
set(DMOD_STACK_SIZE 1024)
set(DMOD_PRIORITY 1)
set(DMOD_MANUAL_LOAD OFF)

# Declare that this module implements the FSI interface
set(DMOD_DIF_IMPLS fsi)

if(DMOD_SYSTEM)
    # In DMOD_SYSTEM mode, build as a regular static library
    add_library(${DMOD_MODULE_NAME} STATIC
        ramfs.c
    )
    
    # Create interface library for consistency with MODULE mode
    add_library(${DMOD_MODULE_NAME}_if INTERFACE)
    
    target_include_directories(${DMOD_MODULE_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    target_include_directories(${DMOD_MODULE_NAME}_if
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    target_link_libraries(${DMOD_MODULE_NAME} PUBLIC dmod fsi_if)
    target_link_libraries(${DMOD_MODULE_NAME}_if INTERFACE ${DMOD_MODULE_NAME})
    
    # Generate the _defs.h file for interface definitions
    to_snake_case(${DMOD_MODULE_NAME} DMOD_MODULE_NAME_SNAKE_CASE)
    set(DMOD_MODULE_TYPE "Library")
    configure_file(${DMOD_SCRIPTS_DIR}/api.h.in ${CMAKE_CURRENT_BINARY_DIR}/${DMOD_MODULE_NAME_SNAKE_CASE}_defs.h)
    
    # Add DIF implementation definitions
    foreach(DIF ${DMOD_DIF_IMPLS})
        target_compile_definitions(${DMOD_MODULE_NAME}
            PRIVATE
                DMOD_DIF_${DIF}
        )
    endforeach()
    
else()
    # In DMOD_MODULE mode, build as a DMF module using dmod_add_library
    dmod_add_library(${DMOD_MODULE_NAME} ${DMOD_MODULE_VERSION}
        ramfs.c
    )
    
    # Link to FSI interface
    target_link_libraries(${DMOD_MODULE_NAME} fsi_if)
endif()
