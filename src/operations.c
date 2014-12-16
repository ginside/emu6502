#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "tools.h"
#include "cpu_variables.h"


/**
*
* Generic Branching function
*/
void inline opBranch() {
  unsigned short memAdr = pc;
  if((Memory[pc+1] & 128) > 0)
  {
    //printf("Memory[pc+1] & 128) > 0\n");-
    //printf("-pc = 0x%x\n",pc);
    //printf("-mem invert = 0x%x\n",(Memory[pc+1]^0xff));
    //TODO Find example to test if this is the right behavior
    pc = pc - (Memory[pc+1]^0xff) - 1;
  } else {
    pc = pc + Memory[pc+1];
  }
  //if branch changes actual page, add 2 cycle, add 1 cycle elsewere
  ic -= 1;
  if((memAdr & 0xff00) != (pc & 0xff00)) {
    ic -= 1;
    //printf("ic--2\n");
  }
  //printf("pc = 0x%x\n",pc);
}

/**
* Generic compare function
*/
void inline opCompare(byte mem, byte cmp) {
  printf("cmp = 0x%x, mem = 0x%x\n", cmp, mem);
  state_register &= ~(CARRY | NEGATIVE | ZERO);
  if(cmp > mem) {
    state_register |= CARRY;
  } else if (cmp == mem) {
    state_register |= (CARRY | ZERO);
  }
  state_register |= ((cmp - mem) & NEGATIVE);
}

/**
* OPERATIONS FUNCTIONS
*/


//"ADD" memory with accumulator and carry
void inline opADC(byte mem) {
  unsigned int result = acc + mem + (state_register & CARRY);

  state_register &= ~(NEGATIVE | OVERFLOW | CARRY);

  printf("------------------ADC, acc = 0x%x , mem = 0x%x-------------\n",acc, mem);
  printf("ACC = 0x%x (%d) - MEM = 0x%x (%d)\n",acc,acc,mem,mem);



  if (state_register & DECIMAL) {
    printf("DECIMAL MODE\n");

    if (((acc & 0xf) + (mem & 0xf) + ((state_register & CARRY) ? 1 : 0)) > 9){
      result += 6;
    }

    stateReg_checkNZ(result); // SET ZERO invalid ?
    if (!((acc ^ mem) & NEGATIVE) && ((acc ^ result) & NEGATIVE)) {
      state_register |= OVERFLOW;
    }
    if(result > 0x99) {
      printf("result = %d\n", result);
      result += 96;
      state_register |= CARRY;
    }




  } else {
    printf("NON DECIMAL MODE\n");
    stateReg_checkNZ(result);
    if (!((acc ^ mem) & NEGATIVE) && ((acc ^ result) & NEGATIVE)) {
      state_register |= OVERFLOW;
    }
    if(result > 0xff) {
      state_register |= CARRY;
    }
  }
  printf("result = %d\n", result);

  acc = (byte) result;
  printf("------------------END OF ADC           --------------------\n");
}


//"AND" memory with accumulator
void inline opAND(byte mem) {
  //printf("value AND = 0x%x acc= 0x%x ",mem,acc);
  acc &= mem;
  //printf(" >>> acc = 0x%x",acc);
  //printf("\n");
  stateReg_checkNZ(acc);
}

byte inline opASL(byte mem) {
  //printf("value = %x\n",value);
  state_register &= ~(CARRY + NEGATIVE + ZERO);

  if((mem & 128) > 0) {
    state_register |= CARRY;
  }

  mem = mem << 1;

  if (mem != 0) {
    state_register |= (mem & 128);
  } else {
    state_register |= ZERO;
  }
  //printf("value = %x\n",value);
  return mem;
}



void inline opBCC() {
  if ((CARRY & state_register) == 0) {
    opBranch();
  }
}

void inline opBCS() {
  if (CARRY & state_register) {
    opBranch();
  }
}

void inline opBEQ() {
  if (ZERO & state_register) {
    opBranch();
  }
}

