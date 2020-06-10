#ifndef METHOD_H
#define METHOD_H

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

// LOCAL FRAME IMPLEMENTATION
// ==========================
typedef struct{
	// local_frame_t *prev_frame; // Pointer to previous frame
	int size;			// Frame size
	int lsp;			// Local stack pointer
	int lvp;			// Local variable pointer
	word_t psp;			// Previous stack pointer
	int ppc;			// previous program counter
	word_t *frame;		// Variable length frame
} local_frame_t;

local_frame_t lframe;	// Local frame
int *ppc;				// Pointer to PC
int nargs;				// Number of arguments the method takes

// INITIALIZE LOCAL FRAME
void frame_init(local_frame_t *fr);

// IVOKEVIRTUAL INSTRUCTION (OPCODE 0XB6)
void INVOKEVIRTUAL(short cp_reference);

// IRETURN INSTRUCTION (OPCODE 0XAC)
void IRETURN(void);

// Doubles the size of the frame if it becomes full
void frame_expand(local_frame_t *f);

#endif /* METHOD_H */
