#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>

struct commit_object_det{
	char tree_hash[40];
	char parent[40];
	char committer[512];
	char author[512];
	char commit_msg[1024];
};

struct commit_object_det parse_commit_object(const char *tree_hash);
char *get_branch();

#endif
