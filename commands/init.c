#include "../include/init.h"

/*
	.rec
		\__ config
		\__ HEAD
		\__ objects
		\__ refs
					\__ heads
								\__ main (created with the first commit)
*/

void init_repository(){

	int repo_check = mkdir(REPO_PATH, 0777);
	if(repo_check == -1){
		if(errno == EEXIST){
			fprintf(stderr, "repo already initialized\n");
			exit(EXIT_FAILURE);
		}
		else{
			fprintf(stderr, "failure in repo init\n");
			exit(EXIT_FAILURE);
		}
	}
	
	int objects_check = mkdir(OBJECTS_PATH, 0777);
	if(objects_check == -1){
		fprintf(stderr, "error in initializing repo (object)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	int refs_check = mkdir(REFS_PATH, 0777);
	if(refs_check == -1){
		fprintf(stderr, "error in initializing repo (refs)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	int heads_check = mkdir(REFS_HEADS_PATH, 0777);
	if(heads_check == -1){
		fprintf(stderr, "error in initializing (heads)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	FILE *fptr;
	
	fptr = fopen(HEADS_FILE_PATH, "w");
	if(fptr == NULL){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	if(fprintf(fptr, "ref: refs/heads/main\n") < 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	if(fclose(fptr) != 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	FILE *fptr2 = fopen(CONFIG_FILE_PATH, "w");
	if(fptr2 == NULL){
		fprintf(stderr, "error in initializing repo (config)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	if(fprintf(fptr2, "name = dummy1\nemail = dummy1@mail.com\n") < 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	if(fclose(fptr2) != 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(REPO_PATH);
		exit(EXIT_FAILURE);
	}
	
	fprintf(stdout, "empty repo initialized\n");
}
