#include "memory.h"
#include "settings.h"

#include "hardware/flash.h"
#include "hardware/sync.h"

uint8_t UNIQUE_ID;

memory* flashData = (memory*) (XIP_BASE + MY_FLASH_DATA); // 256 * 16 bytes = 1 sector = 16 pages
/*
    flashContent[0] contains fScanMin (rxFrequency) and fScanMax (txFrequeny)
*/

void flashInit()
{
    // test if we already stored data in the flash memory
    flash_get_unique_id(&UNIQUE_ID);
    uint8_t* firstFlashByte = (uint8_t*) (XIP_BASE + MY_FLASH_OFFSET);

    if (UNIQUE_ID != *firstFlashByte) // no data in the flash memory
    {
        memory scannerConfig;
        scannerConfig.rxFrequency = F_MIN; // fScanMin
        scannerConfig.txFrequency = F_MAX; // fScanMax

        auto interruptState = save_and_disable_interrupts();
        
        // write unique id
        flash_range_erase(MY_FLASH_OFFSET, FLASH_SECTOR_SIZE); // erase 1 sector = 4096 bytes
        flash_range_program(MY_FLASH_OFFSET, &UNIQUE_ID, FLASH_PAGE_SIZE); // write 1 page = 256 bytes

        // write flash data
        flash_range_erase(MY_FLASH_DATA, FLASH_SECTOR_SIZE); // erase 1 sector = 4096 bytes
        flash_range_program(MY_FLASH_DATA, (uint8_t*) &scannerConfig, FLASH_PAGE_SIZE);
        
        restore_interrupts (interruptState);
    }
}

uint32_t scanMin()
{
    return flashData[0].rxFrequency;
}

uint32_t scanMax()
{
    return flashData[0].txFrequency;
}