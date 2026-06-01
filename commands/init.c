#include "../include/init.h"

void init_repository(){
	char *spoon_repo = "./.spoon"; int spoon_repo_check;
	char *objects_folder = "./.spoon/objects"; int objects_check;
	char *refs_folder = "./.spoon/refs"; int refs_check;
	char *heads_folder = "./.spoon/refs/heads"; int heads_check;
	
	char *HEAD_FILE = "./.spoon/HEAD";
	
	spoon_repo_check = mkdir(spoon_repo, 0777);
	if(spoon_repo_check == -1){
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
		rmdir(spoon_repo);
		exit(EXIT_FAILURE);
	}
	
	refs_check = mkdir(refs_folder, 0777);
	if(refs_check == -1){
		fprintf(stderr, "error in initializing repo (refs)\n");
		rmdir(spoon_repo);
		exit(EXIT_FAILURE);
	}
	
	heads_check = mkdir(heads_folder, 0777);
	if(heads_check == -1){
		fprintf(stderr, "error in initializing (heads)\n");
		rmdir(spoon_repo);
		exit(EXIT_FAILURE);
	}
	
	FILE *fptr;
	
	fptr = fopen(HEAD_FILE, "w");
	if(fptr == NULL){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(spoon_repo);
		exit(EXIT_FAILURE);
	}
	
	if(fprintf(fptr, "ref: refs/heads/main\n") < 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(spoon_repo);
		exit(EXIT_FAILURE);
	}
	
	if(fclose(fptr) != 0){
		fprintf(stderr, "error in initializing repo (HEAD)\n");
		rmdir(spoon_repo);
		exit(EXIT_FAILURE);
	}
}
