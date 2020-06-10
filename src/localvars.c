#include <ijvm.h>
#include <stack.h>
#include <flowcontrol.h>
#include <method.h>
#include <localvars.h>

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

// LOCAL VARIABLES
// ===============

void LDC_W(uint16_t index){
	// LDC_W INSTRUCTION (OPCODE 0x13)
	// Push constant from constant pool onto stack
	word_t word = get_constant((int)index);
	push(word);
}

void ILOAD(word_t index){
	// ILOAD INSTRUCTION (OPCODE 0x15)
	// Push local variable onto stack
	word_t lvar = get_local_variable(index);
	push(lvar);
}

void ISTORE(word_t index){
	// ISTORE INSTRUCTION (OPCODE 0x36)
	// Pop word from stack and store in local variable
	lframe.frame[index] = pop();
}

void IINC(word_t index, word_t cons){
	// IINC INSTRUCTION (OPCODE 0x84)
	// Add a constant value to a local variable.
	// The first byte is the variable index
	// The second byte is the constant
	lframe.frame[index] += (char)cons;
}

void WIINC(short index, word_t cons){
	// WIDE IINC
	lframe.frame[(uint16_t)index] += (char)cons;
}

void WIDE(word_t inst, short index){
	// WIDE INSTRUCTION (OPCODE 0xC4)
	// Prefix instruction: next instruction has a 16-bit index
	switch (inst){
		case OP_ILOAD: // WIDE ILOAD
			ILOAD((uint16_t)index);
			break;
		case OP_ISTORE: // WIDE ISTORE
			ISTORE((uint16_t)index);
			break;
	}
}
