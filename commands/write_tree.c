#include "../include/write_tree.h"
#include "../include/hash_object.h"
#include "../include/object.h"

/*
called on directories to create tree objects. when it is called, hash-object is called on all the files in it.
it is recursive in nature as tree objects are made of the inner directories as well.
*/

char *write_tree(const char *path){
	struct dirent *de;
	
	DIR *dr = opendir(path);
	
	if(dr == NULL){
		fprintf(stderr, "directory couldn't be accessed\n");
		exit(EXIT_FAILURE);
	}
	
	size_t entry_len = 0;
	
	//this while loop calculates the length for each entry in the tree object file
	while((de = readdir(dr)) != NULL){
		if(0 == strcmp(de->d_name, ".git") || 0 == strcmp(de->d_name, ".") || 0 == strcmp(de->d_name, "..")){
			continue;
		}
		
		// 6 for filemode, space, strlen(de->d_name), space, 20-bytes, null-char
		entry_len += (6 + 1 + strlen(de->d_name) + 1 + 20);
	}
	
	char size_in_str[SIZE_BITS];
	snprintf(size_in_str, SIZE_BITS, "%zu", entry_len);
	
	int bits_for_digits = 0;
	for(int i=0; i<SIZE_BITS; i++){
		if(size_in_str[i] >= '0' && size_in_str[i] <= '9'){
			bits_for_digits++;
		}
		else{
			break;
		}
	}
		
	// 5 - "tree ", bits-for-digits, 1 for null, entries, 1 for null
	unsigned char *buffer = (unsigned char *)malloc((5 + bits_for_digits + 1 + entry_len) *
																																					 sizeof(unsigned char));
	
	//tree header
	buffer[0]	= 't'; buffer[1] = 'r'; buffer[2] = 'e'; buffer[3] = 'e'; buffer[4]	= ' ';
	
	for(int i=0; i<bits_for_digits; i++){
		buffer[5 + i] = size_in_str[i];
	}
	buffer[5 + bits_for_digits] = '\0';
	
	size_t iter = 5 + bits_for_digits + 1;
	
	rewinddir(dr);
	
	while((de = readdir(dr)) != NULL){
	
		// to avoid recursion on .git, . , and .. directories
		if(0 == strcmp(de->d_name, ".git") || 0 == strcmp(de->d_name, ".") || 0 == strcmp(de->d_name, "..")){
			continue;
		}
		
		struct stat st;
		char *sha_val;
		char iter_path[ENTRY_SIZE + 1];
		snprintf(iter_path, ENTRY_SIZE+1, "%s%s", path, de->d_name);
		
		if(stat(iter_path, &st)){
			fprintf(stderr, "stat-fn failed\n");
			exit(EXIT_FAILURE);
		}
		
		//procedure with a regular file
		if(S_ISREG(st.st_mode)){
		
			buffer[iter++] = '1'; buffer[iter++] = '0'; buffer[iter++] = '0'; buffer[iter++] = '6';
			buffer[iter++] = '4'; buffer[iter++] = '4'; buffer[iter++] = ' ';
			
			sha_val = hash_object(iter_path);
		}
		
		//procedure with a directory
		else if(S_ISDIR(st.st_mode)){
			char *inner_dir_path = (char *)malloc((ENTRY_SIZE + 1) * sizeof(char));
			snprintf(inner_dir_path, ENTRY_SIZE+1, "%s/", iter_path);
			sha_val = write_tree(inner_dir_path);	
			
			buffer[iter++] = '0'; buffer[iter++] = '4'; buffer[iter++] = '0'; buffer[iter++] = '0';
			buffer[iter++] = '0'; buffer[iter++] = '0'; buffer[iter++] = ' ';
			free(inner_dir_path);
		}
		
		else{
			continue;
		}
		
		for(int i=0; i < strlen(de->d_name); i++){
			buffer[iter++] = de->d_name[i];
		}
		buffer[iter++] = '\0';
		
		for(int i=0; i < 20; i++){
			buffer[iter++] = sha_val[i];
		}
		
		free(sha_val);
	}
	
	buffer[iter] = '\0';
	
	struct compressed_struct *cpress = zlib_compression(buffer);
	unsigned char *sha_val = sha_encoding(buffer);
	char *hash_val = byte_to_hex(sha_val);
	write_compressed(cpress, hash_val);
	fprintf(stdout, "tree-object stored: %s\n", hash_val);
	
	free(hash_val);
	free(cpress);
	
	return sha_val;
}

