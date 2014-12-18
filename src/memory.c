#include "memory.h"
#include "cpu_variables.h"

void inline mem_write(unsigned short adress, byte word) {

}

byte inline mem_read(unsigned short address) {
  byte b = Memory[address];
  return b;
}