void inline opBIT(byte mem) {
  //printf("mem = 0x%x\n", mem);
  state_register &= ~(NEGATIVE + OVERFLOW);
  stateReg_checkZ(acc & mem);
  state_register |= mem & NEGATIVE;
  state_register |= mem & OVERFLOW;
}


void inline opBMI() {
  //cpu_showState(0x30);
  if ((NEGATIVE & state_register) == NEGATIVE) {
    opBranch();
  }
}

void inline opBNE() {
  if ((ZERO & state_register) == 0) {
    opBranch();
  }
}

void inline opBPL() {
  if((NEGATIVE & state_register) == 0){
    opBranch();
  }
}

void inline opBRK() {
  state_register |= BREAK;
  stack_push(state_register);
  stack_push(pc>>8);
  stack_push((pc << 8) >> 8);
  pc = irq_vect;
}

void inline opBVC() {
  if ((OVERFLOW & state_register) == 0) {
    opBranch();
  }
}

void inline opBVS() {
  if (OVERFLOW & state_register) {
    opBranch();
  }
}

void inline opCLC() {
  state_register &= ~CARRY;
}

void inline opCLD() {
  state_register &= ~DECIMAL;
}

void inline opCLI(){
  state_register &= ~INTERRUPT;
}

void inline opCLV() {
  state_register &= ~OVERFLOW;
}


void inline opCMP(byte mem) {
  opCompare(mem, acc);

}
void inline opCPX(byte mem) {
  opCompare(mem, x_reg);
}
void inline opCPY(byte mem) {
  opCompare(mem, y_reg);
}


byte inline opDEC(byte mem) {
  byte result = mem - 1;
  stateReg_checkNZ(result);
  return result;

}

void inline opDEY() {
  y_reg -= 1;
  stateReg_checkNZ(y_reg);
}

void inline opDEX() {
  x_reg -= 1;
  stateReg_checkNZ(x_reg);
}

//"XOR" memory with accumulator
void inline opEOR(byte mem) {
  acc ^= mem;
  stateReg_checkNZ(acc);
}

byte inline opINC(byte mem) {
  byte result = mem + 1;
  stateReg_checkNZ(result);
  return result;
}

void inline opINX() {
  x_reg += 1;
  stateReg_checkNZ(x_reg);
}

void inline opINY() {
  y_reg +=1;
  stateReg_checkNZ(y_reg);
}

void inline opJMP(unsigned short mem) {
  pc = mem;
}

void inline opJSR() {
  stack_push(((pc + 2) >> 8));
  stack_push(((pc + 2) << 8) >> 8);
  pc = concat_next_operands();
}

void inline opLDA(byte mem) {
  stateReg_checkNZ(mem);
  acc = mem;
}

void inline opLDX(byte mem) {
  stateReg_checkNZ(mem);
  x_reg = mem;
}

void inline opLDY(byte mem) {
  stateReg_checkNZ(mem);
  y_reg = mem;
}

byte inline opLSR(byte mem) {
  //printf("LSR MEM = 0x%x \n",mem);
  byte new_carry = 0;
  if (mem & CARRY) {
    new_carry = CARRY;
  }

  mem >>= 1;

  if (mem == 0) {
    state_register |= ZERO;
  }
  state_register &= ~(NEGATIVE);
  state_register |= new_carry;

  return mem;
}

void inline opNOP() {}

//"OR" memory with accumulator
void inline opORA(byte mem) {
  acc  |= mem;
  stateReg_checkNZ(acc);
}


void inline opPHA() {
  stack_push(acc);
}

void inline opPHP() {
  stack_push(state_register);
}

void inline opPLA() {
  acc = stack_pop();
}

void inline opPLP() {
  state_register = stack_pop();
}

