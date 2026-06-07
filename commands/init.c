#include "../include/init.h"

void init_repository(){
	char *repo = REPO_PATH; int repo_check;
	char *objects_folder = OBJECTS_PATH; int objects_check;
	char *refs_folder = REFS_PATH; int refs_check;
	char *heads_folder = REFS_HEADS_PATH; int heads_check;
	
	char *HEAD_FILE = HEADS_FILE_PATH;
	
	repo_check = mkdir(repo, 0777);
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
	
	objects_check = mkdir(objects_folder, 0777);
	if(objects_check == -1){
		fprintf(stderr, "error in initializing repo (object)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	refs_check = mkdir(refs_folder, 0777);
	if(refs_check == -1){
		fprintf(stderr, "error in initializing repo (refs)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	heads_check = mkdir(heads_folder, 0777);
	if(heads_check == -1){
		fprintf(stderr, "error in initializing (heads)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	FILE *fptr;
	
	fptr = fopen(HEAD_FILE, "w");
	if(fptr == NULL){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	if(fprintf(fptr, "ref: refs/heads/main\n") < 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	if(fclose(fptr) != 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	FILE *fptr2 = fopen(CONFIG_FILE_PATH, "w");
	if(fptr2 == NULL){
		fprintf(stderr, "error in initializing repo (config)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	if(fprintf(fptr2, "name = dummy1\nemail = dummy1@mail.com\n") < 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	if(fclose(fptr2) != 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(repo);
		exit(EXIT_FAILURE);
	}
	
	fprintf(stdout, "empty repo initialized\n");
}
