#include "../include/utils.h"
#include "../include/write_tree.h"

char *byte_to_hex(unsigned char *sha_val){

	char *hash_val = (char *)malloc(41 * sizeof(char));
	for(int i=0;i<20;i++){
		sprintf(hash_val + (2*i), "%02x", sha_val[i]);
	}
	
	return hash_val;
}

//this function allows for slicing of strings between pos1 and pos2 of str
//(pos2 char is not included)
unsigned char *slice(unsigned char *str, size_t pos1, size_t pos2){	
	
	if(pos2 <= pos1){
    fprintf(stderr, "invalid slice\n");
    exit(EXIT_FAILURE);
	}
	
	unsigned char *s = (unsigned char *)malloc((pos2- pos1 + 1) * sizeof(unsigned char));
	for(size_t i = 0; i < pos2-pos1; i++){
		s[i] = str[pos1 + i];
	}
	
	s[pos2 - pos1] = '\0';
	return s;
}

/*
parse_tree_entry will parse a tree entry from a tree file
tree-entry in tree-object file: mode filename\0(20-bit-hash-val)
parsed output: <mode-code> mode hash-val filename
*/

unsigned char *parse_tree_entry(unsigned char *str, size_t entry_point, size_t end_idx){
	
	unsigned char *parsed_entry = (unsigned char *)malloc((2*ENTRY_SIZE + 1) * sizeof(unsigned char));
	unsigned char *mode = slice(str, entry_point, entry_point + 6);
	unsigned char *filename = slice(str, entry_point + 7, end_idx - 20);
	unsigned char *sha_val = slice(str, end_idx - 20, end_idx);
	char *hash_val = byte_to_hex(sha_val);
	
	if(0 == strcmp(mode, "100644")){
		snprintf(parsed_entry, 2*ENTRY_SIZE + 1, "100664 blob %s %s", hash_val, filename);
	}
	else if (0 == strcmp(mode, "040000")){
		snprintf(parsed_entry, 2*ENTRY_SIZE + 1, "040000 tree %s %s", hash_val, filename);
	}
	else{ //for now return error
		fprintf(stderr, "%s is a invalid-file-format\n", mode);
		exit(EXIT_FAILURE);
	}
	
	free(mode);
	free(filename);
	free(sha_val);
	free(hash_val);
	
	return parsed_entry;
}

char *get_config_file(){
	FILE *fptr = fopen(CONFIG_FILE_PATH, "r");
	struct stat st;
	
	if(stat(CONFIG_FILE_PATH, &st)){
		fprintf(stderr, "config file couldn't be opened\n");
		exit(EXIT_FAILURE);
	}
	
	char *contents = (char *)malloc(st.st_size + 1 * sizeof(char));
	if(fread(contents, sizeof(char), st.st_size, fptr) == 0){
		fprintf(stderr, "config file couldn't be parsed\n");
		exit(EXIT_FAILURE);
	}
	contents[st.st_size] = '\0';
	
	if(fclose(fptr) != 0){
		fprintf(stderr, "config file couldn't be parsed\n");
		exit(EXIT_FAILURE);
	}
	
	return contents;
}
