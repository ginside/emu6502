/*
 * cpu_variables.c
 *
 *  Created on: Apr 12, 2014
 *      Author: Daniel
 */
#include "emu6502.h"
#include "cpu_variables.h"

/* Processor initialisation */
//Registers\Memory Initialisation

// processor flags ( <-> State Register)
int NEGATIVE    = 128,
		OVERFLOW    = 64,
		BREAK       = 16,
		DECIMAL     = 8,
		INTERRUPT   = 4,
		ZERO        = 2,
		CARRY       = 1;

byte    //Memory declaration
		Memory[0xFFFF]      = { 0 },
		//Registers declaration
		state_register      = 32,

		// Index registers
		x_reg               = 0,
		y_reg               = 0,
		//accumulator
		acc                 = 0;
unsigned short
		// Program Counter
		pc                  = 0,
		// VECTORS
		irq_vect            = 0xFFFE,
		// stack is into the area of memory between 0x100 and 0x1ff,
		// starts at 1ff (going downwards)
		stack_pointer       = 0x1FF;
int
		// Cycle counting, for interruptions
		ic                  = 0,
		interrupt_period    = 0;


/* Byte Length for every opCode */
int opByteLength[256] = {
                //  0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
           /* 0 */  1 , 2 , 0 , 0 , 0 , 2 , 2 , 0 , 1 , 2 , 1 , 0 , 0 , 3 , 3 , 0 ,
           /* 1 */  2 , 2 , 0 , 0 , 0 , 2 , 2 , 0 , 0 , 3 , 0 , 0 , 0 , 3 , 3 , 0 ,
           /* 2 */  3 , 2 , 0 , 0 , 2 , 2 , 2 , 0 , 1 , 2 , 1 , 0 , 3 , 3 , 3 , 0 ,
           /* 3 */  2 , 2 , 0 , 0 , 0 , 2 , 2 , 0 , 1 , 3 , 0 , 0 , 0 , 3 , 3 , 0 ,
           /* 4 */  1 , 2 , 0 , 0 , 0 , 2 , 2 , 0 , 1 , 2 , 1 , 0 , 3 , 3 , 3 , 0 ,

           /* 5 */  2 , 2 , 0 , 0 , 0 , 2 , 2 , 0 , 1 , 3 , 0 , 0 , 0 , 3 , 3 , 0 ,

	         /* 6 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* 7 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* 8 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* 9 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* a */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* b */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* c */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* d */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* e */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* f */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
                    };

/* Cpu cycles required per opcode, Y axis is strong weight bit */
int cycles[256] = {
                //  0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
           /* 0 */  7 , 6 , 0 , 0 , 0 , 3 , 5 , 0 , 3 , 2 , 2 , 0 , 0 , 4 , 6 , 0 ,
           /* 1 */  2 , 5 , 0 , 0 , 0 , 4 , 6 , 0 , 0 , 4 , 0 , 0 , 0 , 4 , 7 , 0 ,
           /* 2 */  6 , 6 , 0 , 0 , 3 , 3 , 5 , 0 , 4 , 2 , 2 , 0 , 4 , 4 , 6 , 0 ,
           /* 3 */  2 , 5 , 0 , 0 , 0 , 4 , 6 , 0 , 2 , 4 , 0 , 0 , 0 , 4 , 7 , 0 ,
           /* 4 */  6 , 6 , 0 , 0 , 0 , 3 , 5 , 0 , 3 , 2 , 2 , 0 , 3 , 4 , 6 , 0 ,

           /* 5 */  2 , 5 , 0 , 0 , 0 , 4 , 6 , 0 , 2 , 4 , 0 , 0 , 0 , 4 , 7 , 0 ,

           /* 6 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* 7 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* 8 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* 9 */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* a */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* b */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* c */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* d */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* e */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
           /* f */  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
                    };

//
int VALUE = 0,
	ADDRESS = 1,
	LEFT = 0,
	RIGHT = 1;

enum instruction_types {
    NOP, //	Pas d'opération
    // Instruction de Transfert
    LDA, //	Chargement de l'accumulateur
    STA, //	Rangement de l'accumulateur
    LDX, //	Chargement de X
    STX, //	Rangement de X
    LDY, //	Chargement de Y
    STY, //	Rangement de Y
    TAX, //	Transfert de A dans X
    TXA, //	Transfert de X dans A
    TAY, //	Transfert de A dans Y
    TYA, //	Transfert de Y dans A
    TSX, //	Transfert de SP dans X
    // Instruction de Pile modifiant SP
    TXS, //	Transfert de X dans SP
    PHA, //	Empiler A
    PHP, //	Empiler P
    PLA, //	Dépiler vers A
    PLP, //	Dépiler vers SR
    // Instruction Arithmétique
    INC, //	Incrémentation mémoire
    DEC, //	Décrémentation mémoire
    INX, //	Incrémentation de X
    DEX, //	Décrémentation de X
    INY, //	Incrémentation de Y
    DEY, //	Décrémentation de Y
    ADC, //	Addition avec retenue
    SBC, //	Soustraction avec retenue
    // Instruction de Rotation
    ASL, //	Décalage a gauche
    LSR, //	Décalage a droite
    ROL, //	Rotation a gauche
    ROR, //	Rotation a droite
    // Instruction Logique
    ORA, //	Ou inclusif
    EOR, //	Ou exclusif
    AND, //	Et logique
    // Instruction du Registre d'Etat pouvant modifier seulement SR
     // Instruction d'initialisation de SR
    SEI, //	Inhibition des interruptions
    CLI, //	Autorisation des interruptions
    SED, //	Mise en mode décimal
    CLD, //	Annulation du mode décimal
    SEC, //	Mise a 1 de la retenue
    CLC, //	Annulation de la retenue
    CLV, //	Annulation de l'indicateur de débordement
      // Instruction de Test
    BIT, //	Test de bits
    CMP, //	Comparaison avec l'Accumulateur
    CPX, //	Comparaison avec X
    CPY, //	Comparaison avec Y
    // Instruction de Saut pouvant modifier le séquencement
    BCC, //	Branchement si pas de retenue
    BCS, //	Branchement si retenue
    BEQ, //	Branchement si zéro
    BMI, //	Branchement si négatif
    BNE, //	Branchement si non égal
    BPL, //	Branchement si positif ou nul
    BVC, //	Branchement si pas de débordement
    BVS, //	Branchement si débordement
    JMP, //	Saut inconditionnel
    JSR, //	Appel d'un sous-programme
    RTS, //	Retour de sous-programme
    BRK, //	Interruption logicielle
    RTI  //	Retour d'interruption
};
