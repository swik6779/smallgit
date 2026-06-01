#include "../include/object.h"
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

struct blob_details *get_file_det(const char *buffer){

	struct blob_details *det = (struct blob_details *)malloc(sizeof(struct blob_details));
	
	unsigned long file_size = 0;
	size_t idx_before_null;
	for(size_t i=5; buffer[i] != '\0'; i++){
		if(buffer[i+1] == '\0'){
			idx_before_null = i;
		}
	}
	
	for(size_t i=5; i<=idx_before_null; i++){
		uint8_t digit = (uint8_t)(buffer[i] - '0');
		file_size = file_size*10 + digit;
	}
	
	uint8_t header_len = 5 + (idx_before_null - 5 + 1) + 1;
	
	det->header_len = header_len;
	det->file_size = file_size;
	
	return det;
}

unsigned char *getcontnts(const char *filepath){ 
	// hash-object.c will extract filepath from the filename. 

	size_t file_size;
	struct stat st;
	if(stat(filepath, &st) == 0){
		file_size = st.st_size;
	}
	else{
		fprintf(stderr, "extract file-size failed\n");
		exit(EXIT_FAILURE);
	}
	
	unsigned char size_in_str[SIZE_BITS];
	snprintf(size_in_str, SIZE_BITS, "%zu", file_size);
	
	int bits_for_digits = 0;
	
	for(int i=0;i<SIZE_BITS;i++){
		if(size_in_str[i] < '0' || size_in_str[i] > '9'){
			break;
		}
		else{
			bits_for_digits++;
		}
	}
	
	size_t header_len = 5 + bits_for_digits + 1;
	size_t buffer_len = file_size + header_len + 1;
	
	unsigned char *buffer = (unsigned char *)malloc(buffer_len * sizeof(unsigned char));
	
	buffer[0] = 'b'; buffer[1] = 'l'; buffer[2] = 'o'; buffer[3] = 'b'; buffer[4] = ' ';
	
	for(int i=0;i<bits_for_digits;i++){
		buffer[5 + i] = size_in_str[i];
	}
	buffer[5 + bits_for_digits] = '\0';
	
	FILE *fptr;
	fptr = fopen(filepath, "r");
	
	if(fptr == NULL){
		fprintf(stderr, "file couldn't be opened");
	}
	
	int ch;
	size_t iter = 5 + bits_for_digits + 1;
	
	while((ch = fgetc(fptr)) != EOF){
		buffer[iter++] = (unsigned char)ch;
	}
	buffer[iter] = '\0';
	
	if(fclose(fptr) != 0){
		fprintf(stderr, "error closing file\n");
		exit(EXIT_FAILURE);
	}
	
	return buffer;
}

unsigned char *sha_encoding(const unsigned char *buffer){
	
	struct blob_details *det = get_file_det(buffer);
	unsigned long buffer_size = det->header_len + det->file_size;
	
	unsigned char *sha_val = (unsigned char *)malloc(21 * sizeof(unsigned char));
	sha_val[20] = '\0';
	
	SHA1(buffer, buffer_size, sha_val);
	
	return sha_val;
}

int main(){
	char *buffer = getcontnts("test");
	struct blob_details *det = get_file_det(buffer);
	
	printf("header_len: %hu\n", det->header_len);
	printf("filesize: %lu\n", det->file_size);
	
	printf("%s\n", buffer);
	for(int i=det->header_len; buffer[i]!='\0'; i++){
		printf("%c", buffer[i]);
	}
	
	unsigned char *sha_val = sha_encoding(buffer); 
	printf("%s\n", sha_val);
	
	for(int i=0;i<20;i++){
		printf("%hu\n", sha_val[i]);
	}
	
	char *hash_val = byte_to_hex(sha_val);
	for(int i=0;i<40;i++){
		printf("%c", hash_val[i]);
	}
	
	return 0;
}
