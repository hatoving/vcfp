#ifndef __BIT_UTIL__
#define __BIT_UTIL__

#include <stdio.h>
#include <stdbool.h>
#include <psp2/kernel/clib.h>

static int bit_position = 0;
static int bit_length = 0;
static bool bit_op_flag = false;

uint32_t read_ubits(FILE* file, int num_bits) {
    int i = 0;
    unsigned int value = 0;
	unsigned int _bitBuffer;
	
	// FIXME, numBits must <= 32
	
    if (!bit_op_flag) {
        bit_op_flag = 1;
        bit_length = 0; // reset _bitBuffer
    }

    while (i < num_bits) {
		
        if (bit_length == 0) {
			_bitBuffer = fgetc(file);
            bit_length = 8;
            bit_position++;
        }
		
        value = (value << 1) | ((_bitBuffer >> --bit_length) & 0x01); // get MSb
        i++;
    }
    
    return value;
}

int read_sbits(FILE* file, int num_bits) {
    int value = 0;
	
	value = read_ubits(file, num_bits);
	
	if (value & (1 << (num_bits - 1)))
		value |=  (-1 << num_bits); // sign extension
	
    return value;
}

#endif