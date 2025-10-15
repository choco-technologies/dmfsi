#ifndef FSI_H
#define FSI_H

#include "dmod.h"
#include "fsi_defs.h"

#include <stddef.h>
#include <stdint.h>

/**
 * @brief FSI - File System Interface for DMOD
 * 
 * This is a DIF (DMOD Interface) that defines file system operations.
 * Multiple modules can implement these interfaces (e.g., FatFS, RamFS, FlashFS).
 * 
 * The interface is based on standard POSIX-like file system operations.
 */

// File operation modes
#define FSI_O_RDONLY    0x0001
#define FSI_O_WRONLY    0x0002
#define FSI_O_RDWR      0x0003
#define FSI_O_CREAT     0x0100
#define FSI_O_TRUNC     0x0200
#define FSI_O_APPEND    0x0400

// Seek modes
#define FSI_SEEK_SET    0
#define FSI_SEEK_CUR    1
#define FSI_SEEK_END    2

// File attributes
#define FSI_ATTR_READONLY   0x01
#define FSI_ATTR_HIDDEN     0x02
#define FSI_ATTR_SYSTEM     0x04
#define FSI_ATTR_DIRECTORY  0x10
#define FSI_ATTR_ARCHIVE    0x20

// Error codes
#define FSI_OK              0
#define FSI_ERR_GENERAL     -1
#define FSI_ERR_NOT_FOUND   -2
#define FSI_ERR_EXISTS      -3
#define FSI_ERR_NO_SPACE    -4
#define FSI_ERR_INVALID     -5
#define FSI_ERR_NOT_EMPTY   -6

/**
 * @brief Directory entry structure
 */
typedef struct {
    char name[256];
    uint32_t size;
    uint32_t attr;
    uint32_t time;
} fsi_dir_entry_t;

/**
 * @brief File statistics structure
 */
typedef struct {
    uint32_t size;
    uint32_t attr;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t atime;
} fsi_stat_t;

// Define DIF signatures for file system operations
// The _sig variables are automatically created by the dmod_fsi_dif macro

/**
 * @brief Initialize the file system
 * @param config Configuration string (FS-specific)
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _init, (const char* config) );

/**
 * @brief Deinitialize the file system
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _deinit, (void) );

/**
 * @brief Open a file
 * @param fp Pointer to store the file handle
 * @param path Path to the file
 * @param mode Open mode (FSI_O_*)
 * @param attr File attributes (FSI_ATTR_*)
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _fopen, (void** fp, const char* path, int mode, int attr) );

/**
 * @brief Close a file
 * @param fp File handle
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _fclose, (void* fp) );

/**
 * @brief Read from a file
 * @param fp File handle
 * @param buffer Buffer to read into
 * @param size Number of bytes to read
 * @param read Pointer to store the number of bytes actually read
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _fread, (void* fp, void* buffer, size_t size, size_t* read) );

/**
 * @brief Write to a file
 * @param fp File handle
 * @param buffer Buffer to write from
 * @param size Number of bytes to write
 * @param written Pointer to store the number of bytes actually written
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _fwrite, (void* fp, const void* buffer, size_t size, size_t* written) );

/**
 * @brief Seek to a position in a file
 * @param fp File handle
 * @param offset Offset to seek to
 * @param whence Seek mode (FSI_SEEK_*)
 * @return The new position, or negative error code
 */
dmod_fsi_dif( 1.0, long, _lseek, (void* fp, long offset, int whence) );

/**
 * @brief Perform I/O control operation
 * @param fp File handle
 * @param request Request code
 * @param arg Argument (request-specific)
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _ioctl, (void* fp, int request, void* arg) );

/**
 * @brief Synchronize file data to storage
 * @param fp File handle
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _sync, (void* fp) );

/**
 * @brief Get a character from file
 * @param fp File handle
 * @return Character read, or negative error code
 */
dmod_fsi_dif( 1.0, int, _getc, (void* fp) );

/**
 * @brief Put a character to file
 * @param fp File handle
 * @param c Character to write
 * @return Character written, or negative error code
 */
dmod_fsi_dif( 1.0, int, _putc, (void* fp, int c) );

/**
 * @brief Get current file position
 * @param fp File handle
 * @return Current position, or negative error code
 */
dmod_fsi_dif( 1.0, long, _tell, (void* fp) );

/**
 * @brief Check if at end of file
 * @param fp File handle
 * @return 1 if at EOF, 0 otherwise, negative on error
 */
dmod_fsi_dif( 1.0, int, _eof, (void* fp) );

/**
 * @brief Get file size
 * @param fp File handle
 * @return File size in bytes, or negative error code
 */
dmod_fsi_dif( 1.0, long, _size, (void* fp) );

/**
 * @brief Flush file buffers
 * @param fp File handle
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _fflush, (void* fp) );

/**
 * @brief Get last error code
 * @param fp File handle
 * @return Error code
 */
dmod_fsi_dif( 1.0, int, _error, (void* fp) );

/**
 * @brief Open a directory
 * @param dp Pointer to store the directory handle
 * @param path Path to the directory
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _opendir, (void** dp, const char* path) );

/**
 * @brief Close a directory
 * @param dp Directory handle
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _closedir, (void* dp) );

/**
 * @brief Read directory entry
 * @param dp Directory handle
 * @param entry Pointer to store the directory entry
 * @return FSI_OK on success, error code otherwise (FSI_ERR_NOT_FOUND at end)
 */
dmod_fsi_dif( 1.0, int, _readdir, (void* dp, fsi_dir_entry_t* entry) );

/**
 * @brief Get file/directory statistics
 * @param path Path to the file/directory
 * @param stat Pointer to store the statistics
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _stat, (const char* path, fsi_stat_t* stat) );

/**
 * @brief Delete a file
 * @param path Path to the file
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _unlink, (const char* path) );

/**
 * @brief Rename a file or directory
 * @param oldpath Current path
 * @param newpath New path
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _rename, (const char* oldpath, const char* newpath) );

/**
 * @brief Change file mode/permissions
 * @param path Path to the file
 * @param mode New mode/permissions
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _chmod, (const char* path, int mode) );

/**
 * @brief Change file access and modification times
 * @param path Path to the file
 * @param atime Access time
 * @param mtime Modification time
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _utime, (const char* path, uint32_t atime, uint32_t mtime) );

/**
 * @brief Create a directory
 * @param path Path to the directory
 * @param mode Directory mode/permissions
 * @return FSI_OK on success, error code otherwise
 */
dmod_fsi_dif( 1.0, int, _mkdir, (const char* path, int mode) );

/**
 * @brief Check if directory exists
 * @param path Path to the directory
 * @return 1 if exists, 0 if not, negative on error
 */
dmod_fsi_dif( 1.0, int, _direxists, (const char* path) );

#endif // FSI_H
