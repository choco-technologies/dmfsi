# dmod-fsi - File System Interface for DMOD

This repository defines a DMOD Interface (DIF) for file systems. It provides a standardized interface that can be implemented by various file system modules (e.g., FatFS, RamFS, FlashFS) to enable virtual file system (VFS) mounting in DMOD-based systems.

## Overview

The FSI (File System Interface) defines a comprehensive set of file system operations based on POSIX-like semantics. This interface allows different file system implementations to be used interchangeably through the DMOD dynamic module system.

## Features

The interface includes:
- **File operations**: open, close, read, write, seek, flush
- **Character I/O**: getc, putc
- **File information**: size, tell, eof, error
- **Directory operations**: opendir, closedir, readdir
- **File management**: stat, unlink, rename, chmod, utime
- **Directory management**: mkdir, direxists
- **Initialization**: init, deinit

## Building

### Prerequisites

- DMOD library (https://github.com/choco-technologies/dmod.git)
- GNU Make or CMake (version 3.18 or higher)
- GCC or compatible C compiler

### Build with CMake

DMOD modules can be built using CMake:

```bash
# First, build DMOD with CMake
cd /path/to/dmod
cmake -B build -DDMOD_MODE=DMOD_SYSTEM
cmake --build build

# Set DMOD_DIR to point to your DMOD installation
export DMOD_DIR=/path/to/dmod

# Build the FSI interface module with examples
cd /path/to/dmod-fsi
cmake -B build -DDMOD_BUILD_EXAMPLES=ON
cmake --build build
```

### Build with Make

DMOD modules can also be built using the DMOD build system with Make:

```bash
# First, build DMOD
cd /path/to/dmod
make

# Set DMOD_DIR to point to your DMOD installation
export DMOD_DIR=/path/to/dmod

# Build the FSI interface module
cd /path/to/dmod-fsi
make

# Build the RamFS example
cd examples/ramfs
make
```

The build process will generate DMF (DMOD Module Format) files that can be dynamically loaded by DMOD.

**Note**: This module was structured following DMOD's module conventions. To create new DMOD modules, use the `new-module.sh` script from the DMOD repository (available in `scripts/new-module.sh` on the develop branch).

## Interface Definition

The interface is defined in `inc/fsi.h`. All operations return integer status codes (FSI_OK on success, negative error codes on failure).

### Example Implementation

The `examples/ramfs` directory contains a simple RAM-based file system implementation demonstrating how to implement the FSI interface. This serves as a reference for creating new file system modules.

## Usage

To implement a new file system:

1. Create a new module directory
2. Include `fsi.h` in your implementation
3. Implement the DIF functions using `dmod_fsi_dif_api_declaration` macro
4. Create Makefile and CMakeLists.txt based on the examples
5. Build your module as a DMOD DMF file

## File Structure

```
dmod-fsi/
├── inc/
│   ├── fsi.h           # Main interface definition
│   └── fsi_defs.h      # DMOD-generated definitions
├── src/
│   └── fsi.c           # Interface registration
├── examples/
│   ├── ramfs/          # Example RAM file system implementation
│   │   ├── ramfs.c
│   │   ├── Makefile
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt
├── Makefile            # Build file for Make
└── CMakeLists.txt      # Build file for CMake
```

## License

MIT License - See LICENSE file for details
