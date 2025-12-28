# DMFSI(3)

## NAME

dmfsi - DMOD File System Interface

## SYNOPSIS

```c
#include "dmfsi.h"

// Context management
dmfsi_context_t dmfsi_init(const char* config);
int dmfsi_deinit(dmfsi_context_t ctx);
int dmfsi_context_is_valid(dmfsi_context_t ctx);

// File operations
int dmfsi_fopen(dmfsi_context_t ctx, void** fp, const char* path, 
                int mode, int attr);
int dmfsi_fclose(dmfsi_context_t ctx, void* fp);
int dmfsi_fread(dmfsi_context_t ctx, void* fp, void* buffer, 
                size_t size, size_t* read);
int dmfsi_fwrite(dmfsi_context_t ctx, void* fp, const void* buffer, 
                 size_t size, size_t* written);
long dmfsi_lseek(dmfsi_context_t ctx, void* fp, long offset, int whence);
int dmfsi_ioctl(dmfsi_context_t ctx, void* fp, int request, void* arg);
int dmfsi_flush(dmfsi_context_t ctx, void* fp);
int dmfsi_feof(dmfsi_context_t ctx, void* fp);
int dmfsi_ferror(dmfsi_context_t ctx, void* fp);
long dmfsi_ftell(dmfsi_context_t ctx, void* fp);
long dmfsi_fsize(dmfsi_context_t ctx, void* fp);

// Character I/O
int dmfsi_fputc(dmfsi_context_t ctx, void* fp, int c);
int dmfsi_fgetc(dmfsi_context_t ctx, void* fp);

// Directory operations
int dmfsi_opendir(dmfsi_context_t ctx, void** dp, const char* path);
int dmfsi_closedir(dmfsi_context_t ctx, void* dp);
int dmfsi_readdir(dmfsi_context_t ctx, void* dp, dmfsi_dir_entry_t* entry);

// File management
int dmfsi_stat(dmfsi_context_t ctx, const char* path, dmfsi_stat_t* stat);
int dmfsi_unlink(dmfsi_context_t ctx, const char* path);
int dmfsi_rename(dmfsi_context_t ctx, const char* oldpath, const char* newpath);
int dmfsi_chmod(dmfsi_context_t ctx, const char* path, int mode);
int dmfsi_utime(dmfsi_context_t ctx, const char* path, uint32_t atime, 
                uint32_t mtime);

// Directory management
int dmfsi_mkdir(dmfsi_context_t ctx, const char* path, int mode);
int dmfsi_direxists(dmfsi_context_t ctx, const char* path);
```

## DESCRIPTION

The **dmfsi** module provides a standardized DMOD Interface (DIF) for file 
system operations. This interface defines a comprehensive set of POSIX-like 
file system operations that can be implemented by various file system modules 
(e.g., FatFS, RamFS, FlashFS, SPIFFS).

The DMFSI interface enables virtual file system (VFS) mounting in DMOD-based 
systems, allowing applications to work with different file system implementations 
through a unified API.

### File Open Modes

File operations support the following modes:

* `DMFSI_O_RDONLY` - Open file for reading only
* `DMFSI_O_WRONLY` - Open file for writing only
* `DMFSI_O_RDWR` - Open file for reading and writing
* `DMFSI_O_CREAT` - Create file if it doesn't exist
* `DMFSI_O_TRUNC` - Truncate file to zero length
* `DMFSI_O_APPEND` - Append to file

### Seek Modes

File seeking supports the following modes:

* `DMFSI_SEEK_SET` - Seek from beginning of file
* `DMFSI_SEEK_CUR` - Seek from current position
* `DMFSI_SEEK_END` - Seek from end of file

### File Attributes

Files can have the following attributes:

* `DMFSI_ATTR_READONLY` - Read-only file
* `DMFSI_ATTR_HIDDEN` - Hidden file
* `DMFSI_ATTR_SYSTEM` - System file
* `DMFSI_ATTR_DIRECTORY` - Directory
* `DMFSI_ATTR_ARCHIVE` - Archive flag

