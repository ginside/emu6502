#ifndef EMU6502_H
#define EMU6502_H

typedef unsigned char byte;

#include "tools.h"

/* OPERATIONS FUNCTIONS */
byte inline opASL(byte value);
void opORA(byte mem);
void inline opBRK();
void inline opPHP();
void inline opBPL();
void inline opCLC();
void inline opBranch();
/* ADDRESSING MODES FUNCTIONS */
short unsigned adrIndexedIndirect();
short unsigned adrIndirectIndexed();
short unsigned adrZeroPage(byte offset, int mode);
byte adrImmediate();
short unsigned adrAbsolute(byte offset, int mode);

/* DEBUG / TESTING FUNCTIONS */
void loadTestingProgram();
void launchEmulation(int Counter, int opCode, int debug);

#endif /* EMU6502_H */
