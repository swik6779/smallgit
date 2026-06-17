#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define REPO_PATH "./.git"
#define OBJECTS_PATH "./.git/objects"
#define REFS_PATH "./.git/refs"
#define REFS_HEADS_PATH "./.git/refs/heads"
#define HEADS_FILE_PATH "./.git/HEAD"
#define CONFIG_FILE_PATH "./.git/config"

void init_repository();

#endif
