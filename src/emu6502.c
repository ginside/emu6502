#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "tools.h"
#include "cpu_variables.h"
#include "emu6502tests.h"

// MOS-6502 Emulator


int main(){
    mem_reset();
    int i = 1;
    for(i; i<=3 ; i++) {
		loadTestingProgram(i);
	}

    // MAIN LOOP
	//launchEmulation(Counter, opCode, debug);
    
    return 1;
}

void launchEmulation(int Counter, int opCode, int debug) {
    unsigned short memAdr;
	for(;;)
    {
        //OpCodeReading + pc incrementation, positionned at first operand (if there is one)
		/*
		if(opByteLength[opCode] == 0) {
			printf("op code #%x cant have zero byte length..\n", opCode);
			return;
		}
		
		if(cycles[opCode] == 0) {
			printf("op code #%x cant have zero cycles for execution time..\n", opCode);
			return;
		}*/

        opCode  =   Memory[pc];
        //Cycles decrementation to handle irqs
        ic      -=  cycles[opCode];
        

        printf("PC=%x | OPCODE=%x | IC=%x \n",pc,opCode,ic);
		

        switch(opCode)
        {
			case 0x00: opBRK(); break;//BRK 
			case 0x01: opORA(adrIndexedIndirect()); break;//ORA (APZ,X) // ACC <- ACC v Mem[Operand1+X_reg]
			case 0x02: break; //NOP
			case 0x03: break; //NOP
			case 0x04: break; //NOP
			case 0x05: opORA(adrZeroPage(0, VALUE)); break;//ORA APZ // ACC <- ACC v Mem[Operand1]
			case 0x06: mem_set(adrZeroPage(0, ADDRESS),opASL(adrZeroPage(0, VALUE))); break;//ASL APZ
			case 0x07: break; //NOP
			case 0x08: opPHP(); break; //PHP // STACK <- SR ; SP -= 1
			case 0x09: opORA(adrImmediate()); break;//ORA #DON
			case 0x0a: acc = opASL(acc); break;//ASL ACC
			case 0x0b: break; //NOP
			case 0x0c: break; //NOP
			case 0x0d: opORA(adrAbsolute(0, VALUE)); break; //ORA ADR
			case 0x0e: mem_set(adrAbsolute(0, ADDRESS), opASL(adrAbsolute(0, VALUE))); break; //ASL ADR 
			case 0x0f: break; //NOP
			case 0x10: opBPL();break;//BPL  
			case 0x11: opORA(adrIndirectIndexed());break; //ORA (APZ),Y
			case 0x12: break; //NOP
			case 0x13: break; //NOP
			case 0x14: break; //NOP
			case 0x15: opORA(adrZeroPage(x_reg, VALUE)); break; //ORA APZ, X
			case 0x16: mem_set(adrZeroPage(x_reg, ADDRESS),opASL(adrZeroPage(x_reg, VALUE)));break; //ASL APZ,X
			case 0x17: break; //NOP
			case 0x18: opCLC(); break; //CLC
			case 0x19: opORA(adrAbsolute(y_reg, VALUE)); break; //ORA ADR,Y
			case 0x1a: break; //NOP
			case 0x1b: break; //NOP
			case 0x1c: break; //NOP
			case 0x1d: opORA(adrAbsolute(x_reg, VALUE)); break; //ORA ADR,X 
			case 0x1e: mem_set(adrAbsolute(x_reg, ADDRESS), opASL(adrAbsolute(x_reg, VALUE))); break; //ASL ADR,X
			case 0x1f: break; //NOP
			
			case 0x20: break;
			case 0x21: break;
			case 0x22: break;
			case 0x23: break;
			case 0x24: break;
			case 0x25: break;
			case 0x26: break;
			case 0x27: break;
			case 0x28: break;
			case 0x29: break;
			case 0x2a: break;
			case 0x2b: break;
			case 0x2c: break;
			case 0x2d: break;
			case 0x2e: break;
			case 0x2f: break;
			case 0x30: break;
			case 0x31: break;
			case 0x32: break;
			case 0x33: break;
			case 0x34: break;
			case 0x35: break;
			case 0x36: break;
			case 0x37: break;
			case 0x38: break;
			case 0x39: break;
			case 0x3a: break;
			case 0x3b: break;
			case 0x3c: break;
			case 0x3d: break;
			case 0x3e: break;
			case 0x3f: break;
			case 0x40: break;
			case 0x41: break;
			case 0x42: break;
			case 0x43: break;
			case 0x44: break;
			case 0x45: break;
			case 0x46: break;
			case 0x47: break;
			case 0x48: break;
			case 0x49: break;
			case 0x4a: break;
			case 0x4b: break;
			case 0x4c: break;
			case 0x4d: break;
			case 0x4e: break;
			case 0x4f: break;
			case 0x50: break;
			case 0x51: break;
			case 0x52: break;
			case 0x53: break;
			case 0x54: break;
			case 0x55: break;
			case 0x56: break;
			case 0x57: break;
			case 0x58: break;
			case 0x59: break;
			case 0x5a: break;
			case 0x5b: break;
			case 0x5c: break;
			case 0x5d: break;
			case 0x5e: break;
			case 0x5f: break;
			case 0x60: break;
			case 0x61: break;
			case 0x62: break;
			case 0x63: break;
			case 0x64: break;
			case 0x65: break;
			case 0x66: break;
			case 0x67: break;
			case 0x68: break;
			case 0x69: break;
			case 0x6a: break;
			case 0x6b: break;
			case 0x6c: break;
			case 0x6d: break;
			case 0x6e: break;
			case 0x6f: break;
			case 0x70: break;
			case 0x71: break;
			case 0x72: break;
			case 0x73: break;
			case 0x74: break;
			case 0x75: break;
			case 0x76: break;
			case 0x77: break;
			case 0x78: break;
			case 0x79: break;
			case 0x7a: break;
			case 0x7b: break;
			case 0x7c: break;
			case 0x7d: break;
			case 0x7e: break;
			case 0x7f: break;
			case 0x80: break;
			case 0x81: break;
			case 0x82: break;
			case 0x83: break;
			case 0x84: break;
			case 0x85: break;
			case 0x86: break;
			case 0x87: break;
			case 0x88: break;
			case 0x89: break;
			case 0x8a: break;
			case 0x8b: break;
			case 0x8c: break;
			case 0x8d: break;
			case 0x8e: break;
			case 0x8f: break;
			case 0x90: break;
			case 0x91: break;
			case 0x92: break;
			case 0x93: break;
			case 0x94: break;
			case 0x95: break;
			case 0x96: break;
			case 0x97: break;
			case 0x98: break;
			case 0x99: break;
			case 0x9a: break;
			case 0x9b: break;
			case 0x9c: break;
			case 0x9d: break;
			case 0x9e: break;
			case 0x9f: break;
			case 0xa0: break;
			case 0xa1: break;
			case 0xa2: break;
			case 0xa3: break;
			case 0xa4: break;
			case 0xa5: break;
			case 0xa6: break;
			case 0xa7: break;
			case 0xa8: break;
			case 0xa9: break;
			case 0xaa: break;
			case 0xab: break;
			case 0xac: break;
			case 0xad: break;
			case 0xae: break;
			case 0xaf: break;
			case 0xb0: break;
			case 0xb1: break;
			case 0xb2: break;
			case 0xb3: break;
			case 0xb4: break;
			case 0xb5: break;
			case 0xb6: break;
			case 0xb7: break;
			case 0xb8: break;
			case 0xb9: break;
			case 0xba: break;
			case 0xbb: break;
			case 0xbc: break;
			case 0xbd: break;
			case 0xbe: break;
			case 0xbf: break;
			case 0xc0: break;
			case 0xc1: break;
			case 0xc2: break;
			case 0xc3: break;
			case 0xc4: break;
			case 0xc5: break;
			case 0xc6: break;
			case 0xc7: break;
			case 0xc8: break;
			case 0xc9: break;
			case 0xca: break;
			case 0xcb: break;
			case 0xcc: break;
			case 0xcd: break;
			case 0xce: break;
			case 0xcf: break;
			case 0xd0: break;
			case 0xd1: break;
			case 0xd2: break;
			case 0xd3: break;
			case 0xd4: break;
			case 0xd5: break;
			case 0xd6: break;
			case 0xd7: break;
			case 0xd8: break;
			case 0xd9: break;
			case 0xda: break;
			case 0xdb: break;
			case 0xdc: break;
			case 0xdd: break;
			case 0xde: break;
			case 0xdf: break;
			case 0xe0: break;
			case 0xe1: break;
			case 0xe2: break;
			case 0xe3: break;
			case 0xe4: break;
			case 0xe5: break;
			case 0xe6: break;
			case 0xe7: break;
			case 0xe8: break;
			case 0xe9: break;
			case 0xea: break;
			case 0xeb: break;
			case 0xec: break;
			case 0xed: break;
			case 0xee: break;
			case 0xef: break;
			case 0xf0: break;
			case 0xf1: break;
			case 0xf2: break;
			case 0xf3: break;
			case 0xf4: break;
			case 0xf5: break;
			case 0xf6: break;
			case 0xf7: break;
			case 0xf8: break;
			case 0xf9: break;
			case 0xfa: break;
			case 0xfb: break;
			case 0xfc: break;
			case 0xfd: break;
			case 0xfe: break;
			case 0xff: break;

        }

        Counter--;
        if(opCode == 255 || Counter == 0) {
            break;
        }
        if (opCode == 0x10 /*BPL*/) {
			continue;
		}
		
        pc += opByteLength[opCode];
    }
    
    
    if(debug == 1)
    {
        cpu_showState(opCode);
        stack_showState();
    }
}

