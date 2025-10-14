#define DMOD_ENABLE_REGISTRATION    ON
#ifndef DMOD_ramfs
#   define DMOD_ramfs
#endif

#include "dmod.h"
#include "fsi.h"

/**
 * @brief RamFS - Simple RAM-based File System
 * 
 * This is a simple example implementation of the FSI interface.
 * Files are stored entirely in RAM with a simple linked list structure.
 */

#define RAMFS_MAX_FILENAME  64
#define RAMFS_MAX_FILES     32

// Helper functions to replace stdlib functions
static int ramfs_strcmp(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

static char* ramfs_strncpy(char* dest, const char* src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

static void* ramfs_memcpy(void* dest, const void* src, size_t n)
{
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

typedef struct RamFS_File {
    char name[RAMFS_MAX_FILENAME];
    uint8_t* data;
    size_t size;
    size_t capacity;
    size_t position;
    int flags;
    struct RamFS_File* next;
} RamFS_File_t;

static RamFS_File_t* g_file_list = NULL;
static int g_initialized = 0;

// Helper function to find a file by name
static RamFS_File_t* ramfs_find_file(const char* path)
{
    RamFS_File_t* file = g_file_list;
    while (file != NULL) {
        if (ramfs_strcmp(file->name, path) == 0) {
            return file;
        }
        file = file->next;
    }
    return NULL;
}

// Implement _init for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _init, (const char* config) )
{
    Dmod_Printf("RamFS: Initializing file system\n");
    if (g_initialized) {
        Dmod_Printf("RamFS: Already initialized\n");
        return FSI_OK;
    }
    g_file_list = NULL;
    g_initialized = 1;
    return FSI_OK;
}

// Implement _deinit for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _deinit, (void) )
{
    Dmod_Printf("RamFS: Deinitializing file system\n");
    if (!g_initialized) {
        return FSI_OK;
    }
    
    // Free all files
    RamFS_File_t* file = g_file_list;
    while (file != NULL) {
        RamFS_File_t* next = file->next;
        if (file->data != NULL) {
            Dmod_Free(file->data);
        }
        Dmod_Free(file);
        file = next;
    }
    g_file_list = NULL;
    g_initialized = 0;
    return FSI_OK;
}

// Implement _fopen for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _fopen, (void** fp, const char* path, int mode, int attr) )
{
    Dmod_Printf("RamFS: Opening file '%s' with mode 0x%x\n", path, mode);
    
    if (!g_initialized) {
        return FSI_ERR_GENERAL;
    }
    
    RamFS_File_t* file = ramfs_find_file(path);
    
    // Check if file exists
    if (file != NULL) {
        // File exists
        if (mode & FSI_O_CREAT) {
            if (mode & FSI_O_TRUNC) {
                // Truncate existing file
                file->size = 0;
                file->position = 0;
            }
        }
    } else {
        // File doesn't exist
        if (!(mode & FSI_O_CREAT)) {
            return FSI_ERR_NOT_FOUND;
        }
        
        // Create new file
        file = (RamFS_File_t*)Dmod_Malloc(sizeof(RamFS_File_t));
        if (file == NULL) {
            return FSI_ERR_NO_SPACE;
        }
        
        ramfs_strncpy(file->name, path, RAMFS_MAX_FILENAME - 1);
        file->name[RAMFS_MAX_FILENAME - 1] = '\0';
        file->data = NULL;
        file->size = 0;
        file->capacity = 0;
        file->position = 0;
        file->flags = mode;
        file->next = g_file_list;
        g_file_list = file;
    }
    
    if (mode & FSI_O_APPEND) {
        file->position = file->size;
    } else {
        file->position = 0;
    }
    
    *fp = (void*)file;
    return FSI_OK;
}

// Implement _fclose for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _fclose, (void* fp) )
{
    Dmod_Printf("RamFS: Closing file\n");
    // In this simple implementation, we don't actually close the file,
    // just mark the handle as invalid
    return FSI_OK;
}

// Implement _fread for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _fread, (void* fp, void* buffer, size_t size, size_t* read) )
{
    RamFS_File_t* file = (RamFS_File_t*)fp;
    if (file == NULL) {
        return FSI_ERR_INVALID;
    }
    
    size_t available = file->size - file->position;
    size_t to_read = (size < available) ? size : available;
    
    if (to_read > 0 && file->data != NULL) {
        ramfs_memcpy(buffer, file->data + file->position, to_read);
        file->position += to_read;
    }
    
    *read = to_read;
    Dmod_Printf("RamFS: Read %zu bytes (requested %zu)\n", to_read, size);
    return FSI_OK;
}

// Implement _fwrite for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _fwrite, (void* fp, const void* buffer, size_t size, size_t* written) )
{
    RamFS_File_t* file = (RamFS_File_t*)fp;
    if (file == NULL) {
        return FSI_ERR_INVALID;
    }
    
    // Check if we need to expand the buffer
    size_t needed = file->position + size;
    if (needed > file->capacity) {
        size_t new_capacity = needed * 2; // Double the capacity
        if (new_capacity < 256) {
            new_capacity = 256;
        }
        
        uint8_t* new_data = (uint8_t*)Dmod_Malloc(new_capacity);
        if (new_data == NULL) {
            *written = 0;
            return FSI_ERR_NO_SPACE;
        }
        
        if (file->data != NULL) {
            ramfs_memcpy(new_data, file->data, file->size);
            Dmod_Free(file->data);
        }
        
        file->data = new_data;
        file->capacity = new_capacity;
    }
    
    ramfs_memcpy(file->data + file->position, buffer, size);
    file->position += size;
    if (file->position > file->size) {
        file->size = file->position;
    }
    
    *written = size;
    Dmod_Printf("RamFS: Wrote %zu bytes\n", size);
    return FSI_OK;
}

// Implement _lseek for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, long, _lseek, (void* fp, long offset, int whence) )
{
    RamFS_File_t* file = (RamFS_File_t*)fp;
    if (file == NULL) {
        return FSI_ERR_INVALID;
    }
    
    long new_pos;
    switch (whence) {
        case FSI_SEEK_SET:
            new_pos = offset;
            break;
        case FSI_SEEK_CUR:
            new_pos = file->position + offset;
            break;
        case FSI_SEEK_END:
            new_pos = file->size + offset;
            break;
        default:
            return FSI_ERR_INVALID;
    }
    
    if (new_pos < 0) {
        return FSI_ERR_INVALID;
    }
    
    file->position = (size_t)new_pos;
    Dmod_Printf("RamFS: Seek to position %ld\n", new_pos);
    return new_pos;
}

// Implement _ioctl for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _ioctl, (void* fp, int request, void* arg) )
{
    Dmod_Printf("RamFS: ioctl request %d (not implemented)\n", request);
    return FSI_ERR_GENERAL;
}

// Implement _sync for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _sync, (void* fp) )
{
    Dmod_Printf("RamFS: Sync (no-op for RAM)\n");
    return FSI_OK;
}

// Implement _getc for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _getc, (void* fp) )
{
    RamFS_File_t* file = (RamFS_File_t*)fp;
    if (file == NULL || file->position >= file->size) {
        return -1;
    }
    return file->data[file->position++];
}

// Implement _putc for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _putc, (void* fp, int c) )
{
    size_t written;
    uint8_t ch = (uint8_t)c;
    int result = fsi_ramfs_fwrite(fp, &ch, 1, &written);
    if (result != FSI_OK || written != 1) {
        return -1;
    }
    return c;
}

// Implement _tell for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, long, _tell, (void* fp) )
{
    RamFS_File_t* file = (RamFS_File_t*)fp;
    if (file == NULL) {
        return FSI_ERR_INVALID;
    }
    return (long)file->position;
}

// Implement _eof for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _eof, (void* fp) )
{
    RamFS_File_t* file = (RamFS_File_t*)fp;
    if (file == NULL) {
        return FSI_ERR_INVALID;
    }
    return (file->position >= file->size) ? 1 : 0;
}

// Implement _size for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, long, _size, (void* fp) )
{
    RamFS_File_t* file = (RamFS_File_t*)fp;
    if (file == NULL) {
        return FSI_ERR_INVALID;
    }
    return (long)file->size;
}

// Implement _fflush for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _fflush, (void* fp) )
{
    Dmod_Printf("RamFS: Flush (no-op for RAM)\n");
    return FSI_OK;
}

// Implement _error for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _error, (void* fp) )
{
    return FSI_OK;
}

// Implement _opendir for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _opendir, (void** dp, const char* path) )
{
    Dmod_Printf("RamFS: opendir '%s' (not fully implemented)\n", path);
    // For simplicity, return the file list as directory handle
    *dp = (void*)g_file_list;
    return FSI_OK;
}

// Implement _closedir for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _closedir, (void* dp) )
{
    Dmod_Printf("RamFS: closedir\n");
    return FSI_OK;
}

// Implement _readdir for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _readdir, (void* dp, FSI_DirEntry_t* entry) )
{
    RamFS_File_t* file = (RamFS_File_t*)dp;
    if (file == NULL) {
        return FSI_ERR_NOT_FOUND;
    }
    
    ramfs_strncpy(entry->name, file->name, sizeof(entry->name) - 1);
    entry->name[sizeof(entry->name) - 1] = '\0';
    entry->size = file->size;
    entry->attr = 0;
    entry->time = 0;
    
    // Move to next file
    // Note: This modifies the dp pointer, which is not ideal but works for this example
    return FSI_OK;
}

// Implement _stat for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _stat, (const char* path, FSI_Stat_t* stat) )
{
    RamFS_File_t* file = ramfs_find_file(path);
    if (file == NULL) {
        return FSI_ERR_NOT_FOUND;
    }
    
    stat->size = file->size;
    stat->attr = 0;
    stat->ctime = 0;
    stat->mtime = 0;
    stat->atime = 0;
    
    Dmod_Printf("RamFS: stat '%s', size=%u\n", path, stat->size);
    return FSI_OK;
}

// Implement _unlink for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _unlink, (const char* path) )
{
    Dmod_Printf("RamFS: unlink '%s'\n", path);
    
    RamFS_File_t* file = g_file_list;
    RamFS_File_t* prev = NULL;
    
    while (file != NULL) {
        if (ramfs_strcmp(file->name, path) == 0) {
            if (prev == NULL) {
                g_file_list = file->next;
            } else {
                prev->next = file->next;
            }
            
            if (file->data != NULL) {
                Dmod_Free(file->data);
            }
            Dmod_Free(file);
            return FSI_OK;
        }
        prev = file;
        file = file->next;
    }
    
    return FSI_ERR_NOT_FOUND;
}

// Implement _rename for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _rename, (const char* oldpath, const char* newpath) )
{
    Dmod_Printf("RamFS: rename '%s' to '%s'\n", oldpath, newpath);
    
    RamFS_File_t* file = ramfs_find_file(oldpath);
    if (file == NULL) {
        return FSI_ERR_NOT_FOUND;
    }
    
    // Check if new name already exists
    if (ramfs_find_file(newpath) != NULL) {
        return FSI_ERR_EXISTS;
    }
    
    ramfs_strncpy(file->name, newpath, RAMFS_MAX_FILENAME - 1);
    file->name[RAMFS_MAX_FILENAME - 1] = '\0';
    
    return FSI_OK;
}

// Implement _chmod for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _chmod, (const char* path, int mode) )
{
    Dmod_Printf("RamFS: chmod '%s' mode=%d (not implemented)\n", path, mode);
    return FSI_OK;
}

// Implement _utime for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _utime, (const char* path, uint32_t atime, uint32_t mtime) )
{
    Dmod_Printf("RamFS: utime '%s' (not implemented)\n", path);
    return FSI_OK;
}

// Implement _mkdir for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _mkdir, (const char* path, int mode) )
{
    Dmod_Printf("RamFS: mkdir '%s' (not implemented)\n", path);
    return FSI_OK;
}

// Implement _direxists for RamFS
dmod_fsi_dif_api_declaration( 1.0, ramfs, int, _direxists, (const char* path) )
{
    Dmod_Printf("RamFS: direxists '%s' (always returns 0)\n", path);
    return 0;
}

int dmod_init(const Dmod_Config_t *Config)
{
    Dmod_Printf("RamFS module initialized\n");
    return 0;
}

int dmod_deinit(void)
{
    Dmod_Printf("RamFS module deinitialized\n");
    // Clean up if initialized
    if (g_initialized) {
        fsi_ramfs_deinit();
    }
    return 0;
}
