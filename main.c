#include "./include/init.h"
#include "./include/hash_object.h"
#include "./include/cat_file.h"
#include "./include/write_tree.h"
#include "./include/commit_tree.h"
#include "./include/add.h"
#include "./include/add_all.h"
#include "./include/commit.h"

int main(int argc, char **argv){
	if(0 == strcmp(argv[1], "init")){
		if(argc != 2){
			fprintf(stderr, "too many arguments for init\n");
			exit(EXIT_FAILURE);
		}
		init_repository();
		
	}
	else if(0 == strcmp(argv[1], "hash-object")){
		if(argc != 3){
			fprintf(stderr, "incorrect-usage of hash-object\n");
			exit(EXIT_FAILURE);
		}
		
		char *hash_val = hash_object(argv[2]);		
		free(hash_val);
	}
	else if(0 == strcmp(argv[1], "cat-file")){
		if(argc != 3){
			fprintf(stderr, "incorrect-usage of cat-file\n");
			exit(EXIT_FAILURE);
		}
		cat_file(argv[2]);
	}
	else if(0 == strcmp(argv[1], "rmv")){
		int rmv_status = system("rm -rf .rec");
		if(rmv_status != 0){
			fprintf(stderr, "rmv failed\n");
			exit(EXIT_FAILURE);
		}
	}
	else if(0 == strcmp(argv[1], "write-tree")){
		char *hash_val;
		if(argc == 2){
			hash_val = write_tree("./");
		}
		else if(argc == 3){
			char path[strlen(argv[2]) + 3 + 1];
			snprintf(path, strlen(argv[2]) + 4, "./%s/", argv[2]);
			struct stat st;
			
			if(stat(path, &st) != 0){
				fprintf(stderr, "cannot access %s\n", path);
				exit(EXIT_FAILURE);
			}
			
			if(!S_ISDIR(st.st_mode)){
				fprintf(stderr, "%s is not a dir-path\n", path);
				exit(EXIT_FAILURE);
			}
			
			hash_val = write_tree(path);
		}
		else{
			fprintf(stderr, "incorrect-usage of write-tree\n");
			exit(EXIT_FAILURE);
		}
		
		free(hash_val);
	}
	else if(0 == strcmp(argv[1], "commit-tree")){
		if(argc != 4){
			fprintf(stderr, "incorrect-usage of commit-tree\n");
			exit(EXIT_FAILURE);
		}
		
		char *hash_val = argv[2];
		char *message = argv[3];
		
		commit_tree(hash_val, message);
	}
	else if(0 == strcmp(argv[1], "add")){
		if(argc != 3){
			fprintf(stderr, "incorrect-usage of add\n");
			exit(EXIT_FAILURE);
		}
		
		if(0 == strcmp(argv[2], "all")){
			add_all("./");
		}
		else{
			add(argv[2]);
		}
	}
	else if(0 == strcmp(argv[1], "commit")){
		if(argc != 3){
			fprintf(stderr, "incorrect-usage of add\n");
			exit(EXIT_FAILURE);
		}
		
		commit(argv[2]);
	}
	else{
		fprintf(stderr, "no-such-command\n");
		exit(EXIT_FAILURE);
	}
}




