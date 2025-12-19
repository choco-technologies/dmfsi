# Getting Started with DMFSI

## Introduction

DMFSI (DMOD File System Interface) provides a standardized API for file system operations in DMOD-based systems. This guide will help you get started with using and implementing DMFSI.

## What is DMFSI?

DMFSI is a DMOD Interface (DIF) that defines file system operations similar to POSIX. It allows:

- **Uniform API**: Use the same API across different file systems
- **Pluggable Implementations**: Switch file systems without changing application code
- **Dynamic Loading**: Load file system modules at runtime
- **Multiple File Systems**: Mount different file systems simultaneously

## Installation

### Using dmf-get

The easiest way to install DMFSI is using the DMOD package manager:

```bash
# Install full package with headers and documentation
dmf-get dmfsi

# Install minimal package (module only)
dmf-get --mini dmfsi

# Install headers only (for development)
dmf-get headers dmfsi

# Install documentation only
dmf-get docs dmfsi
```

### Building from Source

#### Prerequisites

- DMOD library (https://github.com/choco-technologies/dmod.git)
- GNU Make or CMake (version 3.18+)
- GCC or compatible C compiler

#### Build Steps

```bash
# Clone DMOD
git clone https://github.com/choco-technologies/dmod.git
cd dmod
make

# Set DMOD_DIR
export DMOD_DIR=/path/to/dmod

# Clone and build DMFSI
git clone https://github.com/choco-technologies/dmfsi.git
cd dmfsi
make

# The DMF file will be in your DMOD build directory
```

## Basic Usage

### Using a File System Module

Here's a simple example of using a DMFSI-compliant file system:

```c
#include "dmfsi.h"

int main(void) {
    // Initialize the file system
    dmfsi_context_t ctx = dmfsi_init("size=1024");
    if (ctx == NULL) {
        printf("Failed to initialize file system\n");
        return -1;
    }

    // Open a file for writing
    void* fp;
    int result = dmfsi_fopen(ctx, &fp, "/hello.txt", 
                             DMFSI_O_WRONLY | DMFSI_O_CREAT | DMFSI_O_TRUNC, 0);
    if (result != DMFSI_OK) {
        printf("Failed to open file\n");
        dmfsi_deinit(ctx);
        return -1;
    }

    // Write data
    const char* data = "Hello, DMFSI!";
    size_t written;
    result = dmfsi_fwrite(ctx, fp, data, strlen(data), &written);
    if (result != DMFSI_OK || written != strlen(data)) {
        printf("Failed to write data\n");
        dmfsi_fclose(ctx, fp);
        dmfsi_deinit(ctx);
        return -1;
    }

    // Close file
    dmfsi_fclose(ctx, fp);

    // Read the file back
    result = dmfsi_fopen(ctx, &fp, "/hello.txt", DMFSI_O_RDONLY, 0);
    if (result != DMFSI_OK) {
        printf("Failed to open file for reading\n");
        dmfsi_deinit(ctx);
        return -1;
    }

    char buffer[256];
    size_t read_bytes;
    result = dmfsi_fread(ctx, fp, buffer, sizeof(buffer) - 1, &read_bytes);
    if (result == DMFSI_OK) {
        buffer[read_bytes] = '\0';
        printf("Read: %s\n", buffer);
    }

    dmfsi_fclose(ctx, fp);

    // Cleanup
    dmfsi_deinit(ctx);
    return 0;
}
```

### Working with Directories

```c
#include "dmfsi.h"

void list_directory(dmfsi_context_t ctx, const char* path) {
    void* dp;
    
    // Open directory
    int result = dmfsi_opendir(ctx, &dp, path);
    if (result != DMFSI_OK) {
        printf("Failed to open directory: %s\n", path);
        return;
    }

    // Read entries
    dmfsi_dir_entry_t entry;
    while (dmfsi_readdir(ctx, dp, &entry) == DMFSI_OK) {
        printf("%s", entry.name);
        
        if (entry.attr & DMFSI_ATTR_DIRECTORY) {
            printf(" [DIR]");
        }
        
        printf(" (%u bytes)\n", entry.size);
    }

    // Close directory
    dmfsi_closedir(ctx, dp);
}

void create_directory(dmfsi_context_t ctx) {
    // Create a directory
    int result = dmfsi_mkdir(ctx, "/mydir", 0755);
    if (result == DMFSI_OK) {
        printf("Directory created successfully\n");
    } else if (result == DMFSI_ERR_EXISTS) {
        printf("Directory already exists\n");
    }

    // Check if directory exists
    if (dmfsi_direxists(ctx, "/mydir") == 1) {
        printf("Directory exists\n");
    }
}
```

### File Information

```c
#include "dmfsi.h"

void print_file_info(dmfsi_context_t ctx, const char* path) {
    dmfsi_stat_t stat;
    
    int result = dmfsi_stat(ctx, path, &stat);
    if (result != DMFSI_OK) {
        printf("Failed to get file info\n");
        return;
    }

    printf("File: %s\n", path);
    printf("Size: %u bytes\n", stat.size);
    printf("Attributes: 0x%02X\n", stat.attr);
    
    if (stat.attr & DMFSI_ATTR_READONLY) {
        printf("  - Read-only\n");
    }
    if (stat.attr & DMFSI_ATTR_DIRECTORY) {
        printf("  - Directory\n");
    }
    if (stat.attr & DMFSI_ATTR_HIDDEN) {
        printf("  - Hidden\n");
    }
}
```

## Implementing a File System

To create your own DMFSI-compliant file system:

### Step 1: Create Module Structure

```
myfs/
├── myfs.c          # Implementation
├── Makefile        # Build file
└── CMakeLists.txt  # CMake file (optional)
```

### Step 2: Include DMFSI Header

```c
#include "dmfsi.h"
```

### Step 3: Define Your Context

```c
typedef struct {
    // Your file system state
    void* root;
    size_t total_size;
    size_t used_size;
} myfs_context_t;
```

### Step 4: Implement Functions

Use the DIF API declaration macro:

```c
dmod_dmfsi_dif_api_declaration(1.0);
```

Then implement each function:

```c
dmfsi_context_t dmod_dmfsi_dif_impl_1_0_init(const char* config) {
    myfs_context_t* ctx = Dmod_Malloc(sizeof(myfs_context_t));
    if (ctx == NULL) {
        return NULL;
    }
    
    // Initialize your file system
    ctx->root = NULL;
    ctx->total_size = 1024; // Parse from config
    ctx->used_size = 0;
    
    return (dmfsi_context_t)ctx;
}

int dmod_dmfsi_dif_impl_1_0_deinit(dmfsi_context_t ctx) {
    myfs_context_t* myfs = (myfs_context_t*)ctx;
    
    // Clean up your file system
    // Free all allocated memory
    
    Dmod_Free(myfs);
    return DMFSI_OK;
}

// Implement remaining functions...
```

### Step 5: Create Makefile

```makefile
ifndef DMOD_DIR
$(error DMOD_DIR is not set)
endif

include $(DMOD_DIR)/paths.mk

DMOD_MODULE_NAME=myfs
DMOD_MODULE_VERSION=1.0
DMOD_AUTHOR_NAME=Your Name
DMOD_CSOURCES=myfs.c
DMOD_INC_DIRS=$(DMFSI_DIR)/inc
DMOD_LIBS=
DMOD_DEFINITIONS=

include $(DMOD_DMF_LIB_FILE_PATH)
```

### Step 6: Build Your Module

```bash
export DMOD_DIR=/path/to/dmod
export DMFSI_DIR=/path/to/dmfsi
make
```

## Reference Implementation

The `examples/ramfs` directory contains a complete reference implementation of a RAM-based file system. It demonstrates:

- Context management
- File operations (open, read, write, seek, close)
- Directory operations
- Memory management using DMOD APIs
- Error handling

Study `examples/ramfs/ramfs.c` to understand how to implement each DMFSI function.

## Common Patterns

### Error Handling

Always check return values:

```c
int result = dmfsi_fopen(ctx, &fp, path, mode, attr);
if (result != DMFSI_OK) {
    switch (result) {
        case DMFSI_ERR_NOT_FOUND:
            printf("File not found\n");
            break;
        case DMFSI_ERR_NO_SPACE:
            printf("No space left\n");
            break;
        default:
            printf("Error: %d\n", result);
    }
    return -1;
}
```

### Resource Cleanup

Always clean up resources properly:

```c
dmfsi_context_t ctx = dmfsi_init(config);
void* fp = NULL;

// ... operations ...

// Cleanup (even on error)
if (fp) {
    dmfsi_fclose(ctx, fp);
}
if (ctx) {
    dmfsi_deinit(ctx);
}
```

### Checking Context Validity

```c
if (!dmfsi_context_is_valid(ctx)) {
    printf("Invalid context\n");
    return -1;
}
```

## Best Practices

### 1. Initialize Before Use

Always initialize the file system before performing operations:

```c
dmfsi_context_t ctx = dmfsi_init("size=2048");
```

### 2. Check Return Values

All DMFSI functions return status codes. Always check them:

```c
if (result != DMFSI_OK) {
    // Handle error
}
```

### 3. Close Files

Always close files when done:

```c
dmfsi_fclose(ctx, fp);
```

### 4. Use Appropriate Modes

Choose the right file open mode for your use case:

- Read-only: `DMFSI_O_RDONLY`
- Write-only: `DMFSI_O_WRONLY`
- Read-write: `DMFSI_O_RDWR`
- Create if needed: `DMFSI_O_CREAT`
- Truncate existing: `DMFSI_O_TRUNC`
- Append mode: `DMFSI_O_APPEND`

### 5. Handle End-of-File

Check for EOF when reading:

```c
while (!dmfsi_eof(ctx, fp)) {
    // Read data
}
```

### 6. Flush Important Data

Use flush or sync for critical data:

```c
dmfsi_fflush(ctx, fp);  // Flush buffers
dmfsi_sync(ctx, fp);    // Sync to storage
```

## Troubleshooting

### File Not Found

- Verify the path is correct
- Check if parent directories exist
- Use `DMFSI_O_CREAT` flag if creating new files

### No Space Left

- Check available space with your file system
- Close unused files
- Delete unnecessary files

### Invalid Context

- Ensure `dmfsi_init()` succeeded
- Don't use context after `dmfsi_deinit()`
- Check context validity with `dmfsi_context_is_valid()`

## Next Steps

- Read the [API Reference](dmfsi-api.md) for detailed function documentation
- Examine the [RamFS Example](../examples/ramfs/ramfs.c) for implementation details
- Check the [DMOD documentation](https://github.com/choco-technologies/dmod) for module system details

## Support

For issues and questions:
- GitHub Issues: https://github.com/choco-technologies/dmfsi/issues
- DMOD Repository: https://github.com/choco-technologies/dmod
