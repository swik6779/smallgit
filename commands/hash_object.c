#include "../include/hash_object.h"

/*
flow of hash-object: 
get contents of a file into a buffer -> get its SHA hash value -> convert into hexadecimal value -> compress the contents of the file into a blob object -> write compressed contents to a file
*/

char *hash_object(const char *filepath){
	unsigned char *buffer = getcontnts(filepath);
	unsigned char *sha_val = sha_encoding(buffer);
	
	char *hash_val = byte_to_hex(sha_val);
	struct compressed_struct *cpress = zlib_compression(buffer);
	
	write_compressed(cpress, hash_val);
	
	free(buffer);
	free(cpress);
	fprintf(stdout, "blob-object stored: %s\n", hash_val);
	free(hash_val);
	
	return sha_val;
}
