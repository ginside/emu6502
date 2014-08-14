#ifndef TOOLS_H
#define TOOLS_H

void cpu_showState(byte OpCode);
void stack_showState();
/* MEMORY FUNCTIONS */
void mem_reset();
void mem_get_adress_content(unsigned short adress);
void mem_set(unsigned short address, byte value);

unsigned short concat_operands(byte lowAdr,byte highAdr);

void inline stack_push(byte b);
byte inline stack_pop();

/* STATE REGISTER FUNCTIONS */
void inline stateReg_checkNZ(byte op_result);
void inline stateReg_checkZ(byte op_result);
void reinit_emulation();
#endif /* TOOLS_H */
