#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ijvm.h>
#include <endian.h>
#include <stack.h>
#include <localvars.h>
#include <flowcontrol.h>
#include <method.h>

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

int pc; // Program Counter

// BUFFER STRUCTURE DEFINITION
typedef struct buffer{
	// This buffer stores data read from the input binary file
	word_t origin; 	// 32-bit memory address
	word_t size; 	// 32-bit number for block size (in bytes)
	byte_t *data; 	// variable lenght data array
} buffer_t;

buffer_t read_block(FILE *fp){
	// Reads a block from binary file "fp" and stores data in a buffer struct
	
	buffer_t myBuffer;
	word_t block_origin, block_size;

	fread(&block_origin, sizeof(word_t), 1, fp);
	fread(&block_size, sizeof(word_t), 1, fp);

	// Swap to big endian
	myBuffer.origin = swap_uint32(block_origin);
	myBuffer.size = swap_uint32(block_size);

	// Allocate space for data
	myBuffer.data = malloc(myBuffer.size * sizeof(byte_t));

	// Read data from file
	fread(myBuffer.data, sizeof(byte_t), myBuffer.size, fp);

	return myBuffer;
}

buffer_t constant_block, data_block;

// IJVM INITIALIZATION
int init_ijvm(char *binary_file){
	// Initializes the IJVM with the binary file found at the provided argument
	// Note. You need to be able to re-initialize the IJVM after it has been started.
	// Returns  0 on success
	//         -1 on failure

	FILE *fp;
	word_t magicNumber;

	// Initialize input-output variables
	in_f = stdin;
	out_f = stdout;

	fp = fopen(binary_file, "rb"); // Open binary file

	if (fp == NULL){
		return -1;
	}
	pc = 0;	 // initialize program counter

	// Read magicNumber from file
	fread(&magicNumber, sizeof(word_t), 1, fp);
	magicNumber = swap_uint32(magicNumber);

	if (magicNumber != MAGIC_NUMBER){
		return -1; // Not a IJVM binary file
	}
	
	// Read 2 blocks from binary file
	constant_block = read_block(fp);
	data_block = read_block(fp);

	stack_init();			// Initialize stack
	frame_init(&lframe);	// Initialize frame
	fclose(fp); 			// Close file

	return 0;
}

byte_t get_instruction(){
	/* @return The value of the current instruction represented as a byte_t.
	This should NOT increase the program counter. */
	return data_block.data[pc];
}

void destroy_ijvm(){
	// Reset IJVM state
	free(constant_block.data);
	free(data_block.data);
	free(stack.data);
	free(current_frm->frame);
}

void run(){
	// Step while you can
	while (step()){	}
}

void set_input(FILE *f){
	// Sets the input of the IJVM instance to the provided file
	in_f = f;
}

void set_output(FILE *f){
	// Sets the output of the IJVM instance to the provided file
	out_f = f;
}

word_t get_local_variable(int i){
	 // @param i, index of variable to obtain.
	 // @return Returns the i:th local variable of the current frame.
	return current_frm->frame[i];
}

word_t get_constant(int i){
	// @param i, index of the constant to obtain
	// @return The constant at location i in the constant pool.
	if (i > 0){
		i = i * 4;
	}
	byte_t a[4];
	for (int j = 0; j < 4; j++){
		a[j] = constant_block.data[i + j];
	}
	return to_big_end(a);
}

