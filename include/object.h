#ifndef OBJECT_H
#define OBJECT_H

#include <dirent.h>
#include <errno.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zlib.h>

#define SIZE_BITS 20
#define DIR_PATH_SIZE_CONST 17

struct blob_details{
	uint8_t header_len;
	unsigned long file_size;
};

struct compressed_struct{
	unsigned char *compressed_content;
	uLongf size;
};

unsigned char *getcontnts(const char *filepath);
struct blob_details *get_file_det(const char *buffer);
unsigned char *sha_encoding(const unsigned char *buffer);
struct compressed_struct *zlib_compression(const unsigned char *buffer);
void write_compressed(struct compressed_struct *cpress, char *hash_val);

#endif
