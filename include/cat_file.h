#ifndef CAT_FILE_H
#define CAT_FILE_H

#include "../include/object.h"
#include "../include/utils.h"

unsigned char *parse_tree_entry(unsigned char *str, size_t entry_point, size_t end_idx);
unsigned char *slice(unsigned char *str, size_t pos1, size_t pos2);
void cat_file(char *hash_val);

#endif