bool step(){
	aux_pc = &pc; // Pointer to the program counter
	byte_t arg[2]; // Stores argument data for a short

	switch (get_instruction()){
		case OP_BIPUSH: //BIPUSH (0x10)
			BIPUSH((int8_t)data_block.data[pc + 1]);
			pc++;
			break;
		case OP_DUP: //DUP (0x59)
			DUP();
			break;
		case OP_ERR: //ERR (0xFE)
			printf("*** AN ERROR HAS OCCURRED! ***\n");
			return false;
			break;
		case OP_GOTO: //GOTO (0xA7)
			arg[0] = data_block.data[pc + 1];
			arg[1] = data_block.data[pc + 2];
			GOTO(to_be_16(arg));
			pc--;
			break;
		case OP_HALT: //HALT (0xFF)
			return false;
			break;
		case OP_IADD: //IADD (0x60)
			IADD();
			break;
		case OP_IAND: //IAND (0x7E)
			IAND();
			break;
		case OP_IFEQ: //IFEQ (0x99)
			arg[0] = data_block.data[pc + 1];
			arg[1] = data_block.data[pc + 2];
			IFEQ(to_be_16(arg));
			pc--;
			break;
		case OP_IFLT: //IFLT (0x9B)
			arg[0] = data_block.data[pc + 1];
			arg[1] = data_block.data[pc + 2];
			IFLT(to_be_16(arg));
			pc--;
			break;
		case OP_ICMPEQ: //ICMPEQ (0x9F)
			arg[0] = data_block.data[pc + 1];
			arg[1] = data_block.data[pc + 2];
			ICMPEQ(to_be_16(arg));
			pc--;
			break;
		case OP_IINC: //IINC (0x84)
			IINC(data_block.data[pc + 1], data_block.data[pc + 2]);
			pc += 2;
			break;
		case OP_ILOAD: //ILOAD (0x15)
			ILOAD(data_block.data[pc + 1]);
			pc++;
			break;
		case OP_IN: //IN (0xFC)
			IN();
			break;
		case OP_INVOKEVIRTUAL: //INVOKEVIRTUAL (0xB6)
			arg[0] = data_block.data[pc + 1];
			arg[1] = data_block.data[pc + 2];
			ppc = &pc;
			INVOKEVIRTUAL(to_be_16(arg));
			pc--;
			break;
		case OP_IOR: //IOR (0xB0)
			IOR();
			break;
		case OP_IRETURN: //IRETURN (0xAC)
			IRETURN();
			pc += 2;
			break;
		case OP_ISTORE: //ISTORE (0x36)
			ISTORE(data_block.data[pc + 1]);
			pc++;
			break;
		case OP_ISUB: //ISUB (0x64)
			ISUB();
			break;
		case OP_LDC_W: //LDC_W (0x13)
			arg[0] = data_block.data[pc + 1];
			arg[1] = data_block.data[pc + 2];
			LDC_W(to_be_u16(arg));
			pc += 2;
			break;
		case OP_NOP: //NOP (0x00)
			// Do nothing
			break;
		case OP_OUT: //OUT (0xFD)
			OUT();
			break;
		case OP_POP: //POP (0x57)
			POP();
			break;
		case OP_SWAP: //SWAP (0x5F)
			SWAP();
			break;
		case OP_WIDE: //WIDE (0xC4)
			arg[0] = data_block.data[pc + 2];
			arg[1] = data_block.data[pc + 3];
			if (data_block.data[pc + 1] == OP_IINC){
				WIDE_IINC(to_be_u16(arg), data_block.data[pc + 4]);
				pc += 4;
			} else {
				WIDE(data_block.data[pc + 1], to_be_u16(arg));
				pc += 3;
			}
			break;
		default:
			//DEFAULT: Invalid instruction
			return false;
	}
	
	// Check if pc has reached the end of file
	if (text_size() == pc){
		return false;
	}

	pc++;
	return true;
}

int text_size(){
	// Returns the size of the currently loaded program text.
	return data_block.size;
}

byte_t *get_text(){
	// Returns the currently loaded program text as a byte array.
	return data_block.data;
}

int get_program_counter(){
	// Returns the value of the program counter (as an offset from the first instruction).
	return pc;
}

bool finished(){
	// Check whether the machine has any more instructions to execute.
	// A machine will stop running after:
	// * reaching the end of the text section
	// * encountering either the HALT/ERR instructions
	// * encountering an invalid instruction
	return true;
}
