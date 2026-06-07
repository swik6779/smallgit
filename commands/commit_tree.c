#include "../include/commit_tree.h"
#include "../include/init.h"
#include "../include/object.h"

/*
commit object structure: 

commit <size>\0<contents>

contents:

	tree <hash-val>
	parent <hash-val> (this line is only there if there was at least one commit previously)
	author <name> <email> <timestamp> <timezone-offset>
	committer <name> <email> <timestamp> <timezone-offset>

	commit-message
*/

void commit_tree(char *tree_hash, char *message){
	if(strlen(tree_hash) != 40){
		fprintf(stderr, "tree_hash must be 40 chars\n");
		exit(EXIT_FAILURE);
	}
	
	char *config_contents = get_config_file();
	// getting name
	size_t content_len = 4 + 1 + 40 + 1; //tree <hash-val>\n
	content_len += (strlen(message));
	
	char last_hash_val[41] = {0};
	FILE *fptr;	
	if(0 == access(MAIN_PATH, F_OK)){ 
		//in this case, this commit is not the first one, thus, parent field is non-empty
		//parent <hash-val>\n
		content_len += (6 + 1 + 20 + 1);
		
		if((fptr = fopen(MAIN_PATH, "r")) == NULL){
			fprintf(stderr, "cannot open file %s\n", MAIN_PATH);
			exit(EXIT_FAILURE);
		}
		
		fseek(fptr, -41, SEEK_END);
		ssize_t len = read(fptr, last_hash_val, 41);
		
		if(fclose(fptr)){
			fprintf(stderr, "cannot close file\n");
			exit(EXIT_FAILURE);
		}
	}
	
	char name[NAME_LEN];
	char email[EMAIL_LEN];
	size_t email_start;
	size_t i = 7;
	while(1){
		if('\n' == config_contents[i]){
			email_start = i + 1;
			name[i] = '\0';
			break;
		}
		else{
			name[i - 7] = config_contents[i];
		}
		i++;
	}
	
	i+=1;
	
	while(1){
		if('\n' == config_contents[i]){
			email[i] = '\0';
			break;
		}
		else{
			email[i - email_start] = config_contents[i];
			fprintf(stdout, "%c", email[i - email_start]);
		}
		i++;
	}
	fprintf(stdout, "name: %s, email: %s\n", name, email);
	
	time_t commit_time;
	commit_time = time(NULL);
	
	//content_len has to be increased by the below amount for displaying author details
	//format of author details: author <name> <email> <timestamp> +0000\n
	//(+0000) refers to the timezone offset, it is being hardcoded for this implementation 
	//but this may change for future versions
	content_len += (6 + 1 + strlen(name) + 1 + 1 + strlen(email) + 1 + 1 + 10 + 1 + 5 + 1);
	
	//content_len has to be increased by the below amount for displaying committer details
	//format of author details: committer <name> <email> <timestamp> +0000\n
	//for now both author and committer will be the same, may change for future versions
	content_len += (9 + 1 + strlen(name) + 1 + 1 + strlen(email) + 1 + 1 + 10 + 1 + 5 + 1);
	
	content_len += (1 + strlen(message));
	
	char size_in_str[20];
	snprintf(size_in_str, 20, "%lu", content_len);
	
	size_t header_len = 6 + 1 + strlen(size_in_str) + 1;
	char *header = (char *)malloc(header_len * sizeof(char));
	snprintf(header, header_len, "commit %s", size_in_str);
	char *commit_content = (char *)malloc((content_len + 1) * sizeof(char));
	
	if(last_hash_val[0] != '\0'){
		snprintf(commit_content, content_len + 1, "tree %s\nparent %s\nauthor %s <%s> %lu +0000\ncommitter %s <%s> %lu +0000\n\n%s", tree_hash, last_hash_val, name, email, commit_time, name, email, commit_time, message);
	}
	else{
		snprintf(commit_content, content_len + 1, "tree %s\nauthor %s <%s> %lu +0000\ncommitter %s <%s> %lu +0000\n\n%s", tree_hash, name, email, commit_time, name, email, commit_time, message);
	}
	
	char *commit_object = (char *)malloc((content_len + header_len) * sizeof(char));
	memcpy(commit_object, header, header_len);
	memcpy(commit_object + header_len, commit_content, content_len + 1);
	
	unsigned char *sha_val = (unsigned char *)malloc(21 * sizeof(char));
	SHA1(commit_object, header_len + content_len, sha_val);
	
	char *hash_val = byte_to_hex(sha_val);
	
	uLongf compressed_size = compressBound(header_len + content_len);
	
	unsigned char *compressed = (unsigned char *)malloc(compressed_size);
	
	if(compress2(compressed, &compressed_size, commit_object, header_len + content_len, Z_BEST_COMPRESSION) != Z_OK){
		fprintf(stderr, "compression failed\n");
		exit(EXIT_FAILURE);
	}
	
	struct compressed_struct *cpress = (struct compressed_struct *)malloc
																				(sizeof(struct compressed_struct));
	
	cpress->compressed_content = compressed;
	cpress->size = compressed_size;
	write_compressed(cpress, hash_val);
	
	if((fptr = fopen(MAIN_PATH, "w")) == NULL){
		fprintf(stderr, "cannot open file %s\n", MAIN_PATH);
		exit(EXIT_FAILURE);
	}
	
	char hash_val_newline[42];
	snprintf(hash_val_newline, 42, "%s\n", hash_val);
	
	fwrite(hash_val, sizeof(char), 41, fptr);
	fprintf(stdout, "commit-object: %s stored\n", hash_val);
	if(fclose(fptr)){
		fprintf(stderr, "cannot close file\n");
		exit(EXIT_FAILURE);
	}
	
	free(header);
	free(commit_content);
	free(sha_val);
	free(hash_val);
	free(cpress);
}
