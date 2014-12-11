#ifndef EMU6502_H
#define EMU6502_H

typedef unsigned char byte;

#include "tools.h"

/* OPERATIONS FUNCTIONS */
byte inline opASL(byte mem);
void inline opORA(byte mem);
void inline opEOR(byte mem);
void inline opBRK();
void inline opRTI();
void inline opRTS();
void inline opPHP();
void inline opCLC();
void inline opSEC();
void inline opJSR();
void inline opADC(byte mem);
void inline opAND(byte mem);
void inline opBIT(byte mem);
byte inline opROL(byte mem);
byte inline opROR(byte mem);
byte inline opLSR(byte mem);
void inline opPLP();
void inline opBranch();
void inline opBPL();
void inline opBMI();
void inline opBVC();
void inline opBVS();
void inline opPHA();
void inline opPLA();
void inline opCLI();
void inline opJMP(unsigned short mem);

/* ADDRESSING MODES FUNCTIONS */
short unsigned adrIndexedIndirect();
short unsigned adrIndirectIndexed();
short unsigned adrZeroPage(byte offset, int mode);
byte adrImmediate();
short unsigned adrIndirect();
short unsigned adrAbsolute(byte offset, int mode);
/* DEBUG / TESTING FUNCTIONS */
void loadTestingProgram();
void launchEmulation(int Counter, int opCode, int debug);

#endif /* EMU6502_H */
