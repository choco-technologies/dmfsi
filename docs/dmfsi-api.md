# DMFSI API Reference

## Overview

DMFSI (DMOD File System Interface) is a DMOD Interface (DIF) that defines a standardized API for file system operations. This interface allows different file system implementations (FatFS, RamFS, FlashFS, etc.) to be used interchangeably through the DMOD dynamic module system.

## Context Management

### dmfsi_init

```c
dmod_dmfsi_dif(1.0, dmfsi_context_t, _init, (const char* config));
```

Initialize the file system.

**Parameters:**
- `config` - Configuration string (file system specific)

**Returns:**
- Context pointer on success
- NULL on failure

**Example:**
```c
dmfsi_context_t ctx = dmfsi_init("size=1024");
if (ctx == NULL) {
    // Handle initialization error
}
```

### dmfsi_deinit

```c
dmod_dmfsi_dif(1.0, int, _deinit, (dmfsi_context_t ctx));
```

Deinitialize the file system and free resources.

**Parameters:**
- `ctx` - File system context

**Returns:**
- `DMFSI_OK` (0) on success
- Negative error code on failure

### dmfsi_context_is_valid

```c
dmod_dmfsi_dif(1.0, int, _context_is_valid, (dmfsi_context_t ctx));
```

Validate that a context is valid and usable.

**Parameters:**
- `ctx` - File system context to validate

**Returns:**
- 1 if context is valid
- 0 if context is invalid

## File Operations

### dmfsi_fopen

```c
dmod_dmfsi_dif(1.0, int, _fopen, (dmfsi_context_t ctx, void** fp, const char* path, int mode, int attr));
```

Open or create a file.

**Parameters:**
- `ctx` - File system context
- `fp` - Pointer to store the file handle
- `path` - Path to the file
- `mode` - Open mode flags:
  - `DMFSI_O_RDONLY` - Read only
  - `DMFSI_O_WRONLY` - Write only
  - `DMFSI_O_RDWR` - Read and write
  - `DMFSI_O_CREAT` - Create if doesn't exist
  - `DMFSI_O_TRUNC` - Truncate to zero length
  - `DMFSI_O_APPEND` - Append to file
- `attr` - File attributes (see File Attributes)

**Returns:**
- `DMFSI_OK` on success
- `DMFSI_ERR_NOT_FOUND` if file not found and not creating
- `DMFSI_ERR_EXISTS` if file exists and exclusive create
- Other error codes

**Example:**
```c
void* fp;
int result = dmfsi_fopen(ctx, &fp, "/test.txt", DMFSI_O_RDWR | DMFSI_O_CREAT, 0);
if (result == DMFSI_OK) {
    // File opened successfully
}
```

### dmfsi_fclose

```c
dmod_dmfsi_dif(1.0, int, _fclose, (dmfsi_context_t ctx, void* fp));
```

Close an open file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- `DMFSI_OK` on success
- Negative error code on failure

### dmfsi_fread

```c
dmod_dmfsi_dif(1.0, int, _fread, (dmfsi_context_t ctx, void* fp, void* buffer, size_t size, size_t* read));
```

Read data from an open file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `buffer` - Buffer to read into
- `size` - Number of bytes to read
- `read` - Pointer to store actual bytes read

**Returns:**
- `DMFSI_OK` on success (check `*read` for actual bytes)
- Negative error code on failure

### dmfsi_fwrite

```c
dmod_dmfsi_dif(1.0, int, _fwrite, (dmfsi_context_t ctx, void* fp, const void* buffer, size_t size, size_t* written));
```

Write data to an open file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `buffer` - Buffer containing data to write
- `size` - Number of bytes to write
- `written` - Pointer to store actual bytes written

**Returns:**
- `DMFSI_OK` on success (check `*written` for actual bytes)
- `DMFSI_ERR_NO_SPACE` if insufficient space
- Negative error code on failure

### dmfsi_lseek

```c
dmod_dmfsi_dif(1.0, long, _lseek, (dmfsi_context_t ctx, void* fp, long offset, int whence));
```

Move the file position indicator.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `offset` - Offset to seek to
- `whence` - Reference position:
  - `DMFSI_SEEK_SET` - Beginning of file
  - `DMFSI_SEEK_CUR` - Current position
  - `DMFSI_SEEK_END` - End of file

**Returns:**
- New position on success
- Negative error code on failure

### dmfsi_tell

```c
dmod_dmfsi_dif(1.0, long, _tell, (dmfsi_context_t ctx, void* fp));
```

Get the current file position.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- Current position on success
- Negative error code on failure

### dmfsi_eof

```c
dmod_dmfsi_dif(1.0, int, _eof, (dmfsi_context_t ctx, void* fp));
```

Check if at end of file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- 1 if at end of file
- 0 if not at end of file
- Negative error code on failure

