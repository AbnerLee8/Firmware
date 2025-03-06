

#include "bootloader_app.h"
#include "bootloader\bootloader_common.h"

#define BTL_TRIGGER_PATTERN (0x5048434DUL)


static uint32_t *ramStart = (uint32_t *)BTL_TRIGGER_RAM_START;

bool bootloader_Trigger(void)
{
    uint32_t i;

    // Cheap delay. This should give at leat 1 ms delay.
    for (i = 0; i < 2000; i++)
    {
        asm("nop");
    }

    /* Check for Bootloader Trigger Pattern in first 16 Bytes of RAM to enter
     * Bootloader.
     */
    if (BTL_TRIGGER_PATTERN == ramStart[0] && BTL_TRIGGER_PATTERN == ramStart[1] &&
        BTL_TRIGGER_PATTERN == ramStart[2] && BTL_TRIGGER_PATTERN == ramStart[3])
    {
        // Clear the signature for next reset
        ramStart[0] = 0;

        DCACHE_CLEAN_BY_ADDR(ramStart, 4);

        return true;
    }

    return false;
}
