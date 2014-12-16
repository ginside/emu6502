#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "cpu_variables.h"
#include "addressing.h"

/** ADDRESSING MODES */

unsigned short adrIndexedIndirect(int mode) {
  byte indirect = Memory[pc+1]+x_reg;
  printf("Memory[pc+1] = 0x%x x_reg = 0x%x\n",Memory[pc+1], x_reg);
  printf("Indirect = 0x%x\n",indirect);
  unsigned short indirect_address = concat_operands(Memory[indirect],Memory[indirect+1]);
  printf("indirect_address = 0x%x\n", indirect_address);
  if(mode == VALUE) {
    return Memory[indirect_address];
  }
  return indirect_address;
}
unsigned short adrIndirectIndexed(int mode) {
  byte indirect = Memory[pc+1];
  unsigned short adr = concat_operands(Memory[indirect],Memory[indirect+1]) + y_reg;
  /*
  * test
  *printf("indirect = %x\n", indirect);
  *mem_get_adress_content(indirect);
  *mem_get_adress_content(indirect+1);
  *printf("concat = %x y=%x\n",concat_operands(Memory[indirect],Memory[indirect+1]),y_reg);
  *rintf("concat + y = %x \n", concat_operands(Memory[indirect],Memory[indirect+1]) + y_reg);
  *mem_get_adress_content(concat_operands(Memory[indirect],Memory[indirect+1]) + y_reg);
  *printf("------\n");
  *
  */
  if(mode == VALUE) {
    return Memory[adr];
  }
  return adr;
}
short unsigned adrZeroPage(byte offset, int mode) {
  unsigned short adr = Memory[pc + 1] + offset;
  if(mode == ADDRESS) {
    return adr;
  }
  //printf("APZ adr = 0x%x\n",adr);
  return Memory[adr];
}

byte adrImmediate() {
  return Memory[pc+1];
}
unsigned short adrIndirect() {
  unsigned short indirectAddress = concat_next_operands();
  printf("Indirect address = 0x%x \n", indirectAddress);
  unsigned short operand = concat_operands(Memory[indirectAddress], Memory[indirectAddress+1]);
  printf("Operand = 0x%x\n",operand);
  return operand;
}
short unsigned adrAbsolute(byte offset, int mode) {
  printf("adr Abs offset = 0x%x\n",offset);
  if(mode == ADDRESS) {
    return concat_next_operands() + offset;
  }
  return Memory[concat_next_operands() + offset];
}
