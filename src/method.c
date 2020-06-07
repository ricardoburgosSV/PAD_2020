#include <stdlib.h>
#include <ijvm.h>
#include <stack.h>
#include <localvars.h>
#include <method.h>
#include <endian.h>

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

/*
HINT ON IJVM METHOD INVOCATION
https://users.cs.fiu.edu/~prabakar/cda4101/Common/notes/lecture20.html
*/

// Frame initialization
void frame_init(local_frame_t *fr){
	fr->size = 128;
	fr->lsp = 0; //stack.sp;
	fr->lvp = 1; //stack.sp + 1;
	fr->psp = stack.sp;
	fr->ppc = get_program_counter();
	fr->frame = malloc(fr->size * sizeof(word_t));
}

// Frame resize: doubles the frame size
void frame_expand(local_frame_t *f){
	f->size *= 2;
	f->frame = realloc(f->frame, f->size * sizeof(word_t));
}

void INVOKEVIRTUAL(short cp_reference){
	// IVOKEVIRTUAL INSTRUCTION (OPCODE 0xB6)
	// Invoke a method, pops object reference and pops arguments from stack
	// Argument type: short integer (arg is a reference to a pointer in the constant pool)

	push(cp_reference); // Push OBJREF onto the stack
	
	// Get the address of the method code from the constant pool
	word_t method_address = get_constant((int)cp_reference);

	// Get the loaded program text as an array of bytes
	byte_t *program = get_text();

	// Get number of arguments for the calling method
	byte_t aux[2];
	aux[0] = program[method_address];
	aux[1] = program[method_address + 1];
	short number_of_args = to_be_16(aux);

	// Get number of local variables for the calling method
	aux[0] = program[method_address + 2];
	aux[1] = program[method_address + 3];
	short local_var_size = to_be_16(aux);

	// Initialize a new local frame
	frame_init(&lframe);

	// Push OBJREF to local frame
	lframe.frame[0] = cp_reference;
	lframe.lsp++;

	// Push method arguments to local frame
	for (short i = number_of_args; i > 0; i--){
		lframe.frame[i] = pop();
		lframe.lsp++;
	}

	// Set space for local variables in local frame
	if (local_var_size > 0){
		for (short i = number_of_args + 1; i <= number_of_args + local_var_size; i++){
			lframe.lsp++;
		}
	}

	// Push caller's PC to local frame
	lframe.frame[lframe.lsp] = lframe.ppc;
	lframe.lsp++;

	// Set the program counter to the first method instruction
	*ppc = method_address + 4;

	// Pass the number of arguments to IRETURN to update stack pointer
	nargs = number_of_args;
}

void IRETURN(){
	// IRETURN INSTRUCTION (OPCODE 0xAC)
	// Return from method with a word value

	// Push returned value to local frame
	lframe.frame[lframe.lsp] = pop();

	// Restore previous stack pointer - number of arguments took by INVOKEVIRTUAL
	stack.sp = lframe.psp - nargs;

	// Overwrite the pushed OBJREF with the returned value
	pop(); // pops OBJREF off the stack
	push(lframe.frame[lframe.lsp]); // Pushes returned value at the top of local frame

	// Restore previous program counter
	*ppc = lframe.ppc;
	// printf("PC restored to: %d\n", *ppc);
}
