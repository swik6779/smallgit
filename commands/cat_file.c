#include "../include/cat_file.h"
#include "../include/object.h"
#include "../include/write_tree.h"
#include "../include/utils.h"

//cat-file allows for the view of a particular file in a human-readable format
void cat_file(char *hash_val){
	size_t path_len = DIR_PATH_SIZE_CONST + 2 + 1 + 38 + 1;
	char path[path_len];
	snprintf(path, DIR_PATH_SIZE_CONST + 1, "%s", DIR_PATH);
	path[DIR_PATH_SIZE_CONST] = hash_val[0];
	path[DIR_PATH_SIZE_CONST + 1] = hash_val[1];
	path[DIR_PATH_SIZE_CONST + 2] = '/';
	
	for(int i=0;i<38;i++){
		path[DIR_PATH_SIZE_CONST + 3 +i] = hash_val[2 + i];
	}
	path[DIR_PATH_SIZE_CONST + 2 + 1 + 38] = '\0';
	
	size_t file_size;
	struct stat st;
	if(stat(path, &st) == 0){
		file_size = st.st_size;
	}
	else{
		fprintf(stderr, "extract file-size failed, or file-not-exist\n");
		exit(EXIT_FAILURE);
	}
	
	unsigned char *buffer = (unsigned char *)malloc((file_size + 1)*sizeof(unsigned char));
	
	FILE *fptr;
	if((fptr = fopen(path, "rb")) == NULL){
		fprintf(stderr, "file cannot be open-ed\n");
		exit(EXIT_FAILURE);
	}
	
	if(fread(buffer, sizeof(unsigned char), file_size, fptr) != file_size){
		fprintf(stderr, "file-read gone wrong\n");
		exit(EXIT_FAILURE);
	}
	buffer[file_size] = '\0';
	
	uLongf decompressed_size = file_size * 10;
	unsigned char *decompressed = (unsigned char *)malloc(decompressed_size);
	
	uncompress(decompressed, &decompressed_size, buffer, file_size);
	
	unsigned char *s = slice(decompressed, 0, 4);
	
	size_t index_of_null = (strcmp(s, "comm") != 0)? 5:7;
	size_t actual_size = 0;
	while(decompressed[index_of_null] != '\0'){
		actual_size = actual_size*10 + (decompressed[index_of_null] - '0');
		index_of_null++;
	}
	
	
	if(0 == strcmp(s, "blob") || 0 == strcmp(s, "comm")){
		for(size_t i = 0;i < actual_size; i++){
			fprintf(stdout, "%c", decompressed[index_of_null + 1 + i]);
		}
	}
	else if(0 == strcmp(s, "tree")){
		size_t entry_len = 0;
		size_t entry_point = index_of_null + 1;
		size_t end_idx = entry_point + 1;
		
		for(size_t i = index_of_null + 1; i < actual_size + index_of_null + 1; i++){
			if('\0' == decompressed[i]){
				end_idx = i + 20 + 1;
				unsigned char *parsed_entry = parse_tree_entry(decompressed, entry_point, end_idx);
				fprintf(stdout, "%s\n", parsed_entry);
				entry_point = end_idx;
				free(parsed_entry);
				i+=20;
			}
		}
	}
	else{
		fprintf(stderr, "file not-suitable\n");
		exit(EXIT_FAILURE);
	}
	
	free(s);
	free(buffer);
	free(decompressed);
}
