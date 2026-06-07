#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define REPO_PATH "./.rec"
#define OBJECTS_PATH "./.rec/objects"
#define REFS_PATH "./.rec/refs"
#define REFS_HEADS_PATH "./.rec/refs/heads"
#define HEADS_FILE_PATH "./.rec/HEAD"
#define CONFIG_FILE_PATH "./.rec/config"

void init_repository();

#endif
