#ifndef DMFSI_H
#define DMFSI_H

#include "dmod.h"
#include "dmfsi_defs.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief DMFSI - DMOD File System Interface
 * 
 * This is a DIF (DMOD Interface) that defines file system operations.
 * Multiple modules can implement these interfaces (e.g., FatFS, RamFS, FlashFS).
 * 
 * The interface is based on standard POSIX-like file system operations.
 */

/**
 * @brief DMFSI context type
 * 
 * Opaque pointer to file system context structure.
 * Allocated by init, freed by deinit.
 */
typedef struct dmfsi_context* dmfsi_context_t;

// File operation modes
#define DMFSI_O_RDONLY    0x0001
#define DMFSI_O_WRONLY    0x0002
#define DMFSI_O_RDWR      0x0003
#define DMFSI_O_CREAT     0x0100
#define DMFSI_O_TRUNC     0x0200
#define DMFSI_O_APPEND    0x0400

// Seek modes
#define DMFSI_SEEK_SET    0
#define DMFSI_SEEK_CUR    1
#define DMFSI_SEEK_END    2

// File attributes
#define DMFSI_ATTR_READONLY   0x01
#define DMFSI_ATTR_HIDDEN     0x02
#define DMFSI_ATTR_SYSTEM     0x04
#define DMFSI_ATTR_DIRECTORY  0x10
#define DMFSI_ATTR_ARCHIVE    0x20

// Error codes
#define DMFSI_OK              0
#define DMFSI_ERR_GENERAL     -1
#define DMFSI_ERR_NOT_FOUND   -2
#define DMFSI_ERR_EXISTS      -3
#define DMFSI_ERR_NO_SPACE    -4
#define DMFSI_ERR_INVALID     -5
#define DMFSI_ERR_NOT_EMPTY   -6

/**
 * @brief Directory entry structure
 */
typedef struct {
    char name[256];
    uint32_t size;
    uint32_t attr;
    uint32_t time;
} dmfsi_dir_entry_t;

/**
 * @brief File statistics structure
 */
typedef struct {
    uint32_t size;
    uint32_t attr;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t atime;
} dmfsi_stat_t;

// Define DIF signatures for file system operations
// The _sig variables are automatically created by the dmod_dmfsi_dif macro

/**
 * @brief Initialize the file system
 * @param config Configuration string (FS-specific)
 * @return Context pointer on success, NULL on failure
 */
dmod_dmfsi_dif( 1.0, dmfsi_context_t, _init, (const char* config) );

/**
 * @brief Deinitialize the file system
 * @param ctx File system context
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _deinit, (dmfsi_context_t ctx) );

/**
 * @brief Validate the file system context
 * @param ctx File system context
 * @return 1 if context is valid, 0 otherwise
 */
dmod_dmfsi_dif( 1.0, int, _context_is_valid, (dmfsi_context_t ctx) );

/**
 * @brief Open a file
 * @param ctx File system context
 * @param fp Pointer to store the file handle
 * @param path Path to the file
 * @param mode Open mode (DMFSI_O_*)
 * @param attr File attributes (DMFSI_ATTR_*)
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _fopen, (dmfsi_context_t ctx, void** fp, const char* path, int mode, int attr) );

/**
 * @brief Close a file
 * @param ctx File system context
 * @param fp File handle
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _fclose, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Read from a file
 * @param ctx File system context
 * @param fp File handle
 * @param buffer Buffer to read into
 * @param size Number of bytes to read
 * @param read Pointer to store the number of bytes actually read
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _fread, (dmfsi_context_t ctx, void* fp, void* buffer, size_t size, size_t* read) );

/**
 * @brief Write to a file
 * @param ctx File system context
 * @param fp File handle
 * @param buffer Buffer to write from
 * @param size Number of bytes to write
 * @param written Pointer to store the number of bytes actually written
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _fwrite, (dmfsi_context_t ctx, void* fp, const void* buffer, size_t size, size_t* written) );

/**
 * @brief Seek to a position in a file
 * @param ctx File system context
 * @param fp File handle
 * @param offset Offset to seek to
 * @param whence Seek mode (DMFSI_SEEK_*)
 * @return The new position, or negative error code
 */
dmod_dmfsi_dif( 1.0, long, _lseek, (dmfsi_context_t ctx, void* fp, long offset, int whence) );

