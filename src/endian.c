#include <ijvm.h>
#include <endian.h>

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

// BE AWARE OF THE FOLLOWING BYTE ORDERS:
// x86 ARCHITECTURE --> LITTLE-ENDIANNESS
// JAVA-IJVM ARCHITECTURE --> BIG-ENDIANNESS

// Swaps integers from little-endian to big-endian
word_t swap_uint32(word_t num){
	return 	((num>>24)&0xff) | 
			((num<<8)&0xff0000) | 
			((num>>8)&0xff00) | 
			((num<<24)&0xff000000);
}

// Convert a 4-byte array to a big-endian integer
int to_big_end(byte_t *bytes){
	return 	((bytes[0] & 0xFF) << 24) | 
			((bytes[1] & 0xFF) << 16) | 
			((bytes[2] & 0xFF) << 8) | 
			(bytes[3] & 0xFF);
}

// Convert a 4-byte array to a little-endian integer
int to_little_end(byte_t *bytes){
	return 		bytes[0] + 
				(bytes[1] << 8) + 
				(bytes[2] << 16) + 
				(bytes[3] << 24);
}

// Convert a 2-byte array to a big-endian a signed short integer (int16_t)
int16_t to_be_16(byte_t *bytes){
	return	((bytes[0] & 0xFF) << 8) | (bytes[1] & 0xFF);
}

// Convert a 2-byte array to a little-endian signed short integer (int16_t)
int16_t to_le_16(byte_t *bytes){
	return	bytes[0] + (bytes[1] << 8);
}

// Convert a 2-byte array to a big-endian unsigned short integer (uint16_t)
uint16_t to_be_u16(byte_t *bytes){
	return	((bytes[0] & 0xFF) << 8) | (bytes[1] & 0xFF);
}
