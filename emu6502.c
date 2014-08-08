#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "tools.h"
#include "cpu_variables.h"
#include "emu6502tests.h"

// #include "emu6502tests.c"

// MOS-6502 Emulator


int main(){
	// GLOBAL VARIABLES
	int debug = 1;

    // for concatenated operands
    unsigned short memAdr;
    int opCode = 1;
    int Counter = 20;

    mem_reset();
    loadTestingProgram(1);

    // MAIN LOOP

    for(;;)
    {
        //OpCodeReading + pc incrementation, positionned at first operand (if there is one)



        opCode  =   Memory[pc];
        //Cycles decrementation to handle irqs
        ic      -=  cycles[opCode];

        printf("PC=%d | OPCODE=%x\n",pc,opCode);


        switch(opCode)
        {
          case 0x00://BRK
            state_register |= BREAK;
            stack_push(state_register);
            stack_push((pc<<8)>>8);
            stack_push(pc>>8);
            pc = irq_vect;
            break;
// case 0x15://ORA APZ, X
//                // ACC <- ACC v Mem[Mem[Operand1]+X]
//                acc
          case 0x1://ORA (APZ,X)
//            printf("ORA:%x,%x:%x\n",Memory[pc+1],x_reg ,Memory[Memory[pc+1]+x_reg]);
            // ACC <- ACC v Mem[Operand1+X_reg]
            acc  |= Memory[Memory[pc+1]+x_reg];
            stateReg_checkNZ(acc);
            break;

          case 0x5://ORA APZ
            // ACC <- ACC v Mem[Operand1]
            acc |= Memory[Memory[pc+1]];
            stateReg_checkNZ(acc);
            break;

          case 0x6://ASL APZ
            // Mem[Operand1] <- Mem[Operand1] << 1
            Memory[Memory[pc+1]] = opASL(Memory[Memory[pc+1]]);
            // memAt(Memory[pc+1]);
            break;

          case 0x8://PHP
            // STACK <- SR ; SP -= 1
            stack_push(state_register);
            break;

          case 0x9://ORA #DON
            // ACC <- ACC v Memory[pc+1]
            acc |= Memory[pc+1];
            stateReg_checkNZ(acc);
            break;

          case 0xa://ASL ACC
            // ACC <- ACC << 1
            acc = opASL(acc);
            break;

          case 0xb://ANC ??? TO CHECK
            break;

          case 0xc://NOP
            break;

          case 0xd://ORA ADR
            // ACC <- ACC v Mem[Mem[PC+2]Mem[PC+1]]
            acc |= Memory[concat_operands(Memory[pc+1],Memory[pc+2])];
            stateReg_checkNZ(acc);
            break;

          case 0xe://ASL ADR
            // Mem[Mem[PC+2]Mem[PC+1]] <- Mem[Mem[PC+2]Mem[PC+1]] << 1
            memAdr = concat_operands(Memory[pc+1],Memory[pc+2]);
            Memory[memAdr] = opASL(Memory[memAdr]);
            break;

          case 0xf://NOP
            break;

          case 0x10://BPL  ()()()CREATE FUNCTION FOR RE-USE OF BRANCHING PART ()()()
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
        }

        Counter--;
        if(opCode == 255 || Counter == 0)
            break;
        pc += opByteLength[opCode];
    }
    if(debug == 1)
    {
        cpu_showState(opCode);
        stack_showState();
    }
    return 1;
}


/* OPERATIONS FUNCTIONS */

byte inline opASL(byte value){
    state_register &= ~(CARRY + NEGATIVE + ZERO);

    if((value & 128) > 0)
        state_register |= CARRY;

    value = value << 1;

    if (value != 0)
        state_register |= (value & 128);
    else
        state_register |= ZERO;
    return value;
}


