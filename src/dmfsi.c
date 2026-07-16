/**
 * @brief DMFSI interface registration file
 * 
 * This file is responsible for registering the DIF signatures.
 * It must enable registration before including the header.
 */

#define DMOD_ENABLE_REGISTRATION    ON
#ifndef DMOD_dmfsi
#   define DMOD_dmfsi
#endif

#include "dmfsi.h"

// This module doesn't need to define the signature variables anymore
// They're now macros defined in the header

// This module doesn't have init/deinit since it's just an interface definition
int dmod_init(const Dmod_Config_t *Config)
{
    Dmod_Printf("DMFSI interface module initialized\n");
    return 0;
}

int dmod_deinit(void)
{
    Dmod_Printf("DMFSI interface module deinitialized\n");
    return 0;
}

/**
 * @brief Weak default for dmfsi_get_mount_path()
 *
 * Used when nothing (e.g. dmvfs) overrides it with a real implementation -
 * a file system that isn't mounted through anything able to answer this is
 * simply reported as not found.
 */
DMOD_INPUT_WEAK_API_DECLARATION( dmfsi, 1.0, int, _get_mount_path, (dmfsi_context_t ctx, char* path_buffer, size_t buffer_size) )
{
    (void)ctx;
    (void)path_buffer;
    (void)buffer_size;
    return DMFSI_ERR_NOT_FOUND;
}
