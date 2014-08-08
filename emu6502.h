#ifndef EMU6502_H
#define EMU6502_H

typedef unsigned char byte;


/* OPERATIONS FUNCTIONS */
byte inline opASL(byte value);

/* DEBUG / TESTING FUNCTIONS */
void loadTestingProgram();
void launchEmulation();

#endif /* EMU6502_H */
