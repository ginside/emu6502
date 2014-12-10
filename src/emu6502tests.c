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
}opSEC

int instruct_00(){
    //BRK

}

*/

void loadTestingProgram(int program_id){
    reinit_emulation();
    switch(program_id){
        case 0://0x0 to 0x10
            Memory[0x00] = 0x6;//ASL APZ
            Memory[0x01] = 0xfa;//OPERAND 1      assert $FA = E2
            Memory[0x02] = 0x8;//PHP
            Memory[0x03] = 0x5;// ORA APZ
            Memory[0x04] = 0xad;
            Memory[0x05] = 0xff;//end
            Memory[0xad] = 0x12;
            Memory[0xfa] = 0x71;
            Memory[0x174] = 0xFF;
            Memory[0xc7] = 0x2;
            x_reg = 0xc7;
            acc = 0x1;

            printLoadedProgram();
            launchEmulation(5,1,1);

            assert(Memory[0xfa] == 0xe2); // TEST ASL - Page Zero (0x6)
            assert(Memory[0x1FF] == 0xa0); // TEST PHP (0x8)
            assert(acc == 0x13); // TEST ORA - Page Zero (0x5)


            reinit_emulation();
            Memory[0] = 0x9; // ORA IMD
            Memory[1] = 0x1a;
            Memory[2] = 0xff;
            acc = 0x32;
            x_reg = 0;

            printLoadedProgram();
            launchEmulation(3,1,1);
            assert(acc == 0x3a); // TEST ORA - IMD (0x9)

            reinit_emulation();

            Memory[0x00] = 0x1; // ORA (IND, X)
            Memory[0x01] = 0x3e;
            Memory[0x02] = 0x00; // BRK
            Memory[0x03] = 0xff;
      			Memory[0x43] = 0x15;
      			Memory[0x44] = 0x24;
      			Memory[0x2415] = 0x63;
            Memory[0xffff] = 0xff;
            acc = 0x33;
            x_reg = 0x5;

            //printLoadedProgram();
            launchEmulation(4,1,1);

            assert(acc == 0x73); // TEST ORA - (IND, X) (0x1)
            assert((state_register & BREAK) == BREAK);
            //assert(pc == irq_vect);//TEST BRK (0x00)...
            reinit_emulation();

            Memory[0] = 0x0d;// ORA ABS
            Memory[1] = 0x1d;
            Memory[2] = 0xff;
            Memory[3] = 0x0e;// ASL ABS
            Memory[4] = 0x1d;
            Memory[5] = 0xff;
            Memory[6] = 0x10;// BPL Relative
            Memory[7] = 0x12;
            Memory[8] = 0xff;
            Memory[0x18]= 0x10;//BPL Relative invert adress
            Memory[0x19]= 0xf0;
            Memory[0xff1d] = 0x1b;
            acc = 0xBe;

            printLoadedProgram();
            launchEmulation(5,1,1);

            assert(acc == 0xbf); // TEST ORA ABS (0x0d)
            assert(Memory[0xff1d] == 0x36); // TEST ASL - ADR (0x0e)
            assert( pc == 0x8); //TEST DOUBLE BPL
            assert( ic == 0xfffffff0); //TEST DOUBLE BPL
            break;

        case 1://0x11 to 0x1f
            Memory[0] = 0x11; // ORA (IND),Y
            Memory[1] = 0xa5;
            Memory[2] = 0xff;
            Memory[0xa5] = 0x26;
            Memory[0xa6] = 0xbb;
            Memory[0xbb45] = 0x57;
            y_reg = 0x1f;
            acc = 0x88;

            printLoadedProgram();
            launchEmulation(3,1,1);

            assert(acc == 0xdf); // TEST ORA (IND),Y

            reinit_emulation();
            Memory[0] = 0x15; // ORA APZ,X
            Memory[1] = 0x01;
            Memory[2] = 0x16; // ASL APZ,X
            Memory[3] = 0x04;
            Memory[4] = 0xff;
            Memory[0xfd] = 0x81;
            Memory[0xfa] = 0x2f;
            x_reg = 0xf9;
            acc = 0x1b;

            printLoadedProgram();
            launchEmulation(4,1,1);

            assert(acc == 0x3f); // TEST ORA APZ,X
            assert(Memory[0xfd] == 0x2); // TEST ASL APZ,X

            reinit_emulation();
            Memory[0] = 0x19; // ORA ADR,Y
            Memory[1] = 0x01;
            Memory[2] = 0xb1;
            Memory[3] = 0xff;
            Memory[0xb124] = 0x81;
            y_reg = 0x23;
            acc = 0x23;

            printLoadedProgram();
            launchEmulation(3,1,1);

            assert(acc == 0xa3); // TEST ORA ADR,Y

            reinit_emulation();
            Memory[0] = 0x1d; // ORA ADR,X
            Memory[1] = 0x01;
            Memory[2] = 0xb1;
            Memory[3] = 0x1e; // ASL ADR,X
            Memory[4] = 0x01;
            Memory[5] = 0xb1;
            Memory[6] = 0x18;
            Memory[7] = 0xff;
            Memory[0xb124] = 0x81;
            x_reg = 0x23;
            acc = 0x23;

            printLoadedProgram();
            launchEmulation(3,1,1);

            assert(acc == 0xa3); // TEST ORA ADR,X
            assert(Memory[0xb124] == 0x02); // TEST ASL ADR,X
            assert((state_register & CARRY) == 0); // TEST CLC

            break;

        case 2://0x20 to 0x2f

    			Memory[0x0000] 	= 0x20; // JSR ABS
    			Memory[0x0001] 	= 0x05;
    			Memory[0x0002] 	= 0x01;
    			Memory[0x0105] 	= 0x20; // JSR ABS
    			Memory[0x0106]  = 0x03;
    			Memory[0x0107]  = 0x00;
    			Memory[0x0003]	= 0x21; // AND (APZ,X)
    			Memory[0x0004]  = 0xEF;
    			Memory[0x0005]  = 0xFF; // END ---

    			Memory[0x0093]  = 0xAA;
    			Memory[0x00AA]  = 0x17;
    			x_reg = 0xA4;
    			acc = 0xCB;

    			printLoadedProgram();
    			launchEmulation(50,1,1);

    			assert(Memory[0x01FF] == 0); // TEST JSR
    			assert(Memory[0x01FE] == 2); // TEST JSR
    			assert(Memory[0x01FD] == 1); // TEST JSR
    			assert(Memory[0x01FC] == 7); // TEST JSR
    			assert(acc == 0x03); // TEST AND (APZ,X)


                reinit_emulation();

                Memory[0x0000] = 0x24; // BIT APZ
                Memory[0x0001] = 0x56;
                Memory[0x0002] = 0xff;
                Memory[0x0056] = 0xf5;
                acc = 0xa4;

    			printLoadedProgram();
    			launchEmulation(50,1,1);

                assert((state_register & NEGATIVE) != 0); // TEST BIT APZ
                assert((state_register & OVERFLOW) != 0); // TEST BIT APZ
                assert((state_register & ZERO) == 0); // TEST BIT APZ

                reinit_emulation();

                Memory[0x0000] = 0x24; // BIT APZ
                Memory[0x0001] = 0x56;
                Memory[0x0002] = 0xff;
                Memory[0x0056] = 0x00;
                acc = 0xa4;

    			printLoadedProgram();
    			launchEmulation(4,1,1);

                assert((state_register & NEGATIVE) == 0); // TEST BIT APZ
                assert((state_register & OVERFLOW) == 0); // TEST BIT APZ
                assert((state_register & ZERO) != 0); // TEST BIT APZ

                reinit_emulation();

                Memory[0x0000] = 0x25; // AND APZ
                Memory[0x0001] = 0x56;
                Memory[0x0002] = 0xff;
                Memory[0x0056] = 0x00;
                acc = 0xa4;

    			printLoadedProgram();
    			launchEmulation(4,1,1);

                assert((state_register & ZERO) != 0); // TEST AND APZ
                assert(acc == 0); // TEST AND APZ


                reinit_emulation();

                Memory[0x0000] = 0x26; // ROL APZ
                Memory[0x0001] = 0x56; // ROL ADDRESS 0x0056
                Memory[0x0002] = 0x26; // ROL APZ
                Memory[0x0003] = 0x57; // ROL ADDRESS 0x0057
                Memory[0x0004] = 0xff;
                Memory[0x0056] = 0x47;
                Memory[0x0057] = 0x82;
                state_register |= CARRY;

    			printLoadedProgram();
    			launchEmulation(4,1,1);

                assert((state_register & CARRY) != 0);
                assert(Memory[0x0056] == 0x008F); // TEST ROL APZ
                assert(Memory[0x0057] == 0x0004); // TEST ROL APZ

                reinit_emulation();

                Memory[0x0000] = 0x28; // PLP
                Memory[0x0001] = 0xFF; // END
                stack_push(0xE2); // SET SR as 0xE2 in stack


    			printLoadedProgram();
    			launchEmulation(2,1,1);

                assert(state_register == 0x00E2); // TEST PLP
                assert(stack_pointer == 0x01FF); // TEST PLP

                reinit_emulation();

                Memory[0x0000] = 0x29; // AND #DON
                Memory[0x0001] = 0xC2; // SET ACC TO 0x40
                Memory[0x0002] = 0xFF; // END
                Memory[0x01FF] = 0xE2;
                acc = 0x44;

    			printLoadedProgram();
    			launchEmulation(5,1,1);

                assert(acc == 0x40); // TEST AND #DON

                reinit_emulation();

                Memory[0x0000] = 0x2a; // ROL ACC SET ACC TO 0x89
                Memory[0x0001] = 0xFF; // END
                acc = 0x44;
                state_register |= CARRY;

    			printLoadedProgram();
    			launchEmulation(5,1,1);

                assert((state_register & CARRY) == 0);
                assert(acc == 0x89); // TEST ROL ACC

                reinit_emulation();

                Memory[0x0000] = 0x2c; // BIT ABS
                Memory[0x0001] = 0x02;
                Memory[0x0002] = 0x03;
                Memory[0x0003] = 0xFF;

                Memory[0x0302] = 0xFF;
                acc = 0x89;
                state_register |= CARRY;

    			printLoadedProgram();
    			launchEmulation(5,1,1);

                assert((state_register & NEGATIVE) != 0); // TEST BIT ABS
                assert((state_register & OVERFLOW) != 0); // TEST BIT ABS
                assert((state_register & ZERO) == 0); // TEST BIT ABS


                reinit_emulation();

                Memory[0x0000] = 0x2d; // AND ABS
                Memory[0x0001] = 0x90;
                Memory[0x0002] = 0x00;
                Memory[0x0003] = 0xFF;
                Memory[0x0090] = 0x7d;
                acc = 0x89;

    			printLoadedProgram();
    			launchEmulation(5,1,1);

                assert(acc == 0x09); // TEST AND ABS

                reinit_emulation();

                Memory[0x0000] = 0x2E; // ROL ABS
                Memory[0x0001] = 0x90;
                Memory[0x0002] = 0x00;
                Memory[0x0003] = 0xFF;
                Memory[0x0090] = 0x7d;
                state_register |= CARRY;

    			printLoadedProgram();
    			launchEmulation(5,1,1);

                assert(Memory[0x0090] == 0xfb); // TEST AND ABS
                assert((state_register & CARRY) == 0); // TEST AND ABS
    			break;

        case 3://0x30 to 0x3f
            reinit_emulation();
      	    Memory[0x0000] = 0x30; // BMI ADR
      	    Memory[0x0001] = 0x10;
      	    Memory[0x0002] = 0xFF;
      	    Memory[0x0010] = 0x31; // AND (IND),Y
      	    Memory[0x0011] = 0xE1;
      	    Memory[0x0012] = 0xFF; // END
      	    Memory[0x00E1] = 0xAA;
      	    Memory[0x00E2] = 0xAA;
      	    Memory[0xAABC] = 0x04;
                  state_register |= NEGATIVE;
      	    y_reg = 0x12;
      	    acc = 0x05;

      	    printLoadedProgram();
      	    launchEmulation(3,1,1);
                  assert(pc == 0x0012); // TEST BMI ADR
      	    assert(acc == 0x4); // TEST AND (IND),Y

      	    reinit_emulation();
      	    Memory[0x0000] = 0x35; //AND APZ,X
      	    Memory[0x0001] = 0x22;
      	    Memory[0x0002] = 0x36; //ROL APZ,X
            Memory[0x0003] = 0x22;
            Memory[0x0004] = 0x38; // SEC
      	    Memory[0x0005] = 0xFF; //END
      	    Memory[0x0044] = 0x21;

      	    x_reg = 0x22;
      	    acc = 0x22;
      	    printLoadedProgram();
      	    launchEmulation(5,1,1);

      	    assert(acc == 0x20); // TEST AND APZ,X
            assert(Memory[0x0044] == 0x42); // TEST ROL APZ,X
            assert(Memory[0x0044] == 0x42); // TEST ROL APZ,X
            assert((state_register & CARRY) == 1); // TEST CLC

            reinit_emulation();
            Memory[0x0000] = 0x39; //AND ADR,Y
            Memory[0x0001] = 0x12;
            Memory[0x0002] = 0x12;
            Memory[0x0003] = 0xFF;
            Memory[0x1234] = 0x54;
            y_reg = 0x22;
            acc = 0x45;

            printLoadedProgram();
            launchEmulation(3,1,1);
            assert(acc == 0x44); // TEST AND ADR,Y

            reinit_emulation();
            Memory[0x0000] = 0x3D; //AND ADR,X
            Memory[0x0001] = 0x12;
            Memory[0x0002] = 0x12;
            Memory[0x0003] = 0x3E; //ROL ADR,X
            Memory[0x0004] = 0x12;
            Memory[0x0005] = 0x12;
            Memory[0x0006] = 0xFF;
            Memory[0x1234] = 0x54;
            x_reg = 0x22;
            acc = 0x45;

            printLoadedProgram();
            launchEmulation(6,1,1);
            assert(acc == 0x44); // TEST AND ADR,X
            assert(Memory[0x1234] == 0xA8); // TEST ROL ADR,X
            break;

        case 4://0x40 to 0x4f

          reinit_emulation();
          stack_push(0x1E); // PUSH SR
          stack_push(0x1); //HIGH ADR PC
          stack_push(0x2); //LOW ADR PC
          Memory[0x0000] = 0x40; // RTI (return from BRK)
          Memory[0x0102] = 0xFF;

          launchEmulation(2,1,1);
          assert(pc == 0x0102); // TEST RTI
          assert(state_register == 0x1E); // TEST RTI


          reinit_emulation();
          Memory[0x0000] = 0x41; //EOR (APZ,X)
          Memory[0x0001] = 0x12;
          Memory[0x0002] = 0xFF;
          Memory[0x0022] = 0x44;
          Memory[0x0054] = 0x22;
          acc = 0x62;
          x_reg = 0x42;

          printLoadedProgram();
          launchEmulation(2,1,1);

          assert(acc == 0x26); // TEST EOR (APZ,X)

          reinit_emulation();
          Memory[0x0000] = 0x45; //EOR APZ
          Memory[0x0001] = 0x11;
          Memory[0x0002] = 0x46; //LSR APZ
          Memory[0x0003] = 0x12;
          Memory[0x0004] = 0xFF;
          Memory[0x0011] = 0x1A;
          Memory[0x0012] = 0x1B;
          acc = 0xA1;

          printLoadedProgram();
          launchEmulation(4,1,1);

          assert(acc == 0xBB); // TEST EOR APZ
          assert(Memory[0x0012] == 0x0D); //TEST LSR APZ
          assert((state_register & CARRY) == CARRY); //TEST LSR APZ

          reinit_emulation();
          Memory[0x0000] = 0x46; // LSR APZ
          Memory[0x0001] = 0x0A;
          Memory[0x0002] = 0x48; // PHA
          Memory[0x0003] = 0xFF;
          Memory[0x000A] = 0x01;
          acc = 0x11;
          printLoadedProgram();
          launchEmulation(3,1,1);

          assert(Memory[0x000A] == 0);
          assert((state_register & ZERO) == ZERO);  //TEST LSR APZ
          assert((state_register & CARRY) == CARRY); //TEST LSR APZ
          assert(Memory[0x1FF] == 0x11); // TEST PHA

          reinit_emulation();
          Memory[0x0000] = 0x49; // EOR #DON
          Memory[0x0001] = 0xFF;
          Memory[0x0002] = 0xFF;
          acc = 0x11;

          printLoadedProgram();
          launchEmulation(2,1,1);

          assert(acc == 0xEE); // TEST EOR #DON

          reinit_emulation();
          Memory[0x0000] = 0x4A; // LSR ACC
          Memory[0x0001] = 0xFF;
          acc = 0x01;

          printLoadedProgram();
          launchEmulation(2,1,1);

          assert(acc == 0); //TEST LSR ACC
          assert((state_register & ZERO) == ZERO);   //TEST LSR ACC
          assert((state_register & CARRY) == CARRY); //TEST LSR ACC

          reinit_emulation();
          Memory[0x0000] = 0x4C; // JMP ADR
          Memory[0x0001] = 0x01;
          Memory[0x0002] = 0x01;
          Memory[0x0005] = 0x4D; // EOR ADR
          Memory[0x0006] = 0x21;
          Memory[0x0007] = 0x04;
          Memory[0x0008] = 0x4E; // LSR ADR
          Memory[0x0009] = 0x33;
          Memory[0x000A] = 0x33;
          Memory[0x000B] = 0xFF;

          Memory[0x0101] = 0x05;
          Memory[0x0102] = 0x00;
          Memory[0x0421] = 0x1B;
          Memory[0x3333] = 0x9B;

          acc = 0xB1;
          printLoadedProgram();
          launchEmulation(0xB,1,1);

          assert(pc == 0x000B); // TEST JMP ADR
          assert(acc == 0xAA); // TEST EOR ADR
          assert(Memory[0x3333] == 0x4D); // TEST LSR ADR
          assert((state_register & CARRY) == CARRY); // TEST LSR ADR
  			  break;

        case 5://0x50 to 0x5f
          reinit_emulation();
          Memory[0x0000] = 0x50; //BVC
          state_register &= ~OVERFLOW;
          Memory[0x0001] = 0x10;
          Memory[0x0002] = 0x00;
          Memory[0x0010] = 0x51; // EOR (APZ),Y
          Memory[0x0011] = 0x40;
          Memory[0x0012] = 0xFF;
          Memory[0x0040] = 0x22;
          Memory[0x0041] = 0x22;
          Memory[0x2264] = 0x44;
          acc = 0x88;
          y_reg = 0x42;
          printLoadedProgram();
          launchEmulation(3,1,1);

          assert(pc == 0x12); // TEST BVC
          assert(acc == 0xCC); // TEST EOR (APZ),Y

          reinit_emulation();
          Memory[0x0000] = 0x55; // EOR APZ,X
          Memory[0x0001] = 0x05;
          Memory[0x0002] = 0x56; // LSR APZ,X
          Memory[0x0003] = 0x08;
          Memory[0x0004] = 0x58;
          Memory[0x0005] = 0xFF;
          Memory[0x000A] = 0x44;
          Memory[0x000D] = 0x44;
          acc = 0x88;
          x_reg = 0x05;
          state_register |= INTERRUPT;

          printLoadedProgram();
          launchEmulation(3,1,1);

          assert(acc == 0xCC); // TEST EOR APZ,X
          assert(Memory[0x000D] == 0x22); // TEST LSR APZ,X
          assert((state_register & INTERRUPT) == 0); // TEST CLI

          reinit_emulation();
          Memory[0x0000] = 0x59; //EOR ADR,Y
          Memory[0x0001] = 0x55;
          Memory[0x0002] = 0x21;
          Memory[0x0003] = 0xFF;
          Memory[0x2157] = 0x81;
          y_reg = 0x02;
          acc = 0x44;

          printLoadedProgram();
          launchEmulation(3,1,1);

          assert(acc == 0xC5);

          reinit_emulation();
          Memory[0x0000] = 0x5D; //EOR ADR,X
          Memory[0x0001] = 0x55;
          Memory[0x0002] = 0x21;
          Memory[0x0003] = 0x5E; //LSR ADR,X
          Memory[0x0004] = 0x55;
          Memory[0x0005] = 0x27;
          Memory[0x0006] = 0xFF;

          Memory[0x2157] = 0x81;
          Memory[0x2757] = 0x44;
          x_reg = 0x02;
          acc = 0x44;

          printLoadedProgram();
          launchEmulation(3,1,1);

          assert(acc == 0xC5);
          assert(Memory[0x2757] == 0x22);

			    break;

        case 6://0x60 to 0x6f
          reinit_emulation();
          Memory[0x0000] = 0x60; //RTS
          stack_push(0x22);
          stack_push(0x41);
          Memory[0x2242] = 0X58;
          Memory[0x2243] = 0xFF;

          launchEmulation(3,1,1);

          assert(pc == 0x2243); // TEST RTS

          reinit_emulation();
          Memory[0x0000] = 0x61; //ADC (APZ,X)
          Memory[0x0001] = 0x21;
          Memory[0x0002] = 0xFF;
          Memory[0x00C9] = 0x22;
          Memory[0x00CA] = 0x32;
          Memory[0x3222] = 0x88;
          x_reg = 0xA8;
          acc = 0x45;
          state_register |= CARRY;

          printLoadedProgram();
          launchEmulation(2,1,1);

          assert(acc == 0xBD);
          break;

        case 7://0x70 to 0x7f
			  break;

        case 8://0x80 to 0x8f
			  break;

        case 9://0x90 to 0x9f
			  break;

        case 0xA://0xa0 to 0xaf
			  break;

        case 0xB://0xb0 to 0xbf
			  break;

        case 0xC://0xc0 to 0xcf
			  break;

        case 0xD://0xd0 to 0xdf
			  break;

        case 0xE://0xe0 to 0xef
			  break;

        case 0xF://0xf0 to 0xff
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
