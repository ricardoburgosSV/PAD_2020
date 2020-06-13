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

/* HINT ON IJVM METHOD INVOCATION
https://users.cs.fiu.edu/~prabakar/cda4101/Common/notes/lecture20.html
*/

// Frame initialization
void frame_init(frame_t *fr) {
	fr->size = 128;
	fr->prev = NULL;
	fr->frame = malloc(fr->size * sizeof(word_t));
	fr->lsp = 1;
	fr->psp = stack.sp;
	fr->ppc = get_program_counter();
	fr->nargs = 0;
	current_frm = fr;
}

// Frame memory allocation
void frm_alloc(frame_t *fr){
	fr->frame = malloc(fr->size * sizeof(word_t));
}

// Frame memory deallocation
void frm_free(frame_t *fr){
	free(fr->frame);
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
	byte_t nbr_args[2];
	nbr_args[0] = program[method_address];
	nbr_args[1] = program[method_address + 1];
	short number_of_args = to_be_16(nbr_args);
	// Get number of local variables for the calling method
	byte_t nbr_vars[2];
	nbr_vars[0] = program[method_address + 2];
	nbr_vars[1] = program[method_address + 3];
	short local_var_size = to_be_16(nbr_vars);

	// Create and initialize a new local frame
	frame_t *local = malloc(sizeof(frame_t));
	local->size = 2 + number_of_args + local_var_size;
	frm_alloc(local);
	local->prev = current_frm;
	// fprintf(stderr, "INVOKEVIRTUAL CFRM: %p\n", current_frm);
	local->lsp = 1;
	local->psp = stack.sp;
	local->ppc = get_program_counter();
	local->nargs = number_of_args;

	// Push OBJREF to local frame
	local->frame[0] = cp_reference;
	local->lsp++;

	// Push method arguments to local frame
	for (short i = number_of_args; i > 0; i--){
		local->frame[i] = pop();
		local->lsp++;
	}

	// Push local variables to local frame
	if (local_var_size > 0){
		for (short i = number_of_args + 1; i <= number_of_args + local_var_size; i++){
			local->frame[i] = get_local_variable(i);
			local->lsp++;
		}
	}

	// Set the program counter to the first method instruction
	// and update current frame pointer
	*ppc = method_address + 4;
	current_frm = local;
}

void IRETURN(){
	// IRETURN INSTRUCTION (OPCODE 0xAC)
	// Return from method with a word value

	// fprintf(stderr, "IRETURN IN\n");

	// Push returned value to local frame
	current_frm->frame[current_frm->lsp] = pop();

	// Restore previous stack pointer
	stack.sp = current_frm->psp - current_frm->nargs;

	// Overwrite the pushed OBJREF with the returned value
	pop(); // pops OBJREF off the stack
	// Pushes returned value at the top of local frame
	push(current_frm->frame[current_frm->lsp]);

	// Restore previous program counter
	*ppc = current_frm->ppc;

	// restore current frame pointer to previous frame
	frame_t *used_frm = current_frm;
	current_frm = current_frm->prev;
	// fprintf(stderr, "IRETURN PFRM: %p\n", current_frm);
	
	// release memory
	free(used_frm);

	// fprintf(stderr, "IRETURN OUT\n");
}
