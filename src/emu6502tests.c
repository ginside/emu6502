#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
#include "emu6502tests.h"
#include "cpu_variables.h"

/* Function template:
int instructionOpCode(){
    DEFINE TESTING PROGRAM IN MEMORY

    LAUNCH THE instruction opCode Function

    TEST MEMORY
}

int instruct_00(){
    //BRK

}

*/

void loadTestingProgram(int program_id){
    switch(program_id){
        case 1:
            //testing instructions from 0x0 to 0x9
            Memory[0] = 0x6;//ASL APZ
            Memory[1] = 0xfa;//OPERAND 1      assert $FA = E2
            Memory[2] = 0x8;//PHP
            Memory[3] = 0x5;// ORA APZ
            Memory[4] = 0xad;
            Memory[5] = 0xff;//end
            Memory[0xad] = 0x12;
            Memory[0xfa] = 0x71;
            Memory[0x174] = 0xFF;
            Memory[0xc7] = 0x2;
            x_reg = 0xc7;
            acc = 0x1;
            
            printLoadedProgram();
            launchEmulation(20,1,1);
            
            assert(Memory[0xfa] == 0xe2); // TEST ASL - Page Zero (0x6)
            assert(Memory[0xFF+0xFF] == 0xa0); // TEST PHP (0x8)
            assert(acc == 0x13); // TEST ORA - Page Zero (0x5)
            
            reinit_emulation();
            
            Memory[0] = 0x9; // ORA IMD
            Memory[1] = 0x1a;
            Memory[2] = 0xff;
            acc = 0x32;
            x_reg = 0;
            
            printLoadedProgram();
            launchEmulation(3,1,1);
            printf("%x\n",acc);
            assert(acc == 0x3a); // TEST ORA - IMD (0x9)
            
            reinit_emulation();
            
            Memory[0] = 0x1; // ORA (IND, X)
            Memory[1] = 0x3e;
            Memory[2] = 0x00; // BRK
            Memory[3] = 0xff;
			Memory[0x43] = 0x15;
			Memory[0x44] = 0x24;
			Memory[0x2415] = 0x63;
            acc = 0x33;
            x_reg = 0x5;
            
            printLoadedProgram();
            launchEmulation(3,1,1);
            
            assert(acc == 0x73); // TEST ORA - (IND, X) (0x1)
            assert((state_register & BREAK) == BREAK);
            //assert(pc == irq_vect);//TEST BRK (0x00)...
            
            break;
            
            
            
        case 2:
            //Testing instructions from 0xa to 0x10
            Memory[0] = 0x0d;// ORA ABS
            Memory[1] = 0x1d;
            Memory[2] = 0xff;
            Memory[3] = 0x0e;// ASL ABS
            Memory[4] = 0x1d;
            Memory[5] = 0xff;
            Memory[6] = 0x10;// BPL Relative
            Memory[7] = 0xfb;
            Memory[8] = 0xff;
            Memory[0xff1d] = 0x1b;
            acc = 0xBe;
            
            printLoadedProgram();
            launchEmulation(3,1,1);
            
            assert(acc == 0xbf); // TEST ORA ABS (0x0d)
            assert(Memory[0xff1d] == 0x36); // TEST ASL - ADR (0xe)
            assert( 1 == 1);
            break;
        case 3:
            //Testing instructions from 0x11 to ...
            Memory[0] = 0x11;
            Memory[1] = 0xa5;
            Memory[2] = 0xff;
            Memory[0xa5] = 0xa6;
            Memory[0xc5] = 0x57;
            y_reg = 0x1f;
            acc = 0x88;
            break;
    }
}

void printLoadedProgram() {	
    int i;
    printf("TESTING PROGRAM LOADED:\n");
	printf("+--------------------+\n");
	printf("|MemAd|OpCd|Opr1|Opr2|\n");
	printf("+-----+----+----+----+\n");
	for(i=0;i<0xffff;i++)
	{

		if(Memory[i] == 255)
			break;
		printf("|$");
		if(i<0x10)
			printf("000");
		else if (i<0x100)
			printf("00");
		else if (i<0x1000)
			printf("0");
		printf("%x|0x",i);
		if(Memory[i] < 0x10)
			printf("0");
		printf("%x|",Memory[i]);
		if(opByteLength[Memory[i]]==1)
			printf("    |    |");
		if(opByteLength[Memory[i]]==2)
		{
		   printf("0x");
		   if(Memory[i+1] < 0x10)
			   printf("0");
		   printf("%x|    |",Memory[i+1]);
		   i+=1;
		}
		if(opByteLength[Memory[i]]==3)
		{
			if(Memory[i+1] < 0x10)
			   printf("0");
			printf("0x%x|",Memory[i+1]);
			if(Memory[i+2] < 0x10)
			   printf("0");
		   printf("0x%x|",Memory[i+2]);
			i+=2;
		}printf("\n");
	}
	printf("+--------------------+\n");
}