### dmfsi_size

```c
dmod_dmfsi_dif(1.0, long, _size, (dmfsi_context_t ctx, void* fp));
```

Get the size of an open file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- File size in bytes on success
- Negative error code on failure

### dmfsi_fflush

```c
dmod_dmfsi_dif(1.0, int, _fflush, (dmfsi_context_t ctx, void* fp));
```

Flush file buffers to storage.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- `DMFSI_OK` on success
- Negative error code on failure

### dmfsi_sync

```c
dmod_dmfsi_dif(1.0, int, _sync, (dmfsi_context_t ctx, void* fp));
```

Synchronize file data to storage (similar to fsync).

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- `DMFSI_OK` on success
- Negative error code on failure

### dmfsi_getc

```c
dmod_dmfsi_dif(1.0, int, _getc, (dmfsi_context_t ctx, void* fp));
```

Read a single character from file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- Character read (0-255) on success
- Negative error code on failure or EOF

### dmfsi_putc

```c
dmod_dmfsi_dif(1.0, int, _putc, (dmfsi_context_t ctx, void* fp, int c));
```

Write a single character to file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `c` - Character to write

**Returns:**
- Character written on success
- Negative error code on failure

### dmfsi_error

```c
dmod_dmfsi_dif(1.0, int, _error, (dmfsi_context_t ctx, void* fp));
```

Get the last error code for a file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:**
- Error code

### dmfsi_ioctl

```c
dmod_dmfsi_dif(1.0, int, _ioctl, (dmfsi_context_t ctx, void* fp, int request, void* arg));
```

Perform file system specific I/O control operation.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `request` - Request code (file system specific)
- `arg` - Argument pointer (request specific)

**Returns:**
- `DMFSI_OK` on success
- Negative error code on failure

## Directory Operations

### dmfsi_opendir

```c
dmod_dmfsi_dif(1.0, int, _opendir, (dmfsi_context_t ctx, void** dp, const char* path));
```

Open a directory for reading.

**Parameters:**
- `ctx` - File system context
- `dp` - Pointer to store directory handle
- `path` - Path to the directory

**Returns:**
- `DMFSI_OK` on success
- `DMFSI_ERR_NOT_FOUND` if directory not found
- Other error codes

### dmfsi_closedir

```c
dmod_dmfsi_dif(1.0, int, _closedir, (dmfsi_context_t ctx, void* dp));
```

Close an open directory.

**Parameters:**
- `ctx` - File system context
- `dp` - Directory handle

**Returns:**
- `DMFSI_OK` on success
- Negative error code on failure

### dmfsi_readdir

```c
dmod_dmfsi_dif(1.0, int, _readdir, (dmfsi_context_t ctx, void* dp, dmfsi_dir_entry_t* entry));
```

Read next directory entry.

**Parameters:**
- `ctx` - File system context
- `dp` - Directory handle
- `entry` - Pointer to store directory entry information

**Returns:**
- `DMFSI_OK` on success
- `DMFSI_ERR_NOT_FOUND` when no more entries
- Other error codes

**Directory Entry Structure:**
```c
typedef struct {
    char name[256];      // Entry name
    uint32_t size;       // File size
    uint32_t attr;       // Attributes (DMFSI_ATTR_*)
    uint32_t time;       // Timestamp
} dmfsi_dir_entry_t;
```

### dmfsi_mkdir

```c
dmod_dmfsi_dif(1.0, int, _mkdir, (dmfsi_context_t ctx, const char* path, int mode));
```

Create a new directory.

**Parameters:**
- `ctx` - File system context
- `path` - Path of directory to create
- `mode` - Directory permissions/mode

**Returns:**
- `DMFSI_OK` on success
- `DMFSI_ERR_EXISTS` if already exists
- Other error codes

### dmfsi_direxists

```c
dmod_dmfsi_dif(1.0, int, _direxists, (dmfsi_context_t ctx, const char* path));
```

Check if a directory exists.

**Parameters:**
- `ctx` - File system context
- `path` - Directory path to check

**Returns:**
- 1 if directory exists
- 0 if directory does not exist
- Negative error code on failure

## File Management

### dmfsi_stat

```c
dmod_dmfsi_dif(1.0, int, _stat, (dmfsi_context_t ctx, const char* path, dmfsi_stat_t* stat));
```

Get file or directory statistics.

**Parameters:**
- `ctx` - File system context
- `path` - Path to file/directory
- `stat` - Pointer to store statistics

**Returns:**
- `DMFSI_OK` on success
- `DMFSI_ERR_NOT_FOUND` if not found
- Other error codes

