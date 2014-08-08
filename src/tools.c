#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "cpu_variables.h"

void cpu_showState(byte opCode){
    printf("+-- CPU STATE --+\nRegister X : 0x%x\nRegister Y : 0x%x\n", x_reg,y_reg);
    printf("Accumulator : 0x%x\nStack Pointer: 0x%x\nProgram Counter: 0x%x\n",acc,stack_pointer,pc);
    printf("+---------------+\n");
    printf("  State register\n");
    printf("+---------------+\n");
    printf("|N|V|X|B|D|I|Z|C|\n");
    printf("+---------------+\n");
    printf("|%d|%d|X|%d|%d|%d|%d|%d|\n", ((state_register & NEGATIVE)? 1 : 0),
                                                ((state_register & OVERFLOW)? 1 : 0),
                                                ((state_register & BREAK)? 1 : 0),
                                                ((state_register & DECIMAL)? 1 : 0),
                                                ((state_register & INTERRUPT)? 1 : 0),
                                                ((state_register & ZERO) ? 1 : 0),
                                                ((state_register & CARRY)? 1 : 0));
    printf("+---------------+\n\n");
    printf("OpCode: 0x%x\n\n\n",opCode);
}


/* MEMORY FUNCTIONS */

/**
 * Resets the memory address field
 */
void mem_reset() {
    int i =0;
    for (i = 0;i< 0xFF;i++)
        Memory[i] = 0;
}

/**
 * Prints the content of a memory address
 */
void mem_get_adress_content(unsigned short adress){
    printf("$%x : %x\n",adress,Memory[adress]);
}

void mem_set(unsigned short address, byte value) {
	//printf("setting $%x = %x\n",address, value);
	Memory[address] = value;
}
/**
 * Concatenate two operands to form a 16bits word
 */
unsigned short concat_operands(byte lowAdr,byte highAdr){
    return (unsigned short) highAdr*256 + lowAdr;
}
unsigned short concat_next_operands() {
	return (unsigned short) (Memory[pc+2]*256 + Memory[pc+1]);
}
/* STACK FUNCTIONS */

/**
 * Push a memory address content in the stack
 */
void inline stack_push(byte b){
    Memory[0xFF+stack_pointer] = b;
    stack_pointer -= 1;
}

/**
 * Pops the top stack element
 */
byte inline stack_pop(){
    stack_pointer += 1;
    return Memory[0xFF+stack_pointer-1];
}

/**
 * Prints the stack contents
 */
void stack_showState(){
    printf("-- STACK STATE -- \n");
    int i;
    for(i=0xFF;i >= stack_pointer ; i--)
        printf("$%x  %x\n",i,Memory[0xFF+i]);
    for(i=0x9;i >= stack_pointer ; i--)
        printf("$0%x  %x\n",i,Memory[0xFF+i]);

    printf("-- STACK END --\n\n\n");
}
/* STATE REGISTER FUNCTIONS */

/**
 * Sets the N and Z flags after an op_code is executed:
 * 	N: must be set to 1 if the result of the op_code is negative
 * 	Z: must be set to 1 if the result of the op_code is 0
 */
void inline stateReg_checkNZ(byte op_result){
	//Resets N and Z in the state register
    state_register &= ~(NEGATIVE + ZERO);

    if(op_result == 0) {
        state_register |= ZERO;
    	return;
    }

	state_register |= op_result & NEGATIVE;
}


void reinit_emulation() {
	printf("\n\n -- REINITIALISATION --\n\n\n");
	mem_reset();
	ic                  = 0,
	pc                  = 0,
	interrupt_period    = 0,
	state_register      = 32,
		
	// stack is into the area of memory between 0x100 and 0x1ff,
	// starts at 1ff (going downwards)
	stack_pointer       = 0xFF, 
	// Index registers
	x_reg               = 0,
	y_reg               = 0;
	
}
