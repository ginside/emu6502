#ifndef EMU6502_H
#define EMU6502_H

typedef unsigned char byte;

#include "tools.h"

/* OPERATIONS FUNCTIONS */

void inline opBranch();
void inline opCompare(byte mem, byte cmp);

void inline opADC(byte mem);
void inline opAND(byte mem);
byte inline opASL(byte mem);
void inline opBCC();
void inline opBCS();
void inline opBEQ();
void inline opBIT(byte mem);
void inline opBMI();
void inline opBNE();
void inline opBPL();
void inline opBRK();
void inline opBVC();
void inline opBVS();
void inline opCLC();
void inline opCLD();
void inline opCLI();
void inline opCLV();
void inline opCMP(byte mem);
void inline opCPX(byte mem);
void inline opCPY(byte mem);
byte inline opDEC(byte mem);
void inline opDEY();
void inline opDEX();
void inline opEOR(byte mem);
byte inline opINC(byte mem);
void inline opINX();
void inline opINY();
void inline opJMP(unsigned short mem);
void inline opJSR();
void inline opLDA(byte mem);
void inline opLDX(byte mem);
void inline opLDY(byte mem);
byte inline opLSR(byte mem);
void inline opNOP();
void inline opORA(byte mem);
void inline opPHA();
void inline opPHP();
void inline opPLA();
void inline opPLP();
byte inline opROL(byte mem);
byte inline opROR(byte mem);
void inline opRTI();
void inline opRTS();
void inline opSBC(byte mem);
void inline opSEC();
void inline opSED();
void inline opSEI();
void inline opSTA(unsigned short m_address);
void inline opSTX(unsigned short m_address);
void inline opSTY(unsigned short m_address);
void inline opTAX();
void inline opTAY();
void inline opTXA();
void inline opTXS();
void inline opTSX();
void inline opTYA();


/* ADDRESSING MODES FUNCTIONS */
short unsigned adrIndexedIndirect(int mode);       // (APZ,X)
short unsigned adrIndirectIndexed(int mode);       // (APZ),Y
short unsigned adrZeroPage(byte offset, int mode); // APZ ; APZ,X ; APZ,Y
byte adrImmediate();                               // #DON
short unsigned adrIndirect();                      // Ind
short unsigned adrAbsolute(byte offset, int mode); // ADR ; ADR,X ; ADR,Y
/* DEBUG / TESTING FUNCTIONS */
void loadTestingProgram();
void launchEmulation(int Counter, int opCode, int debug);

#endif /* EMU6502_H */
