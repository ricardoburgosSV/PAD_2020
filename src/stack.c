#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ijvm.h>
#include <stack.h>
#include <method.h>

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

// HINT ON INTEGER ENCODING
// https://cryptii.com/pipes/integer-encoder

/* STACK INITIALIZATION */

void stack_init(){ //INITIALIZE STACK
	stack.size = 1024;
	stack.sp = 0;
	stack.data = malloc(stack.size * sizeof(word_t));
}

/* ESSENTIAL STACK OPERATIONS */

void push(word_t n){ //PUSH
	// Adds an element to the top of the stack
	// Increment stack pointer
	assert(stack.sp < stack.size-1);
	stack.data[++stack.sp] = n;

	// Doubles the stack size if it becomes full
	if (stack.sp == stack.size-2){
		stack_expand();
	}
}

word_t pop(){ //POP
	// Remove the element at the top of the stack and returns it
	// Decrement stack pointer
	assert(stack.sp > 0);
	return stack.data[stack.sp--];
}

word_t peek(){ //TOP
	// Return the element at the top of the stack without removing it
	assert(stack.sp > 0);
	return stack.data[stack.sp];
}

int size(){ //SIZE
	// Returns the size of the stack
	return stack.size;
}

word_t tos(){
	// TOP OF STACK
	// this function should return the word at the top of the stack of the current
	// frame, interpreted as a signed integer.
	return peek();
	// return current_frm->frame[current_frm->lsp];
}

word_t *get_stack(){
	// Should return a pointer to the beginning of the stack
	/**
	 * Returns the stack of the current frame as an array of integers,
	 * with entry[0] being the very bottom of the stack and
	 * entry[stack_size() - 1] being the top.
	 **/
	return &stack.data[0];
}

/* BASIC IJVM STACK MANIPULATION INSTRUCTIONS */

void IN(){
	// IN INSTRUCTION (OPCODE 0XFC)
	// Reads a character from the input and pushes it onto the stack
	// If no character is available, 0 is pushed
	char a = fgetc(in_f);
	if (a == -1 || (int)(a) > 255){
		push(0);
	} else{
		push(a);
	}
}

void OUT(){
	// OUT INSTRUCTION (OPCODE 0xFD)
	// Pop word off stack and print it to standard out
	// Output the word at the top of the stack interpreted as ASCII to the
	// output specified by the method set_output()
	char a = (char)pop();
	fprintf(out_f, "%c", a);
}

void IADD(){
	// IADD INSTRUCTION (OPCODE 0x60)
	// Pops two words from stack; push their sum
	word_t a, b;
	a = pop();
	b = pop();
	push(a + b);
}

void ISUB(){
	// ISUB INSTRUCTION (OPCODE 0x64)
	// Pop two words from stack; subtract the top word
	// from the second to top word, push the answer
	word_t a, b;
	a = pop();
	b = pop();
	push(b - a);
}

void IAND(){
	// IAND INSTRUCTION (OPCODE 0x7E)
	// Pop two words from stack; push bit-wise AND
	word_t a, b;
	a = pop();
	b = pop();
	push(a & b);
}

void IOR(){
	// IOR INSTRUCTION (OPCODE 0xB0)
	// Pop two words from stack; push bit-wise OR
	word_t a, b;
	a = pop();
	b = pop();
	push(a | b);
}

void BIPUSH(word_t a){
	// BIPUSH INSTRUCTION (OPCODE 0x10)
	// Push a byte onto stack
	push(a);
}

void SWAP(){
	// SWAP INSTRUCTION (OPCODE 0x5F)
	// Swap the two top words on the stack
	word_t a, b;
	a = pop();
	b = pop();
	push(a);
	push(b);
}

void POP(){
	// POP INSTRUCTION (OPCODE Ox57)
	// Delete word from the top of the stack
	pop();
}

void DUP(){
	// DUP INSTRUCTION (OPCODE 0x0x59)
	// Copy top word on stack and push onto stack
	word_t a;
	a = peek();
	push(a);
}

/* =============== */
/* UTILITY METHODS */
/* =============== */

void print_hex(byte_t *barr, int size){
	// Prints an array of bytes as hexadecimal to the standard output
	for (size_t i = 0; i < size; i++){
		fprintf(stdout,"0x%02x ", barr[i]);
		if (i % 8 == 7){
			fprintf(stdout,"\n");
		}
	}
	fprintf(stdout,"\n");
}

int stack_size(){
	// Returns the size of the stack of the current frame.
	return size();
}

void stack_expand(){
	// Doubles the size of the stack when it becomes full
	stack.size *= 2;
	stack.data = realloc(stack.data, stack.size * sizeof(word_t));
}
