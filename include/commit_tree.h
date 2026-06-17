#ifndef COMMIT_TREE_H
#define COMMIT_TREE_H

#include "../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAIN_PATH "./.git/refs/heads/main"
#define NAME_LEN 128
#define EMAIL_LEN 256

void commit_tree(char *tree_hash, char *message);

#endif
