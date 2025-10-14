cmake_minimum_required(VERSION 3.18)

set(DMOD_MODULE_NAME ramfs)
set(DMOD_MODULE_VERSION "1.0")
set(DMOD_AUTHOR_NAME "DMOD FSI Team")
set(DMOD_STACK_SIZE 2048)
set(DMOD_PRIORITY 1)
set(DMOD_MANUAL_LOAD OFF)

dmod_add_library(${DMOD_MODULE_NAME} ${DMOD_MODULE_VERSION}
    ramfs.c
)

# Link against FSI interface
target_include_directories(${DMOD_MODULE_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
)
