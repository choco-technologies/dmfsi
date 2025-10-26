# #############################################################################
# 
# 	RamFS - Simple RAM-based File System
# 	Example implementation of DMFSI interface
#
# #############################################################################

# Path to DMOD directory (can be overridden via command line or environment)
ifndef DMOD_DIR
$(error DMOD_DIR is not set. Please set it to the path of the DMOD repository)
endif

# Path to DMFSI module
DMFSI_DIR=../..

# -----------------------------------------------------------------------------
#  Paths initialization
# -----------------------------------------------------------------------------
include $(DMOD_DIR)/paths.mk

# -----------------------------------------------------------------------------
#   Module configuration
# -----------------------------------------------------------------------------

# The name of the module
DMOD_MODULE_NAME=ramfs

# The version of the module
DMOD_MODULE_VERSION=1.0

# The name of the author
DMOD_AUTHOR_NAME=DMOD DMFSI Team

# The list of C sources
DMOD_CSOURCES=ramfs.c

# The list of C++ sources
DMOD_CXXSOURCES=

# The list of include directories
DMOD_INC_DIRS=$(DMFSI_DIR)/inc

# The list of libraries to link
DMOD_LIBS=

# The list of definitions
DMOD_DEFINITIONS=

# -----------------------------------------------------------------------------
#   List of MAL interfaces implemented by the module
# -----------------------------------------------------------------------------
DMOD_MAL_IMPLS=

# -----------------------------------------------------------------------------
#   List of DIF interfaces implemented by the module
# -----------------------------------------------------------------------------
DMOD_DIF_IMPLS=dmfsi

# -----------------------------------------------------------------------------
#   Include the dmod lib makefile
# -----------------------------------------------------------------------------
include $(DMOD_DMF_LIB_FILE_PATH)