### Error Codes

Operations return the following error codes:

* `DMFSI_OK` (0) - Success
* `DMFSI_ERR_GENERAL` (-1) - General error
* `DMFSI_ERR_NOT_FOUND` (-2) - File or directory not found
* `DMFSI_ERR_EXISTS` (-3) - File or directory already exists
* `DMFSI_ERR_NO_SPACE` (-4) - No space left on device
* `DMFSI_ERR_INVALID` (-5) - Invalid argument
* `DMFSI_ERR_NOT_EMPTY` (-6) - Directory not empty

## CONTEXT MANAGEMENT

### dmfsi_init()

Initialize the file system and return a context handle.

**Parameters:**
- `config` - Configuration string (file system specific)

**Returns:** Context handle on success, NULL on failure

### dmfsi_deinit()

Deinitialize the file system and free all resources.

**Parameters:**
- `ctx` - File system context

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_context_is_valid()

Validate a file system context.

**Parameters:**
- `ctx` - File system context

**Returns:** 1 if valid, 0 if invalid

## FILE OPERATIONS

### dmfsi_fopen()

Open a file for reading, writing, or both.

**Parameters:**
- `ctx` - File system context
- `fp` - Pointer to store file handle
- `path` - Path to file
- `mode` - Open mode (DMFSI_O_*)
- `attr` - File attributes (DMFSI_ATTR_*)

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_fclose()

Close an open file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_fread()

Read data from an open file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `buffer` - Buffer to read into
- `size` - Number of bytes to read
- `read` - Pointer to store actual bytes read

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_fwrite()

Write data to an open file.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `buffer` - Buffer to write from
- `size` - Number of bytes to write
- `written` - Pointer to store actual bytes written

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_lseek()

Move the file position indicator.

**Parameters:**
- `ctx` - File system context
- `fp` - File handle
- `offset` - Offset in bytes
- `whence` - Seek mode (DMFSI_SEEK_*)

**Returns:** New file position, or negative error code

## DIRECTORY OPERATIONS

### dmfsi_opendir()

Open a directory for reading entries.

**Parameters:**
- `ctx` - File system context
- `dp` - Pointer to store directory handle
- `path` - Path to directory

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_closedir()

Close an open directory.

**Parameters:**
- `ctx` - File system context
- `dp` - Directory handle

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_readdir()

Read the next directory entry.

**Parameters:**
- `ctx` - File system context
- `dp` - Directory handle
- `entry` - Pointer to directory entry structure

**Returns:** DMFSI_OK on success, error code otherwise

## FILE MANAGEMENT

### dmfsi_stat()

Get file or directory information.

**Parameters:**
- `ctx` - File system context
- `path` - Path to file or directory
- `stat` - Pointer to stat structure

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_unlink()

Delete a file.

**Parameters:**
- `ctx` - File system context
- `path` - Path to file

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_rename()

Rename or move a file.

**Parameters:**
- `ctx` - File system context
- `oldpath` - Current path
- `newpath` - New path

**Returns:** DMFSI_OK on success, error code otherwise

### dmfsi_mkdir()

Create a directory.

**Parameters:**
- `ctx` - File system context
- `path` - Path to new directory
- `mode` - Directory permissions

**Returns:** DMFSI_OK on success, error code otherwise

## IMPLEMENTATION

To implement the DMFSI interface in a file system module:

1. Include the DMFSI header:
   ```c
   #include "dmfsi.h"
   ```

2. Declare DIF implementation in CMakeLists.txt:
   ```cmake
   set(DMOD_DIF_IMPLS dmfsi)
   ```

3. Implement all required functions using the DIF API declaration macros

4. Register the implementation with DMOD

## EXAMPLE

See the `ramfs` example in the `examples/` directory for a complete 
implementation of the DMFSI interface.

## SEE ALSO

ramfs(3), dmod(7)

## AUTHORS

DMOD DMFSI Team

## LICENSE

MIT License - See LICENSE file for details
