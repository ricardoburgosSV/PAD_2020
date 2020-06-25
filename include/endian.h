#ifndef ENDIAN_H
#define ENDIAN_H
#include <ijvm.h>
#include <stdint.h>

/*
VRIJE UNIVERSITEIT AMSTERDAM
FACULTY OF SCIENCE
B. COMPUTER SCIENCE
COURSE: PROJECT APPLICATION DEVELOPMENT (X_400556)
RICARDO BURGOS LARA (2627499)
JUNE 2020
*/

// Swap to 32-bit IJVM words
uint32_t swap_uint32(uint32_t num);

// 4-Byte array to big endian
int to_big_end(byte_t *bytes);

// 4-Byte array to little endian
int to_little_end(byte_t *bytes);

// 2-Byte array to big endian
int16_t to_be_16(byte_t *bytes);

// 2-Byte array to little endian
int16_t to_le_16(byte_t *bytes);

uint16_t to_be_u16(byte_t *bytes);

#endif /* ENDIAN_H */
