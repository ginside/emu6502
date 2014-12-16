#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "tools.h"
#include "cpu_variables.h"
#include "emu6502tests.h"

#include "assert.h"

// MOS-6502 Emulator


int main(){
    mem_reset();
    int i = 0x0;
    for(i; i<=0xE ; i++) {
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
4
		if(cycles[opCode] == 0) {
			printf("op code #%x cant have zero cycl5es for execution time..\n", opCode);
			return;
		}*/

        opCode  =   Memory[pc];
        //Cycles decrementation to handle irqs
        ic      -=  cycles[opCode];


        printf("PC=0x%x | OPCODE=0x%x | IC=0x%x \n",pc,opCode,ic);


        switch(opCode)
        {
			case 0x00: opBRK(); break;//BRK
			case 0x01: opORA(adrIndexedIndirect(VALUE)); break;//ORA (APZ,X) // ACC <- ACC v Mem[Operand1+X_reg]
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
			case 0x11: opORA(adrIndirectIndexed(VALUE));break; //ORA (APZ),Y
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

			case 0x20: opJSR(); break; //JSR ADR
			case 0x21: opAND(adrIndexedIndirect(VALUE)); break; //AND (APZ,X)
			case 0x22: break; //NOP
			case 0x23: break; //NOP
			case 0x24: opBIT(adrZeroPage(0, VALUE)); break; //BIT APZ
			case 0x25: opAND(adrZeroPage(0, VALUE)); break; //AND APZ
			case 0x26: mem_set(adrZeroPage(0, ADDRESS), opROL(adrZeroPage(0, VALUE))); break; //ROL APZ
			case 0x27: break; //NOP
			case 0x28: opPLP(); break; //PLP
			case 0x29: opAND(adrImmediate()); break; //AND #DON
			case 0x2a: acc = opROL(acc); break; //ROL ACC
			case 0x2b: break; //NOP
			case 0x2c: opBIT(adrAbsolute(0, VALUE)); break; //BIT ADR
			case 0x2d: opAND(adrAbsolute(0, VALUE)); break; //AND ADR
			case 0x2e: mem_set(adrAbsolute(0, ADDRESS), opROL(adrAbsolute(0, VALUE))); break; //ROL ADR
			case 0x2f: break; //NOP

			case 0x30: opBMI(); break; //BMI rel
			case 0x31: opAND(adrIndirectIndexed(VALUE)); break; //AND (APZ),Y
			case 0x32: break; //NOP
			case 0x33: break; //NOP
			case 0x34: break; //NOP
			case 0x35: opAND(adrZeroPage(x_reg, VALUE)); break; //AND APZ,X
			case 0x36: mem_set(adrZeroPage(x_reg, ADDRESS),opROL(adrZeroPage(x_reg, VALUE))); //ROL APZ,X
			case 0x37: break; //NOP
			case 0x38: opSEC(); break; //SEC
			case 0x39: opAND(adrAbsolute(y_reg, VALUE)); break; //AND ADR,Y
			case 0x3a: break; //NOP
			case 0x3b: break; //NOP
			case 0x3c: break; //NOP
			case 0x3d: opAND(adrAbsolute(x_reg, VALUE)); break; //AND ADR,X
			case 0x3e: mem_set(adrAbsolute(x_reg, ADDRESS),opROL(adrAbsolute(x_reg, VALUE))); break; //ROL ADR,X
			case 0x3f: break; //NOP

			case 0x40: opRTI(); break; //RTI
			case 0x41: opEOR(adrIndexedIndirect(VALUE)); break; //EOR (APZ,X)
			case 0x42: break; //NOP
			case 0x43: break; //NOP
			case 0x44: break; //NOP
			case 0x45: opEOR(adrZeroPage(0, VALUE)); break; //EOR APZ
			case 0x46: mem_set(adrZeroPage(0, ADDRESS), opLSR(adrZeroPage(0, VALUE))); break; //LSR APZ
			case 0x47: break; //NOP
			case 0x48: opPHA(); break; //PHA
			case 0x49: opEOR(adrImmediate()); break; //EOR #DON
			case 0x4a: acc = opLSR(acc); break; //LSR ACC
			case 0x4b: break; //NOP
			case 0x4c: opJMP(adrAbsolute(0, VALUE)); break; //JMP ADR
			case 0x4d: opEOR(adrAbsolute(0, VALUE)); break; //EOR ADR
			case 0x4e: mem_set(adrAbsolute(0, ADDRESS), opLSR(adrAbsolute(0, VALUE))); break; //LSR ADR
			case 0x4f: break; //NOP

			case 0x50: opBVC(); break; //BVC ADR
			case 0x51: opEOR(adrIndirectIndexed(VALUE)); break; //EOR (APZ),Y
			case 0x52: break; //NOP
			case 0x53: break; //NOP
			case 0x54: break; //NOP
			case 0x55: opEOR(adrZeroPage(x_reg, VALUE)); break; //EOR APZ,X
			case 0x56: mem_set(adrZeroPage(x_reg, ADDRESS), opLSR(adrZeroPage(x_reg, VALUE))); break; //LSR APZ,X
			case 0x57: break; //NOP
			case 0x58: opCLI(); break; //CLI
			case 0x59: opEOR(adrAbsolute(y_reg, VALUE)); break; //EOR ADR,Y
			case 0x5a: break; //NOP
			case 0x5b: break; //NOP
			case 0x5c: break; //NOP
			case 0x5d: opEOR(adrAbsolute(x_reg, VALUE)); break; //EOR ADR,X
			case 0x5e: mem_set(adrAbsolute(x_reg, ADDRESS), opLSR(adrAbsolute(x_reg, VALUE))); break; break; //LSR ADR,X
			case 0x5f: break; //NOP

			case 0x60: opRTS(); break; //RTS
			case 0x61: opADC(adrIndexedIndirect(VALUE)); break; //ADC (APZ,X)
			case 0x62: break; //NOP
			case 0x63: break; //NOP
			case 0x64: break; //NOP
			case 0x65: opADC(adrZeroPage(0, VALUE)); break; //ADC APZ
			case 0x66: mem_set(adrZeroPage(0, ADDRESS), opROR(adrZeroPage(0, VALUE))); break; //ROR APZ
			case 0x67: break; //NOP
			case 0x68: opPLA(); break; //PLA
			case 0x69: opADC(adrImmediate()); break; //ADC #DON
			case 0x6a: acc = opROR(acc); break; //ROR ACC
			case 0x6b: break; //NOP
			case 0x6c: opJMP(adrIndirect()); break; //JMP ind
			case 0x6d: opADC(adrAbsolute(0, VALUE)); break; //ADC ADR
			case 0x6e: mem_set(adrAbsolute(0, ADDRESS), opROR(adrAbsolute(0, VALUE))); break; //ROR ADR
			case 0x6f: break; //NOP

			case 0x70: opBVS(); break; //BVS rel
			case 0x71: opADC(adrIndirectIndexed(VALUE)); break; //ADC (APZ),Y
			case 0x72: break; //NOP
			case 0x73: break; //NOP
			case 0x74: break; //NOP
			case 0x75: opADC(adrZeroPage(x_reg, VALUE)); break; //ADC APZ,X
			case 0x76: mem_set(adrZeroPage(x_reg, ADDRESS), opROR(adrZeroPage(x_reg, VALUE)));break; //ROR APZ,X
			case 0x77: break; //NOP
			case 0x78: opSEI(); break; //SEI
			case 0x79: opADC(adrAbsolute(y_reg, VALUE)); break; //ADC ADR,Y
			case 0x7a: break; //NOP
			case 0x7b: break; //NOP
			case 0x7c: break; //NOP
			case 0x7d: opADC(adrAbsolute(x_reg, VALUE)); break; //ADC ADR,X
			case 0x7e: mem_set(adrAbsolute(x_reg, ADDRESS), opROR(adrAbsolute(x_reg, VALUE))); break; //ROR ADR,X
			case 0x7f: break; //NOP

			case 0x80: break; //NOP
			case 0x81: opSTA(adrIndexedIndirect(ADDRESS)); break; //STA (APZ,X)
			case 0x82: break; //NOP
			case 0x83: break; //NOP
			case 0x84: opSTY(adrZeroPage(0, ADDRESS));  break; //STY APZ
			case 0x85: opSTA(adrZeroPage(0, ADDRESS)); break; //STA APZ
			case 0x86: opSTX(adrZeroPage(0, ADDRESS)); break; //STX APZ
			case 0x87: break; //NOP
			case 0x88: opDEY(); break; //DEY
			case 0x89: break; //NOP
			case 0x8a: opTXA(); break; //TXA
			case 0x8b: break; //NOP
			case 0x8c: opSTY(adrAbsolute(0, ADDRESS)); break; //STY ADR
			case 0x8d: opSTA(adrAbsolute(0, ADDRESS)); break; //STA ADR
			case 0x8e: opSTX(adrAbsolute(0, ADDRESS)); break; //STX ADR
			case 0x8f: break; //NOP

			case 0x90: opBCC(); break; //BCC rel
			case 0x91: opSTA(adrIndirectIndexed(ADDRESS)); break; //STA (APZ),Y
			case 0x92: break; //NOP
			case 0x93: break; //NOP
			case 0x94: opSTY(adrZeroPage(x_reg, ADDRESS)); break; //STY APZ,X
			case 0x95: opSTA(adrZeroPage(x_reg, ADDRESS)); break; //STA APZ,X
			case 0x96: opSTX(adrZeroPage(y_reg, ADDRESS)); break; //STX APZ,Y
			case 0x97: break; //NOP
			case 0x98: opTYA(); break; //TYA
			case 0x99: opSTA(adrAbsolute(y_reg, ADDRESS)); break; //STA ADR,Y
			case 0x9a: opTXS(); break; //TXS
			case 0x9b: break; //NOP
			case 0x9c: break; //NOP
			case 0x9d: opSTA(adrAbsolute(x_reg, ADDRESS)); break; //STA ADR,X
			case 0x9e: break; //NOP
			case 0x9f: break; //NOP

			case 0xa0: opLDY(adrImmediate()); break; //LDY #DON
			case 0xa1: opLDA(adrIndexedIndirect(VALUE)); break; //LDA (APZ,X)
			case 0xa2: opLDX(adrImmediate()); break; //LDX #DON
			case 0xa3: break; //NOP
			case 0xa4: opLDY(adrZeroPage(0, VALUE)); break; //LDY APZ
			case 0xa5: opLDA(adrZeroPage(0, VALUE)); break; //LDA APZ
			case 0xa6: opLDX(adrZeroPage(0, VALUE)); break; //LDX APZ
			case 0xa7: break; //NOP
			case 0xa8: opTAY(); break; //TAY
			case 0xa9: opLDA(adrImmediate()); break; //LDA #DON
			case 0xaa: opTAX(); break; //TAX
			case 0xab: break; //NOP
			case 0xac: opLDY(adrAbsolute(0, VALUE)); break; //LDY ADR
			case 0xad: opLDA(adrAbsolute(0, VALUE)); break; //LDA ADR
			case 0xae: opLDX(adrAbsolute(0, VALUE)); break; //LDX ADR
			case 0xaf: break; //NOP

			case 0xb0: opBCS(); break; //BCS rel
			case 0xb1: opLDA(adrIndirectIndexed(VALUE)); break; //LDA (APZ),Y
			case 0xb2: break; //NOP
			case 0xb3: break; //NOP
			case 0xb4: opLDY(adrZeroPage(x_reg, VALUE)); break; //LDY APZ,X
			case 0xb5: opLDA(adrZeroPage(x_reg, VALUE)); break; //LDA APZ,X
			case 0xb6: opLDX(adrZeroPage(y_reg, VALUE)); break; //LDX APZ,Y
			case 0xb7: break; //NOP
			case 0xb8: opCLV(); break; //CLV
			case 0xb9: opLDA(adrAbsolute(y_reg, VALUE)); break; //LDA ADR,Y
			case 0xba: opTSX(); break; //TSX
			case 0xbb: break; //NOP
			case 0xbc: opLDY(adrAbsolute(x_reg, VALUE)); break; //LDY ADR,X
			case 0xbd:
      printf("mem 0x%x \n",adrAbsolute(x_reg,ADDRESS));
      opLDA(adrAbsolute(x_reg, VALUE)); break; //LDA ADR,X
			case 0xbe: opLDX(adrAbsolute(y_reg, VALUE)); break; //LDX ADR,Y
			case 0xbf: break; //NOP

			case 0xc0: opCPY(adrImmediate()); break; //CPY #DON
			case 0xc1: opCMP(adrIndexedIndirect(VALUE)); break; //CMP (APZ,X)
			case 0xc2: break; //NOP
			case 0xc3: break; //NOP
			case 0xc4: opCPY(adrZeroPage(0, VALUE)); break; //CPY APZ
			case 0xc5: opCMP(adrZeroPage(0, VALUE)); break; //CMP APZ
			case 0xc6: mem_set(adrZeroPage(0, ADDRESS), opDEC(adrZeroPage(0, VALUE))); break; //DEC APZ
			case 0xc7: break; //NOP
			case 0xc8: opINY(); break; //INY
			case 0xc9: opCMP(adrImmediate()); break; //CMP #DON
			case 0xca: opDEX(); break; //DEX
			case 0xcb: break; //NOP
			case 0xcc: opCPY(adrAbsolute(0, VALUE)); break; //CPY ADR
			case 0xcd: opCMP(adrAbsolute(0, VALUE)); break; //CMP ADR
			case 0xce: mem_set(adrAbsolute(0, ADDRESS), opDEC(adrAbsolute(0, VALUE))); break; //DEC ADR
			case 0xcf: break; //NOP

			case 0xd0: opBNE(); break; //BNE rel
			case 0xd1: opCMP(adrIndirectIndexed(VALUE)); break; //CMP (APZ),Y
			case 0xd2: break; //NOP
			case 0xd3: break; //NOP
			case 0xd4: break; //NOP
			case 0xd5: opCMP(adrZeroPage(x_reg, VALUE)); break; //CMP APZ,X
			case 0xd6: mem_set(adrZeroPage(x_reg, ADDRESS), opDEC(adrZeroPage(x_reg, VALUE))); break; //DEC APZ,X
			case 0xd7: break; //NOP
			case 0xd8: opCLD(); break; //CLD
			case 0xd9: opCMP(adrAbsolute(y_reg, VALUE)); break; //CMP ADR,Y
			case 0xda: break; //NOP
			case 0xdb: break; //NOP
			case 0xdc: break; //NOP
			case 0xdd: opCMP(adrAbsolute(x_reg, VALUE)); break; //CMP ADR,X
			case 0xde: mem_set(adrAbsolute(x_reg, ADDRESS), opDEC(adrAbsolute(x_reg, VALUE))); break; //DEC ADR,X
			case 0xdf: break; //NOP

			case 0xe0: opCPX(adrImmediate()); break; //CPX #DON
			case 0xe1: opSBC(adrIndexedIndirect(VALUE)); break; //SBC (APZ,X)
			case 0xe2: break; //NOP
			case 0xe3: break; //NOP
			case 0xe4: opCPX(adrZeroPage(0, VALUE)); break; //CPX APZ
			case 0xe5: opSBC(adrZeroPage(0, VALUE)); break; //SBC APZ
			case 0xe6: mem_set(adrZeroPage(0, ADDRESS), opINC(adrZeroPage(0, VALUE))); break; //INC APZ
			case 0xe7: break; //NOP
			case 0xe8: opINX(); break; //INX
			case 0xe9: opSBC(adrImmediate()); break; //SBC #DON
			case 0xea: opNOP(); break; //NOP
			case 0xeb: break; //NOP
			case 0xec: opCPX(adrAbsolute(0, VALUE)); break; //CPX ADR
			case 0xed: opSBC(adrAbsolute(0, VALUE)); break; //SBC ADR
			case 0xee: mem_set(adrAbsolute(0, ADDRESS), opINC(adrAbsolute(0, VALUE))); break; //INC ADR
			case 0xef: break; //NOP

			case 0xf0: opBEQ(); break; //BEQ rel
			case 0xf1: opSBC(adrIndirectIndexed(VALUE)); break; //SBC (APZ),Y
			case 0xf2: break; //NOP
			case 0xf3: break; //NOP
			case 0xf4: break; //NOP
			case 0xf5: opSBC(adrZeroPage(x_reg, VALUE)); break; //SBC APZ,X
			case 0xf6: mem_set(adrZeroPage(x_reg, ADDRESS),opINC(adrZeroPage(x_reg, VALUE))); break; //INC APZ,X
			case 0xf7: break; //NOP
			case 0xf8: opSED(); break; //SED
			case 0xf9: opSBC(adrAbsolute(y_reg, VALUE)); break; //SBC ADR,Y
			case 0xfa: break; //NOP
			case 0xfb: break; //NOP
			case 0xfc: break; //NOP
			case 0xfd: opSBC(adrAbsolute(x_reg, VALUE)); break; //SBC ADR,X
			case 0xfe: mem_set(adrAbsolute(x_reg, ADDRESS),opINC(adrAbsolute(x_reg, VALUE))); break; //INC ADR,X
			case 0xff: break; //NOP
        }

        Counter--;
        if(opCode == 255 || Counter == 0) {
            break;
        }
        if ( opCode == 0x10 /*BPL*/
      	  || opCode == 0x20 /*JSR*/
      	  || opCode == 0x30 /*BMI*/
          || opCode == 0x40 /*RTI*/
          || opCode == 0x4C || opCode == 0x6C /*JMP*/
          || opCode == 0x50 /*BVC*/
          || opCode == 0x70 /*BVS*/) {
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
  state_register &= ~(NEGATIVE | OVERFLOW | CARRY);

  unsigned int result = acc - mem - ((state_register & CARRY) == CARRY ? 0 : 1);

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
