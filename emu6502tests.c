#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "emu6502.h"
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

    int i;

    printf("TESTING PROGRAM LOADED:\n");
    switch(program_id){
        case 1:
            //testing instructions from 0x0 to 0x9
            Memory[0] = 0x6;//ASL APZ
            Memory[1] = 0xfa;//OPERAND 1
            Memory[2] = 0x8;//PHP
            Memory[3] = 0x5;
            Memory[4] = 0xad;
            Memory[5] = 0x8;
            Memory[6] = 0x9;
            Memory[7] = 0xa7;
            Memory[8] = 0xff;
            Memory[0xad] = 0x12;
            x_reg = 0xc7;
            Memory[0xfa] = 0x71;
            Memory[0x174] = 0xFF;
            acc = 0x1;
            break;
        case 2:
            //Testing instructions from 0xa to 0x10
            Memory[0] = 0x0d;
            Memory[1] = 0x1d;
            Memory[2] = 0xff;
            Memory[3] = 0x0e;
            Memory[4] = 0x1d;
            Memory[5] = 0xff;
            Memory[6] = 0x10;
            Memory[7] = 0xfb;
            Memory[8] = 0xff;
            Memory[0xff1d] = 0x1c;
            acc = 0xBe;
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
