#ifndef    BOOTINFO_H_INCLUDED
#define    BOOTINFO_H_INCLUDED

#include "type.h"

typedef struct _BootInfo
{
    uint8_t cyls, leds, vmode, reserve;
    uint16_t scrnx, scrny;
    uint8_t* vram;
} BootInfo;

const BootInfo* get_bootinfo(void);

#endif
