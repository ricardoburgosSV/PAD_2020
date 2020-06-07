#include <ijvm.h>
#include <stack.h>
#include <flowcontrol.h>

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

// IJVM BRANCHING & FLOW CONTROL INSTRUCTIONS
// ==========================================

void GOTO(short offset){
	// GOTO INSTRUCTION (OPCODE 0xA7)
	// Unconditional jump; short argument (2 bytes: 16 bits)
	// add an offset to the program counter
	*aux_pc += (int)offset;
}

void IFEQ(short offset){
	// IFEQ INSTRUCTION (OPCODE 0x99)
	// Pop word from stack and branch if it is zero; short argument (2 bytes: 16 bits)
	word_t a = pop();
	if (a == OP_NOP){
		GOTO(offset);
	} else {
		*aux_pc += 3;
	}
}

void IFLT(short offset){
	// IFLT INSTRUCTION (OPCODE 0x9B)
	// Pop a word from stack and branch if it is less than zero; short argument (2 bytes: 16 bits)
	word_t a = pop();
	if (a < 0){
		GOTO(offset);
	} else {
		*aux_pc += 3;
	}
}

void ICMPEQ(short offset){
	// ICMPEQ INSTRUCTION (OPCODE 0x9F)
	// Pop two words from the stack and branch if they are equal; short argument (2 bytes: 16 bits)
	word_t a, b;
	a = pop();
	b = pop();
	if (a == b){
		GOTO(offset);
	} else {
		*aux_pc += 3;
	}
}
