#include "../include/init.h"
#include "../include/log.h"

char *get_branch(){
	FILE *HEAD_fptr;
	if((HEAD_fptr = fopen(HEADS_FILE_PATH, "r")) == NULL){
		fprintf(stderr, "%s couldn't be accessed\n", HEADS_FILE_PATH);
		exit(EXIT_FAILURE);
	}
	
	if(fseek(HEAD_fptr, 0, SEEK_END) != 0){
		fprintf(stderr, "SEEK_END couldn't be reached\n");
		exit(EXIT_FAILURE);
	}
	
	long num;
	if((num = ftell(HEAD_fptr)) == 1L){
		fprintf(stderr, "ftell failed\n");
		exit(EXIT_FAILURE);
	}
	
	if(fseek(HEAD_fptr, 6, SEEK_SET) != 0){
		fprintf(stderr, "fseek failed\n");
		exit(EXIT_FAILURE);
	}
	
	char *branch = malloc((num - 5 + 1) * sizeof(char));
	
	fread(branch, 1, num-5+1, HEAD_fptr);
	branch[num - 3] = '\0';
	
	return branch;
}

struct commit_object_det parse_commit_object(const char *tree_hash){
	
}

void log(){
	char *branch = get_branch();
	char path[5 + strlen(branch) + 1];
	snprintf(path, 5 + strlen(branch) + 1, ".rec/%s", branch);
	
	FILE *fptr;
	if((fptr = fopen(path, "r")) == NULL){
		fprintf(stderr, "%s couldn't be accessed\n", path);
		exit(EXIT_FAILURE);
	}
	
	char latest_hash[40 + 1];
	fread(latest_hash, 40, 1, fptr);
	latest_hash[40] = '\0';
	
	struct commit_object_det det = parse_commit_object(latest_hash);
	
}




