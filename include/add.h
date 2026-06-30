#ifndef ADD_H
#define ADD_H

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define INDEX_PATH "./.rec/index"
#define MAX_FILE_NAME_SIZE 1024

struct index_entry_det{
	uint32_t ctime;
	uint32_t ctime_nano;
	uint32_t mtime;
	uint32_t mtime_nano;
	uint32_t dev;
	uint32_t ino;
	uint32_t mode;
	uint32_t sixteen_bits_and_padding;
	uint32_t uid;
	uint32_t gid;
	uint32_t file_size;
	uint16_t flag;
};

struct entry{
	struct index_entry_det det;
	unsigned char sha_val[20];
	char filename[MAX_FILE_NAME_SIZE];
};

struct index_entry_det *in_big_endian(const char *filepath);
void add(const char *filepath);
struct entry *get_index_entry(FILE **fptr);
void write_to_index_file(struct entry *ent, FILE **fptr);

#endif
