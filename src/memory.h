#ifndef MEMORY_H
#define MEMORY_H

#include "emu6502.h"

void inline mem_write(unsigned short adress, byte word);
byte inline mem_read(unsigned short address);

#endif //MEMORY_H
