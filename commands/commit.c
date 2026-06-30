#include "../include/utils.h"
#include "../include/write_tree.h"
#include "../include/commit_tree.h"
#include "../include/commit.h"

void commit(const char *message){
	unsigned char *tree_sha_val = write_tree("./");
	char *tree_hash = byte_to_hex(tree_sha_val);
	commit_tree(tree_hash, message);
	
	free(tree_hash);
}
