#include "tools.h"

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
