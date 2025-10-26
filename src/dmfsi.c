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
