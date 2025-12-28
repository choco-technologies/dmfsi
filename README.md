# DMFSI - DMOD File System Interface

This repository defines a DMOD Interface (DIF) for file systems. It provides a standardized interface that can be implemented by various file system modules (e.g., FatFS, RamFS, FlashFS) to enable virtual file system (VFS) mounting in DMOD-based systems.

## Overview

The DMFSI (DMOD File System Interface) defines a comprehensive set of file system operations based on POSIX-like semantics. This interface allows different file system implementations to be used interchangeably through the DMOD dynamic module system.

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

- GNU Make or CMake (version 3.18 or higher)
- GCC or compatible C compiler
- Git (for automatic DMOD fetching)
- DMOD library (https://github.com/choco-technologies/dmod.git) - **automatically fetched if not provided**

### Automatic DMOD Fetching

If you don't have DMOD installed, both CMake and Make will automatically fetch and build it for you. Simply run the build commands without setting `DMOD_DIR`.

### Build with CMake

DMOD modules can be built using CMake in two modes:

#### DMOD_MODULE Mode (Default)

In this mode, modules are built as dynamically loadable DMF (DMOD Module Format) files:

**Option 1: Automatic DMOD fetching (recommended for quick start)**
```bash
# Build the DMFSI interface module with examples
# DMOD will be automatically fetched from git
cd /path/to/dmod-fsi
cmake -B build -DDMOD_BUILD_EXAMPLES=ON -DDMOD_BUILD_TOOLS=OFF -DDMOD_BUILD_TESTS=OFF
cmake --build build

# DMF files will be created in build/dmf/
```

**Option 2: Use existing DMOD installation**
```bash
# First, build DMOD
cd /path/to/dmod
make

# Set DMOD_DIR to point to your DMOD installation
export DMOD_DIR=/path/to/dmod

# Build the DMFSI interface module with examples
cd /path/to/dmod-fsi
cmake -B build -DDMOD_BUILD_EXAMPLES=ON
cmake --build build

# DMF files will be created in build/dmf/
```

#### DMOD_SYSTEM Mode

In this mode, modules are built as static libraries that can be linked into a DMOD system application:

```bash
# With automatic DMOD fetching
cmake -B build -DDMOD_MODE=DMOD_SYSTEM -DDMOD_BUILD_EXAMPLES=ON -DDMOD_BUILD_TOOLS=OFF -DDMOD_BUILD_TESTS=OFF
cmake --build build

# Or with existing DMOD installation
export DMOD_DIR=/path/to/dmod
cmake -B build -DDMOD_MODE=DMOD_SYSTEM -DDMOD_BUILD_EXAMPLES=ON
cmake --build build

# Static libraries (.a files) will be created in build/
```

### Build with Make

DMOD modules can also be built using the DMOD build system with Make:

**Option 1: Automatic DMOD fetching (recommended for quick start)**
```bash
# Build the DMFSI interface module
# DMOD will be automatically cloned and built to .dmod/
cd /path/to/dmod-fsi
make

# Build the RamFS example
cd examples/ramfs
make
```

**Option 2: Use existing DMOD installation**
```bash
# First, build DMOD
cd /path/to/dmod
make

# Set DMOD_DIR to point to your DMOD installation
export DMOD_DIR=/path/to/dmod

# Build the DMFSI interface module
cd /path/to/dmod-fsi
make

# Build the RamFS example
cd examples/ramfs
make
```

The build process will generate DMF (DMOD Module Format) files that can be dynamically loaded by DMOD.

**Note**: This module was structured following DMOD's module conventions. To create new DMOD modules, use the `new-module.sh` script from the DMOD repository (available in `scripts/new-module.sh` on the develop branch).

## Interface Definition

The interface is defined in `inc/dmfsi.h`. All operations return integer status codes (DMFSI_OK on success, negative error codes on failure).

### Example Implementation

The `examples/ramfs` directory contains a simple RAM-based file system implementation demonstrating how to implement the DMFSI interface. This serves as a reference for creating new file system modules.

## Usage

To implement a new file system:

1. Create a new module directory
2. Include `dmfsi.h` in your implementation
3. Implement the DIF functions using `dmod_dmfsi_dif_api_declaration` macro
4. Create Makefile and CMakeLists.txt based on the examples
5. Build your module as a DMOD DMF file

## File Structure

```
dmod-fsi/
├── inc/
│   ├── dmfsi.h         # Main interface definition
│   └── dmfsi_defs.h    # DMOD-generated definitions
├── src/
│   └── dmfsi.c         # Interface registration
├── examples/
│   ├── ramfs/          # Example RAM file system implementation
│   │   ├── ramfs.c
│   │   ├── Makefile
│   │   └── CMakeLists.txt
│   └── CMakeLists.txt
├── Makefile            # Build file for Make
└── CMakeLists.txt      # Build file for CMake
```

## Release Process

The repository includes an automated release workflow that builds and packages the DMFSI module for all supported DMOD architectures.

### Creating a Release

1. Create a new release on GitHub with a version tag (e.g., `v1.0`, `v1.1`)
2. The release workflow will automatically:
   - Discover all available DMOD architectures
   - Build the dmfsi interface module for each architecture
   - Build the ramfs example module for each architecture
   - Package the modules with headers, documentation, and license
   - Create versioned release archives (e.g., `dmfsi-v1.0-x86_64-linux.zip`)
   - Generate a `versions.dmm` manifest file
   - Upload all artifacts to the GitHub release
   - Create/update a `vlatest` release pointing to the newest version

### Release Package Contents

Each release archive includes:
- `dmfsi.dmf` - Main interface module
- `ramfs.dmf` - Example RAM file system implementation
- `include/` - Header files (dmfsi.h, dmfsi_defs.h)
- `README.md` - Documentation
- `LICENSE` - License file
- `IMPLEMENTATION.md` - Implementation details
- `RELEASE_NOTES.txt` - Release notes from GitHub

## License

MIT License - See LICENSE file for details
