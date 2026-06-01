#include "../include/utils.h"

char dec_to_hex(uint8_t val){
	switch(val){
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 10:
			return 'a';
		case 11:
			return 'b';
		case 12:
			return 'c';
		case 13:
			return 'd';
		case 14:
			return 'e';
		case 15:
			return 'f';
	}
	
	return 'x';
}

char *hexadecimal_equivalent(unsigned char ch, char hex[]){
	uint8_t ch_val = (uint8_t)ch;
	
	hex[0] = dec_to_hex(ch_val / 16);
	hex[1] = dec_to_hex(ch_val % 16);
	
	return hex;
}

char *byte_to_hex(unsigned char *sha_val){

	char *hash_val = (char *)malloc(41 * sizeof(char));
	char hex[2];
	
	size_t j = 0;
	for(size_t i = 0; i<20; i++){
		hexadecimal_equivalent(sha_val[i], hex);
		hash_val[j++] = hex[0];
		hash_val[j++] = hex[1];
	}
	
	hash_val[40] = '\0';
	
	return hash_val;
}
