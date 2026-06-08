#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../include/init.h"

char *byte_to_hex(unsigned char *sha_val);
char *get_config_file();
unsigned char *parse_tree_entry(unsigned char *str, size_t entry_point, size_t end_idx);
unsigned char *slice(unsigned char *str, size_t pos1, size_t pos2);

#endif
