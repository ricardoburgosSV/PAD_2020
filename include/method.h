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
typedef struct frame{
	unsigned short size;		// Frame size
	word_t *frame;				// Variable length frame
	struct frame *prev;		 	// Pointer to previous frame
	int lsp;					// Local stack pointer
	int psp;					// Previous stack pointer
	int ppc;					// Previous program counter
	short nargs;				// Number of arguments
} frame_t;

frame_t lframe; 				// Frame for main method
frame_t aux_frame;				// Method's local frame
frame_t *current_frm; 			// Current frame pointer
int *ppc;						// Pointer to PC

// INITIALIZE LOCAL FRAME
void frame_init(frame_t *fr);

// IVOKEVIRTUAL INSTRUCTION (OPCODE 0XB6)
void INVOKEVIRTUAL(short cp_reference);

// IRETURN INSTRUCTION (OPCODE 0XAC)
void IRETURN(void);

// Doubles the size of the frame if it becomes full
void frame_expand(frame_t *f);

#endif /* METHOD_H */
