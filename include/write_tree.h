#ifndef WRITE_TREE_H
#define WRITE_TREE_H

#include <dirent.h>
#include <sys/stat.h>

#define ENTRY_SIZE 1024

char *write_tree(const char *path);

#endif
