
#include "bootinfo.h"

const BootInfo* get_bootinfo(void)
{
    return (const BootInfo*)0x0FF0;
}

