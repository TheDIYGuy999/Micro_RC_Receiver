#ifndef pgmRead64_h
#define pgmRead64_h

#include "Arduino.h"

//
// =======================================================================================================
// Read 64 bit block from PROGMEM
// =======================================================================================================
//

uint64_t pgm_read_64(const void *ptr, uint8_t index) {
    uint64_t result;
    memcpy_P( &result, (uint8_t*)ptr + (index * 8), sizeof(uint64_t) ); // ptr is counting in bytes!
    return result;
}

#endif
