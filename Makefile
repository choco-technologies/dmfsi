# #############################################################################
# 
# 	DMFSI - DMOD File System Interface
# 	This module defines file system interface that can be implemented by 
# 	multiple modules (e.g., FatFS, RamFS, FlashFS)
#
# #############################################################################

# Path to DMOD directory (can be overridden via command line or environment)
ifndef DMOD_DIR
# Check if DMOD_DIR is set in environment
ifdef ENV_DMOD_DIR
DMOD_DIR := $(ENV_DMOD_DIR)
else
# DMOD_DIR not provided, fetch DMOD automatically
$(info DMOD_DIR not set, fetching DMOD from git repository)

# Allow override of DMOD git repository and branch
DMOD_GIT_REPOSITORY ?= https://github.com/choco-technologies/dmod.git
DMOD_GIT_TAG ?= develop

# Set DMOD_DIR to a local directory where we'll fetch DMOD
DMOD_DIR := $(CURDIR)/.dmod

# Check if DMOD already exists
ifeq ($(wildcard $(DMOD_DIR)/paths.mk),)
# DMOD doesn't exist, clone it
$(info Cloning DMOD to $(DMOD_DIR)...)
$(shell git clone --depth 1 --branch $(DMOD_GIT_TAG) $(DMOD_GIT_REPOSITORY) $(DMOD_DIR))
$(info DMOD cloned successfully)

# Build DMOD to generate required configuration files
$(info Building DMOD system...)
$(shell cd $(DMOD_DIR) && $(MAKE) --no-print-directory)
$(info DMOD built successfully)
endif
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
