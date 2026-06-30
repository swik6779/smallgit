#include "../include/add.h"
#include "../include/add_all.h"

void add_all(const char *dirpath){
	DIR *dir;
	if((dir = opendir(dirpath)) == NULL){
		fprintf(stderr, "cannot open dir %s\n", dirpath);
		exit(EXIT_FAILURE);
	}
	
	struct dirent *de;
	while((de = readdir(dir)) != NULL){
		
		if(0 == strcmp(de->d_name, ".") || 0 == strcmp(de->d_name, "..") || 0 == strcmp(de->d_name, ".rec")){
			continue;
		}
		
		char path[FILEPATH_MAX_SIZE];

		if(strcmp(dirpath, "./") == 0){
    	snprintf(path, sizeof(path), "%s", de->d_name);
    }
		else{
	    snprintf(path, sizeof(path), "%s/%s", dirpath, de->d_name);
	   }
	
		struct stat st;
		
		if(stat(path, &st)){
			fprintf(stderr, "cannot access %s\n", de->d_name);
			exit(EXIT_FAILURE);
		}
		
		if(S_ISREG(st.st_mode)){
			add(path);
		}
		else if(S_ISDIR(st.st_mode)){
			add_all(path);
		}
	}
	
	if(closedir(dir)){
		fprintf(stderr, "cannot close directory stream\n");
		exit(EXIT_FAILURE);
	}
}
