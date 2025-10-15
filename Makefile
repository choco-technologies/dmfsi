# #############################################################################
# 
# 	FSI - File System Interface for DMOD
# 	This module defines file system interface that can be implemented by 
# 	multiple modules (e.g., FatFS, RamFS, FlashFS)
#
# #############################################################################

# Path to DMOD directory (can be overridden via command line or environment)
ifndef DMOD_DIR
$(error DMOD_DIR is not set. Please set it to the path of the DMOD repository)
endif

# -----------------------------------------------------------------------------
#  Paths initialization
# -----------------------------------------------------------------------------
include $(DMOD_DIR)/paths.mk

# -----------------------------------------------------------------------------
#   Module configuration
# -----------------------------------------------------------------------------

# The name of the module
DMOD_MODULE_NAME=fsi

# The version of the module
DMOD_MODULE_VERSION=1.0

# The name of the author
DMOD_AUTHOR_NAME=DMOD FSI Team

# The list of C sources
DMOD_CSOURCES=src/fsi.c

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
