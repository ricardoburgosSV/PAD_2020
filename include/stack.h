#ifndef STACK_H
#define STACK_H

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

// STACK IMPLEMENTATION
// ====================

typedef struct{
	int size;		// Stack size
	int sp;			// Stack pointer
	word_t *data; 	// variable length stack
} stack_t;

stack_t stack;
FILE *in_f;		// Pointer to input file
FILE *out_f;	// Pointer to output file
int *aux_pc; 	// Auxiliary pointer to program counter

// ESSENTIAL STACK OPERATIONS:
// ===========================

void stack_init(void); // Initializes the stack

void push(word_t n); // PUSH

word_t pop(void); // POP

word_t peek(void); // TOP

int size(void); // SIZE


// BASIC IJVM MANIPULATION INSTRUCTIONS
// ====================================

// IN INSTRUCTION (OPCODE 0XFC)
void IN(void);

// OUT INSTRUCTION (OPCODE 0xFD)
void OUT(void);

// IADD INSTRUCTION (OPCODE 0x60)
void IADD(void);

// ISUB INSTRUCTION (OPCODE 0x64)
void ISUB(void);

// IAND INSTRUCTION (OPCODE 0x7E)
void IAND(void);

// IOR INSTRUCTION (OPCODE 0xB0)
void IOR(void);

// BIPUSH INSTRUCTION (OPCODE 0x10)
void BIPUSH(word_t a);

// SWAP INSTRUCTION (OPCODE Ox5F)
void SWAP(void);

// POP INSTRUCTION (OPCODE Ox57)
void POP(void);

// DUP INSTRUCTION (OPCODE 0x0x59)
void DUP(void);


// UTILITY METHODS
// ===============

// Prints an array of bytes as hexadecimal to the standard output
void print_hex(byte_t *barr, int size);

//  Returns the stack size of the current frame
int stack_size(void);

// Doubles the size of the stack when it becomes full
void stack_expand(void);

#endif /* STACK_H */