byte inline opROL(byte mem) {
  byte old_carry = (state_register & CARRY),
  new_carry = 0;
  //printf("old_carry = 0x%x\n", old_carry);
  if (mem & NEGATIVE) {
    new_carry = CARRY;
  }
  //printf("new_carry = 0x%x\n", new_carry);
  mem <<= 1;

  if (old_carry) {
    mem |= CARRY;
  }
  state_register &= ~(CARRY);
  state_register |= new_carry;

  //printf("new_mem_value = 0x%x\n", mem);
  return mem;
}

byte inline opROR(byte mem) {
  byte old_carry = (state_register & CARRY),
  new_carry = 0;
  if (mem & CARRY) {
    new_carry = CARRY;
  }

  mem >>= 1;

  if (mem == 0) {
    state_register |= ZERO;
  }
  if (old_carry) {
    mem |= NEGATIVE;
  }
  state_register &= ~(CARRY);
  state_register |= new_carry;

  return mem;
}


void inline opRTI() {
  // byte lowAdr = stack_pop();
  // byte highAdr = stack_pop();
  // pc = concat_operands(lowAdr,highAdr);
  printf("RTI\n");
  printf("pc=0x%x\n",pc);
  byte lowAdrPc = stack_pop();
  byte highAdrPc = stack_pop();
  pc = concat_operands(lowAdrPc,highAdrPc);
  state_register = stack_pop();
  printf("pc=0x%x\n",pc);
}

void inline opRTS() {
  printf("RTS\n");
  printf("pc=0x%x\n",pc);
  byte lowAdrPc = stack_pop();
  byte highAdrPc = stack_pop();
  pc = concat_operands(lowAdrPc,highAdrPc);

  printf("pc=0x%x\n",pc);
}

void inline opSBC(byte mem) {

  unsigned int result = acc - mem - (((state_register & CARRY) == CARRY) ? 0 : 1);

  state_register &= ~(NEGATIVE | OVERFLOW | CARRY);

  printf("------------------SBC, acc = 0x%x , mem = 0x%x-------------\n",acc, mem);
  printf("ACC = 0x%x (%d) - MEM = 0x%x (%d)\n",acc,acc,mem,mem);

  stateReg_checkNZ(result);


  if (((acc ^ mem) & NEGATIVE) && ((acc ^ result) & NEGATIVE)) {
    state_register |= OVERFLOW;
  }

  if (state_register & DECIMAL) {
    // printf("DECIMAL MODE\n");
    if ( ((acc & 0xf) - ((state_register & CARRY) == CARRY ? 0 : 1)) < (mem & 0xf)){
      result -= 6;
    }
    if(result > 0x99) {
      printf("result = %d\n", result);
      result -= 0x60;
    }
  }

  if(result < 0x100) {
    state_register |= CARRY;
  }
  printf("result = %d\n", result);

  acc = (byte) result;
  printf("------------------END OF SBC           --------------------\n");
}

void inline opSEC() {
  state_register |= CARRY;
}

void inline opSED() {
  state_register |= DECIMAL;
}

void inline opSEI() {
  state_register |= INTERRUPT;
}

void inline opSTA(unsigned short m_address) {
  // printf("---------------------STA mem address = 0x%x \n",m_address);
  Memory[m_address] = acc;
}
void inline opSTX(unsigned short m_address){
  Memory[m_address] = x_reg;
}
void inline opSTY(unsigned short m_address){
  Memory[m_address] = y_reg;
}

void inline opTAX() {
  x_reg = acc;
  stateReg_checkNZ(x_reg);
}

void inline opTAY() {
  y_reg = acc;
  stateReg_checkNZ(y_reg);
}

void inline opTSX() {
  x_reg = stack_pointer - 0x100;
  stateReg_checkNZ(x_reg);
}
void inline opTXA() {
  acc = x_reg;
  stateReg_checkNZ(acc);
}
void inline opTXS() {
  stack_pointer = 0x100 + x_reg;
}
void inline opTYA() {
  acc = y_reg;
  stateReg_checkNZ(y_reg);
}