/** OPERATIONS FUNCTIONS */
void inline opBRK() {
	state_register |= BREAK;
	stack_push(state_register);
	stack_push((pc<<8)>>8);
	stack_push(pc>>8);
	pc = irq_vect;
}

void inline opPHP() {
	stack_push(state_register); 
}

//"OR" memory with accumulator
void opORA(byte mem) {
	acc  |= mem;
	stateReg_checkNZ(acc);
}

byte inline opASL(byte value) {
	printf("value = %x\n",value);
    state_register &= ~(CARRY + NEGATIVE + ZERO);

    if((value & 128) > 0) {
        state_register |= CARRY;
    }
    
    value = value << 1;

    if (value != 0) {
        state_register |= (value & 128);
    } else {
        state_register |= ZERO;
    }
    printf("value = %x\n",value);
    return value;
}
void inline opBPL() {
	if((NEGATIVE & state_register) == 0){
		opBranch();
	}
}

void inline opCLC() {
	state_register &= ~CARRY;
}
/**
 * Generic Branching function
 */
void inline opBranch() {
	unsigned short memAdr = pc;
	if((Memory[pc+1] & 128) > 0)
	{
		//printf("Memory[pc+1] & 128) > 0\n");
		//printf("-pc = 0x%x\n",pc);
		//printf("-mem invert = 0x%x\n",(Memory[pc+1]^0xff));
		//TODO Find example to test if this is the right behavior
		pc = pc - (Memory[pc+1]^0xff) - 1;
	} else {
		pc = pc + Memory[pc+1];
	}
	//if branch changes actual page, add 2 cycle, 1 elsewere
	ic -= 1;
	if((memAdr & 0xff00) != (pc & 0xff00)) {
		ic -= 1;
		//printf("ic--2\n");
	}
}


/** ADDRESSING MODES */


unsigned short adrIndexedIndirect() {
	byte indirect = Memory[pc+1]+x_reg;
	return Memory[concat_operands(Memory[indirect],Memory[indirect+1])];
}
unsigned short adrIndirectIndexed() {
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
	return Memory[adr];
}
short unsigned adrZeroPage(byte offset, int mode) {
	unsigned short adr = Memory[pc + 1] + offset;
	if(mode == ADDRESS) {
		return adr;
	}
	return Memory[adr];
}

byte adrImmediate() {
	return Memory[pc+1];
}

short unsigned adrAbsolute(byte offset, int mode) {
	if(mode == ADDRESS) {
		return concat_next_operands() + offset;
	}
	return Memory[concat_next_operands() + offset];
}
