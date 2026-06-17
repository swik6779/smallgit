#ifndef ADD_ALL_H
#define ADD_ALL_H

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#define FILEPATH_MAX_SIZE 1024

void add_all(const char *dirpath);

#endif
