#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "tools.h"
#include "cpu_variables.h"
#include "emu6502tests.h"

// MOS-6502 Emulator


int main(){
    mem_reset();
    
    loadTestingProgram(2);

    // MAIN LOOP
	//launchEmulation(Counter, opCode, debug);
    
    return 1;
}

void launchEmulation(int Counter, int opCode, int debug) {
    unsigned short memAdr;
    
	for(;;)
    {
        //OpCodeReading + pc incrementation, positionned at first operand (if there is one)



        opCode  =   Memory[pc];
        //Cycles decrementation to handle irqs
        ic      -=  cycles[opCode];

        printf("PC=%d | OPCODE=%x\n",pc,opCode);


        switch(opCode)
        {
          case 0x00: opBRK(); break;//BRK 
          case 0x1: opORA(adrIndexedIndirect()); break;//ORA (APZ,X) // ACC <- ACC v Mem[Operand1+X_reg]
		  case 0x2: break; //NOP
		  case 0x3: break; //NOP
		  case 0x4: break; //NOP
          case 0x5: opORA(adrZeroPage()); break;//ORA APZ // ACC <- ACC v Mem[Operand1]
          case 0x6: mem_set(Memory[pc+1],opASL(adrZeroPage())); break;//ASL APZ
		  case 0x7: break; //NOP
          case 0x8: opPHP(); break; //PHP // STACK <- SR ; SP -= 1
          case 0x9: opORA(adrImmediate()); break;//ORA #DON
          case 0xa: acc = opASL(acc); break;//ASL ACC
          case 0xb: break; //NOP
          case 0xc: break; //NOP
          case 0xd: opORA(adrAbsolute()); break; //ORA ADR
          case 0xe: mem_set(concat_next_operands(), opASL(adrAbsolute())); break; //ASL ADR 
          case 0xf: break; //NOP
          case 0x10://BPL  
            // IF NEGATIVE = 0: PC <- PC + OP1, if branch changes actual page, add 2 cycle, 1 elsewere
            if((NEGATIVE & state_register) == 0){
//                    memAt(Memory[pc+1]);
                    memAdr = pc;
//                    memAt(memAdr);
                    if((Memory[pc+1] & 128) > 0)
                     {

//                        printf("-pc = 0x%x\n",pc);
//                        printf("-mem invert = 0x%x\n",(Memory[pc+1]^0xff));
                        pc = pc - (Memory[pc+1]^0xff) - 1;

//                        printf("-pc = 0x%x\n",pc);
                     }
                    else
                        pc = pc + Memory[pc+1];

                    if((memAdr & 0xff00) != (pc & 0xff00))
                        ic -= 2;
                    else
                        ic -= 1;
//                    printf("pc = 0x%x\n",pc);
                }
                break;

            case 0x11://ORA (APZ),Y
                // printf("ORA:%x,%x:%x\n",Memory[Memory[pc+1] ],y_reg ,Memory[Memory[Memory[pc+1]]+y_reg]);
                // ACC <- ACC v Mem[Mem[Operand1]+Y_reg]
                acc  |= Memory[Memory[Memory[pc+1]]+y_reg];
                stateReg_checkNZ(acc);
                break;
			
            /*case 0x15://ORA APZ, X
                // ACC <- ACC v Mem[Mem[Operand1]+X]
                acc
                break;*/
			case 0x12: break;
			case 0x13: break;
			case 0x14: break;
			case 0x15: break;
			
			// case 0x15://ORA APZ, X
			//                // ACC <- ACC v Mem[Mem[Operand1]+X]
			//                acc
			case 0x16: break;
			case 0x17: break;
			case 0x18: break;
			case 0x19: break;
			case 0x1a: break;
			case 0x1b: break;
			case 0x1c: break;
			case 0x1d: break;
			case 0x1e: break;
			case 0x1f: break;
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
        pc += opByteLength[opCode];
    }
    
    
    if(debug == 1)
    {
        cpu_showState(opCode);
        stack_showState();
    }
}

/* OPERATIONS FUNCTIONS */
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
	printf("mem = %x\n", mem);
	acc  |= mem;
	stateReg_checkNZ(acc);
}

byte inline opASL(byte value) {
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
    return value;
}
void inline opBPL() {
	
}

/* ADDRESSING MODES */
//
unsigned short adrIndexedIndirect() {
	byte indirect = Memory[pc+1]+x_reg;
	return Memory[concat_operands(Memory[indirect],Memory[indirect+1])];
}/*
byte adrIndirectIndexed() {
	y_reg;
}*/
short unsigned adrZeroPage() {
	return Memory[Memory[pc+1]];
}

byte adrImmediate() {
	return Memory[pc+1];
}

short unsigned adrAbsolute() {
	return Memory[concat_next_operands()];
}
