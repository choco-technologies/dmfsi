# RamFS - RAM-Based File System Example

## Overview

RamFS is a reference implementation of the DMFSI (DMOD File System Interface). It demonstrates how to implement a complete file system using the DMFSI DIF (DMOD Interface).

This implementation stores files entirely in RAM, making it useful for:
- Learning how to implement DMFSI
- Testing and development
- Temporary file storage
- Fast I/O operations

## Features

- **Complete DMFSI Implementation**: All 27 DIF functions implemented
- **Simple Memory Management**: Uses linked lists for file storage
- **No External Dependencies**: Only uses DMOD APIs
- **Dynamic Allocation**: Files grow as needed
- **Standard Operations**: Supports read, write, seek, directory operations

## Implementation Details

### File Structure

Files are stored in a simple linked list structure:

```c
typedef struct ramfs_file {
    char name[256];              // File name
    uint8_t* data;               // File content
    size_t size;                 // Current file size
    size_t capacity;             // Allocated capacity
    struct ramfs_file* next;     // Next file in list
} ramfs_file_t;
```

### File Handles

Open file handles track the current position:

```c
typedef struct {
    ramfs_file_t* file;          // Associated file
    size_t position;             // Current position
    int mode;                    // Open mode
} ramfs_handle_t;
```

### Memory Management

- Uses `Dmod_Malloc()` and `Dmod_Free()` for all allocations
- Files start with 64-byte capacity and grow as needed
- Growth factor: doubles capacity when more space is needed

## Building

### Prerequisites

- DMOD library installed and built
- DMFSI interface module installed
- GNU Make or CMake

### Build with Make

```bash
# Set DMOD_DIR
export DMOD_DIR=/path/to/dmod

# Build the module
make
```

### Build with CMake

```bash
# Set DMOD_DIR
export DMOD_DIR=/path/to/dmod

# Configure and build
cmake -B build -DDMOD_BUILD_EXAMPLES=ON
cmake --build build
```

## Usage

### Initializing RamFS

```c
#include "dmfsi.h"

// Initialize with total size limit
dmfsi_context_t ctx = dmfsi_init("size=4096");
if (ctx == NULL) {
    printf("Failed to initialize RamFS\n");
    return -1;
}
```

Configuration format: `size=<bytes>`

Example: `"size=8192"` creates a RAM file system with 8KB limit.

### Basic File Operations

```c
// Create and write a file
void* fp;
dmfsi_fopen(ctx, &fp, "/test.txt", DMFSI_O_WRONLY | DMFSI_O_CREAT, 0);
const char* data = "Hello, World!";
size_t written;
dmfsi_fwrite(ctx, fp, data, strlen(data), &written);
dmfsi_fclose(ctx, fp);

// Read the file
dmfsi_fopen(ctx, &fp, "/test.txt", DMFSI_O_RDONLY, 0);
char buffer[256];
size_t read_bytes;
dmfsi_fread(ctx, fp, buffer, sizeof(buffer), &read_bytes);
dmfsi_fclose(ctx, fp);

// Cleanup
dmfsi_deinit(ctx);
```

### Directory Operations

```c
// List files
void* dp;
dmfsi_opendir(ctx, &dp, "/");

dmfsi_dir_entry_t entry;
while (dmfsi_readdir(ctx, dp, &entry) == DMFSI_OK) {
    printf("%s (%u bytes)\n", entry.name, entry.size);
}

dmfsi_closedir(ctx, dp);
```

## Limitations

As a simple reference implementation, RamFS has some limitations:

1. **No Directory Support**: All files are in a flat namespace (paths like `/file.txt` work, but subdirectories are not fully supported)
2. **No Persistence**: Data is lost when the file system is deinitialized
3. **Limited Metadata**: Minimal file attributes and timestamps
4. **Memory Only**: Cannot be larger than available RAM
5. **No Access Control**: No permission checking or user management
6. **Single-threaded**: Not thread-safe

These limitations are intentional to keep the example simple and focused on demonstrating the DMFSI interface.

## Code Structure

### Initialization Functions

- `dmod_dmfsi_dif_impl_1_0_init()` - Initialize the file system
- `dmod_dmfsi_dif_impl_1_0_deinit()` - Clean up and free memory
- `dmod_dmfsi_dif_impl_1_0_context_is_valid()` - Validate context

### File Operations

- `dmod_dmfsi_dif_impl_1_0_fopen()` - Open or create a file
- `dmod_dmfsi_dif_impl_1_0_fclose()` - Close an open file
- `dmod_dmfsi_dif_impl_1_0_fread()` - Read from a file
- `dmod_dmfsi_dif_impl_1_0_fwrite()` - Write to a file
- `dmod_dmfsi_dif_impl_1_0_lseek()` - Change file position
- `dmod_dmfsi_dif_impl_1_0_tell()` - Get current position
- `dmod_dmfsi_dif_impl_1_0_eof()` - Check for end of file
- `dmod_dmfsi_dif_impl_1_0_size()` - Get file size
- And more...

### Helper Functions

- `ramfs_find_file()` - Find a file by name
- `ramfs_create_file()` - Create a new file entry
- `ramfs_delete_file()` - Remove a file from the list
- `ramfs_ensure_capacity()` - Grow file buffer as needed
- Custom string functions (strcmp, strncpy, memcpy) without stdlib

## Learning from This Example

When creating your own DMFSI implementation, study how RamFS:

1. **Manages Context**: Stores file system state in a context structure
2. **Handles File Operations**: Implements seek, read, write with position tracking
3. **Uses DMOD APIs**: All memory allocation through DMOD SAL
4. **Reports Errors**: Returns appropriate error codes
5. **Validates Input**: Checks parameters before operations
6. **Manages Resources**: Properly allocates and frees memory

## Extending RamFS

To make RamFS more capable, consider adding:

- Directory tree support
- File attributes and permissions
- Timestamps (creation, modification, access)
- Thread-safety with mutexes
- Block-based allocation for efficiency
- File compression
- Persistence (save/load to disk)

## Testing

The RamFS implementation serves as a test case for DMFSI. You can use it to:

- Verify DMFSI interface compatibility
- Test file system operations
- Benchmark file I/O performance
- Develop applications using DMFSI

## Related Documentation

- [DMFSI API Reference](../../docs/dmfsi-api.md)
- [Getting Started Guide](../../docs/getting-started.md)
- [DMOD Documentation](https://github.com/choco-technologies/dmod)

## License

This example is part of the DMFSI project and is provided under the MIT License. See the LICENSE file in the root directory for details.