**Statistics Structure:**
```c
typedef struct {
    uint32_t size;       // File size in bytes
    uint32_t attr;       // Attributes (DMFSI_ATTR_*)
    uint32_t ctime;      // Creation time
    uint32_t mtime;      // Modification time
    uint32_t atime;      // Access time
} dmfsi_stat_t;
```

### dmfsi_unlink

```c
dmod_dmfsi_dif(1.0, int, _unlink, (dmfsi_context_t ctx, const char* path));
```

Delete a file.

**Parameters:**
- `ctx` - File system context
- `path` - Path to file to delete

**Returns:**
- `DMFSI_OK` on success
- `DMFSI_ERR_NOT_FOUND` if file not found
- Other error codes

### dmfsi_rename

```c
dmod_dmfsi_dif(1.0, int, _rename, (dmfsi_context_t ctx, const char* oldpath, const char* newpath));
```

Rename or move a file or directory.

**Parameters:**
- `ctx` - File system context
- `oldpath` - Current path
- `newpath` - New path

**Returns:**
- `DMFSI_OK` on success
- `DMFSI_ERR_NOT_FOUND` if old path not found
- `DMFSI_ERR_EXISTS` if new path exists
- Other error codes

### dmfsi_chmod

```c
dmod_dmfsi_dif(1.0, int, _chmod, (dmfsi_context_t ctx, const char* path, int mode));
```

Change file mode/permissions.

**Parameters:**
- `ctx` - File system context
- `path` - Path to file
- `mode` - New mode/permissions

**Returns:**
- `DMFSI_OK` on success
- Negative error code on failure

### dmfsi_utime

```c
dmod_dmfsi_dif(1.0, int, _utime, (dmfsi_context_t ctx, const char* path, uint32_t atime, uint32_t mtime));
```

Change file access and modification times.

**Parameters:**
- `ctx` - File system context
- `path` - Path to file
- `atime` - Access time
- `mtime` - Modification time

**Returns:**
- `DMFSI_OK` on success
- Negative error code on failure

## Constants

### File Open Modes

```c
#define DMFSI_O_RDONLY    0x0001  // Read only
#define DMFSI_O_WRONLY    0x0002  // Write only
#define DMFSI_O_RDWR      0x0003  // Read and write
#define DMFSI_O_CREAT     0x0100  // Create if not exists
#define DMFSI_O_TRUNC     0x0200  // Truncate to zero
#define DMFSI_O_APPEND    0x0400  // Append mode
```

### Seek Modes

```c
#define DMFSI_SEEK_SET    0  // From beginning
#define DMFSI_SEEK_CUR    1  // From current position
#define DMFSI_SEEK_END    2  // From end
```

### File Attributes

```c
#define DMFSI_ATTR_READONLY   0x01  // Read-only file
#define DMFSI_ATTR_HIDDEN     0x02  // Hidden file
#define DMFSI_ATTR_SYSTEM     0x04  // System file
#define DMFSI_ATTR_DIRECTORY  0x10  // Directory
#define DMFSI_ATTR_ARCHIVE    0x20  // Archive flag
```

### Error Codes

```c
#define DMFSI_OK              0   // Success
#define DMFSI_ERR_GENERAL    -1   // General error
#define DMFSI_ERR_NOT_FOUND  -2   // File/directory not found
#define DMFSI_ERR_EXISTS     -3   // Already exists
#define DMFSI_ERR_NO_SPACE   -4   // No space left
#define DMFSI_ERR_INVALID    -5   // Invalid parameter
#define DMFSI_ERR_NOT_EMPTY  -6   // Directory not empty
```

## Helper Functions

### dmfsi_strndup

```c
static inline char* dmfsi_strndup(const char* str, size_t n);
```

Duplicate a string using DMOD memory allocation.

**Parameters:**
- `str` - Input string
- `n` - Maximum characters to duplicate

**Returns:**
- Duplicated string pointer
- NULL on failure

### dmfsi_path_create

```c
static inline dmfsi_path_t* dmfsi_path_create(const char* path);
```

Parse a path string into a path structure.

**Parameters:**
- `path` - Path string to parse

**Returns:**
- Pointer to dmfsi_path_t structure
- NULL on failure

### dmfsi_path_free

```c
static inline void dmfsi_path_free(dmfsi_path_t* path);
```

Free a path structure.

**Parameters:**
- `path` - Path structure to free

## Implementation Guide

To implement the DMFSI interface in your file system module:

1. Include the DMFSI header:
```c
#include "dmfsi.h"
```

2. Declare your implementations using the DIF macro:
```c
dmod_dmfsi_dif_api_declaration(1.0);
```

3. Implement all required functions with appropriate logic for your file system.

4. See the `examples/ramfs` directory for a complete reference implementation.

## See Also

- [Getting Started Guide](getting-started.md) - Quick start guide
- [RamFS Example](../examples/ramfs/ramfs.c) - Reference implementation
