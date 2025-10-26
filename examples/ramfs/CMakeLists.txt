cmake_minimum_required(VERSION 3.18)

set(DMOD_MODULE_NAME ramfs)
set(DMOD_MODULE_VERSION "1.0")
set(DMOD_AUTHOR_NAME "DMOD FSI Team")
set(DMOD_STACK_SIZE 1024)
set(DMOD_PRIORITY 1)
set(DMOD_MANUAL_LOAD OFF)

# Declare that this module implements the FSI interface
set(DMOD_DIF_IMPLS fsi)

dmod_add_library(${DMOD_MODULE_NAME} ${DMOD_MODULE_VERSION}
    ramfs.c
)

# Link to FSI interface
target_link_libraries(${DMOD_MODULE_NAME} fsi_if)
