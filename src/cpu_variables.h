/*
 * cpu_variables.h
 *
 *  Created on: Apr 12, 2014
 *      Author: Daniel
 */

#ifndef CPU_VARIABLES_H_
#define CPU_VARIABLES_H_

/* Processor initialisation */
//Registers\Memory Initialisation

// processor flags ( <-> State Register)
int     NEGATIVE,
		OVERFLOW,
		BREAK,
		DECIMAL,
		INTERRUPT,
		ZERO,
		CARRY;

byte    //Memory declaration
		Memory[0xFFFF],
		//Registers declaration
		state_register,
		// Index registers
		x_reg,
		y_reg,
		//accumulator
		acc;
unsigned short
		// Program Counter
		pc,
		// VECTORS
		irq_vect,
		// stack is into the area of memory between 0x100 and 0x1ff, starts at 1FF
		stack_pointer;
int
		// Cycle counting, for interruptions
		ic,
		interrupt_period;

/* Cpu cycles required per opcode, Y axis is strong weight bit */
int cycles[256];

/* Byte Length for every opCode */
int opByteLength[256];

int ADDRESS,VALUE,LEFT,RIGHT;
enum instruction_types;


#endif /* CPU_VARIABLES_H_ */
