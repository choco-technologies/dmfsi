# #############################################################################
# 
# 	DMFSI - DMOD File System Interface
# 	This module defines file system interface that can be implemented by 
# 	multiple modules (e.g., FatFS, RamFS, FlashFS)
#
# #############################################################################

# Path to DMOD directory (can be overridden via command line or environment)
ifndef DMOD_DIR
# DMOD_DIR not provided, will fetch DMOD automatically
DMOD_DIR := $(CURDIR)/.dmod
AUTO_FETCH_DMOD := 1
endif

# Allow override of DMOD git repository and branch (for auto-fetch)
DMOD_GIT_REPOSITORY ?= https://github.com/choco-technologies/dmod.git
DMOD_GIT_TAG ?= develop

# Auto-fetch DMOD if needed
ifdef AUTO_FETCH_DMOD
ifeq ($(wildcard $(DMOD_DIR)/paths.mk),)
$(info DMOD_DIR not set, fetching DMOD from git repository)
$(info Cloning DMOD to $(DMOD_DIR)...)
DMOD_CLONE_OUTPUT := $(shell git clone --depth 1 --branch $(DMOD_GIT_TAG) $(DMOD_GIT_REPOSITORY) $(DMOD_DIR) 2>&1)
ifeq ($(wildcard $(DMOD_DIR)/paths.mk),)
$(error Failed to clone DMOD: $(DMOD_CLONE_OUTPUT))
endif
$(info Building DMOD system...)
DMOD_BUILD_OUTPUT := $(shell cd $(DMOD_DIR) && $(MAKE) --no-print-directory 2>&1)
ifeq ($(wildcard $(DMOD_DIR)/build/dmod-config.h),)
$(error Failed to build DMOD: $(DMOD_BUILD_OUTPUT))
endif
$(info DMOD fetched and built successfully)
endif
endif

# -----------------------------------------------------------------------------
#  Paths initialization
# -----------------------------------------------------------------------------
include $(DMOD_DIR)/paths.mk

# -----------------------------------------------------------------------------
#   Module configuration
# -----------------------------------------------------------------------------

# The name of the module
DMOD_MODULE_NAME=dmfsi

# The version of the module
DMOD_MODULE_VERSION=1.0

# The name of the author
DMOD_AUTHOR_NAME=DMOD DMFSI Team

# The list of C sources
DMOD_CSOURCES=src/dmfsi.c

# The list of C++ sources
DMOD_CXXSOURCES=

# The list of include directories
DMOD_INC_DIRS=inc

# The list of libraries to link
DMOD_LIBS=

# The list of definitions
DMOD_DEFINITIONS=

# -----------------------------------------------------------------------------
#   List of MAL interfaces implemented by the module
# -----------------------------------------------------------------------------
DMOD_MAL_IMPLS=

# -----------------------------------------------------------------------------
#   List of DIF interfaces defined by the module
# -----------------------------------------------------------------------------
DMOD_DIF_IMPLS=

# -----------------------------------------------------------------------------
#   Include the dmod lib makefile
# -----------------------------------------------------------------------------
include $(DMOD_DMF_LIB_FILE_PATH)
