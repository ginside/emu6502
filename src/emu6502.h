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

/* ADDRESSING MODES FUNCTIONS */
short unsigned adrIndexedIndirect();
short unsigned adrZeroPage();
byte adrImmediate();
short unsigned adrAbsolute();

/* DEBUG / TESTING FUNCTIONS */
void loadTestingProgram();
void launchEmulation(int Counter, int opCode, int debug);

#endif /* EMU6502_H */