/**
 * @brief Perform I/O control operation
 * @param ctx File system context
 * @param fp File handle
 * @param request Request code
 * @param arg Argument (request-specific)
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _ioctl, (dmfsi_context_t ctx, void* fp, int request, void* arg) );

/**
 * @brief Synchronize file data to storage
 * @param ctx File system context
 * @param fp File handle
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _sync, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Get a character from file
 * @param ctx File system context
 * @param fp File handle
 * @return Character read, or negative error code
 */
dmod_dmfsi_dif( 1.0, int, _getc, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Put a character to file
 * @param ctx File system context
 * @param fp File handle
 * @param c Character to write
 * @return Character written, or negative error code
 */
dmod_dmfsi_dif( 1.0, int, _putc, (dmfsi_context_t ctx, void* fp, int c) );

/**
 * @brief Get current file position
 * @param ctx File system context
 * @param fp File handle
 * @return Current position, or negative error code
 */
dmod_dmfsi_dif( 1.0, long, _tell, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Check if at end of file
 * @param ctx File system context
 * @param fp File handle
 * @return 1 if at EOF, 0 otherwise, negative on error
 */
dmod_dmfsi_dif( 1.0, int, _eof, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Get file size
 * @param ctx File system context
 * @param fp File handle
 * @return File size in bytes, or negative error code
 */
dmod_dmfsi_dif( 1.0, long, _size, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Flush file buffers
 * @param ctx File system context
 * @param fp File handle
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _fflush, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Get last error code
 * @param ctx File system context
 * @param fp File handle
 * @return Error code
 */
dmod_dmfsi_dif( 1.0, int, _error, (dmfsi_context_t ctx, void* fp) );

/**
 * @brief Open a directory
 * @param ctx File system context
 * @param dp Pointer to store the directory handle
 * @param path Path to the directory
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _opendir, (dmfsi_context_t ctx, void** dp, const char* path) );

/**
 * @brief Close a directory
 * @param ctx File system context
 * @param dp Directory handle
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _closedir, (dmfsi_context_t ctx, void* dp) );

/**
 * @brief Read directory entry
 * @param ctx File system context
 * @param dp Directory handle
 * @param entry Pointer to store the directory entry
 * @return DMFSI_OK on success, error code otherwise (DMFSI_ERR_NOT_FOUND at end)
 */
dmod_dmfsi_dif( 1.0, int, _readdir, (dmfsi_context_t ctx, void* dp, dmfsi_dir_entry_t* entry) );

/**
 * @brief Get file/directory statistics
 * @param ctx File system context
 * @param path Path to the file/directory
 * @param stat Pointer to store the statistics
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _stat, (dmfsi_context_t ctx, const char* path, dmfsi_stat_t* stat) );

/**
 * @brief Delete a file
 * @param ctx File system context
 * @param path Path to the file
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _unlink, (dmfsi_context_t ctx, const char* path) );

/**
 * @brief Rename a file or directory
 * @param ctx File system context
 * @param oldpath Current path
 * @param newpath New path
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _rename, (dmfsi_context_t ctx, const char* oldpath, const char* newpath) );

/**
 * @brief Change file mode/permissions
 * @param ctx File system context
 * @param path Path to the file
 * @param mode New mode/permissions
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _chmod, (dmfsi_context_t ctx, const char* path, int mode) );

/**
 * @brief Change file access and modification times
 * @param ctx File system context
 * @param path Path to the file
 * @param atime Access time
 * @param mtime Modification time
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _utime, (dmfsi_context_t ctx, const char* path, uint32_t atime, uint32_t mtime) );

/**
 * @brief Create a directory
 * @param ctx File system context
 * @param path Path to the directory
 * @param mode Directory mode/permissions
 * @return DMFSI_OK on success, error code otherwise
 */
dmod_dmfsi_dif( 1.0, int, _mkdir, (dmfsi_context_t ctx, const char* path, int mode) );

/**
 * @brief Check if directory exists
 * @param ctx File system context
 * @param path Path to the directory
 * @return 1 if exists, 0 if not, negative on error
 */
dmod_dmfsi_dif( 1.0, int, _direxists, (dmfsi_context_t ctx, const char* path) );

#endif // DMFSI_H
