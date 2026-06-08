#include "../include/object.h"

/*
	This function gets the contents of a file. This is the first step of the hash-object function.
	the format of output:
	
	blob <filesize>\0
	--file contents 
*/

unsigned char *getcontnts(const char *filepath){ 

	size_t file_size;
	struct stat st;
	if(0 == stat(filepath, &st)){
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
	fptr = fopen(filepath, "rb");
	
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

/*
	get_file_det gives two particular details: the length of the header, and the file size 
	of the file whose contents are in the const char *buffer.
	
	These pieces of info are used for sha_encoding and the zlib_compression functions
*/

struct blob_details *get_file_det(const char *buffer){

	struct blob_details *det = (struct blob_details *)malloc(sizeof(struct blob_details));
	
	unsigned long file_size = 0;
	size_t idx_before_space;
	size_t idx_before_null;
	
	for(size_t i=0; buffer[i] != ' '; i++){
		if(buffer[i+1] == ' '){
			idx_before_space = i;
		}
	}
	
	for(size_t i=0; buffer[i] != '\0'; i++){
		if(buffer[i+1] == '\0'){
			idx_before_null = i;
		}
	}
	
	for(size_t i=idx_before_space+2; i<=idx_before_null; i++){
		uint8_t digit = (uint8_t)(buffer[i] - '0');
		file_size = file_size*10 + digit;
	}
	
	uint8_t header_len = idx_before_space + 1 + (idx_before_null - idx_before_space) + 1;
	
	det->header_len = header_len;
	det->file_size = file_size;
	
	return det;
}

//sha1-hash value
unsigned char *sha_encoding(const unsigned char *buffer){
	
	struct blob_details *det = get_file_det(buffer);
	unsigned long buffer_size = det->header_len + det->file_size;
	
	unsigned char *sha_val = (unsigned char *)malloc(21 * sizeof(unsigned char));
	sha_val[20] = '\0';
	
	SHA1(buffer, buffer_size, sha_val);
	
	return sha_val;
}

//zlib-compression function
struct compressed_struct *zlib_compression(const char *buffer){
	
	struct blob_details *det = get_file_det(buffer);
	unsigned long buffer_size = det->header_len + det->file_size;
	
	uLongf compressed_size = compressBound(buffer_size);
	
	unsigned char *compressed = (unsigned char *)malloc(compressed_size);
	
	if(compress2(compressed, &compressed_size, buffer, buffer_size, Z_BEST_COMPRESSION) != Z_OK){
		fprintf(stderr, "compression failed\n");
		exit(EXIT_FAILURE);
	}
	
	struct compressed_struct *cpress = (struct compressed_struct *)malloc
																				(sizeof(struct compressed_struct));
	
	cpress->compressed_content = compressed;
	cpress->size = compressed_size;
	
	free(det);
	
	return cpress;
}

void write_compressed(struct compressed_struct *cpress, char *hash_val){
	unsigned char *compressed_content = cpress->compressed_content;
	uLongf size = cpress->size;
	
	char directory_path[DIR_PATH_SIZE_CONST + 3 + 1];
	snprintf(directory_path, DIR_PATH_SIZE_CONST+1, "%s", DIR_PATH);
	directory_path[DIR_PATH_SIZE_CONST] = hash_val[0];
	directory_path[DIR_PATH_SIZE_CONST+1] = hash_val[1];
	directory_path[DIR_PATH_SIZE_CONST+2] = '/';
	directory_path[DIR_PATH_SIZE_CONST+3] = '\0';
	
	int dir_created = mkdir(directory_path, 0777);
	if(dir_created != 0){
		if(errno != EEXIST){
			fprintf(stderr, "error in writing file\n");
			exit(EXIT_FAILURE);
		}
	}
	
	char file_path[DIR_PATH_SIZE_CONST + 3 + 38 + 1];
	strncpy(file_path, directory_path, DIR_PATH_SIZE_CONST+3);
	
	file_path[DIR_PATH_SIZE_CONST+2] = '/';
	for(size_t i=0; i<38; i++){
		file_path[DIR_PATH_SIZE_CONST + 3 + i] = hash_val[2 + i];
	}
	file_path[DIR_PATH_SIZE_CONST + 1 + 40] = '\0';
	
	FILE *fptr;
	if((fptr = fopen(file_path, "wb")) == NULL){
		fprintf(stderr, "error in opening file [%s]\n", file_path);
		exit(EXIT_FAILURE);
	}
	
	fwrite(compressed_content, sizeof(unsigned char), size, fptr);
	
	if(fclose(fptr) == EOF){
		fprintf(stderr, "error in closing file\n");
		exit(EXIT_FAILURE);
	}
}
