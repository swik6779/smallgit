#include "./include/init.h"
#include "./include/hash_object.h"

int main(int argc, char **argv){
	if(strcmp(argv[1], "init") == 0){
		init_repository();
	}
	else if(strcmp(argv[1], "hash-object") == 0){
		if(argc != 3){
			fprintf(stderr, "incorrect-usage\n");
			exit(EXIT_FAILURE);
		}
		else{
			hash_object(argv[2]);
		}
	}
	else{
		fprintf(stderr, "function-not-defined\n");
		exit(EXIT_FAILURE);
	}
}
