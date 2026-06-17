#include "../include/add.h"
#include "../include/utils.h"
#include "../include/object.h"
#include "../include/hash_object.h"

/*
INDEX FILE:
index file header: DIRC<version><num_entries> (12 byte header)
<index entry>
.
.
.
---------------------------------------------------------------
index entry structure:
<ctime> <ctime_nanosec> <mtime> <mtime_nanosec> <dev> <ino> <mode> (all 32 bits)
<object-type (4 bits)> <unused (3 bits)> <unix permisiion (9 bits)> 
<uid> <gid> <file_size> <object-name>
--flags
<assume-valid-bit> <extended-flag> <stage (2 bit)> <name-length (12 bit)>
*/

struct index_entry_det *in_big_endian(const char *filepath){

	struct stat st;
	if(stat(filepath, &st)){
		fprintf(stderr, "stat-failed\n");
		exit(EXIT_FAILURE);
	}

	struct index_entry_det *entry = malloc(sizeof(struct index_entry_det));
	entry->ctime = htonl(st.st_ctime);
	entry->ctime_nano = htonl(st.st_ctim.tv_nsec);
	entry->mtime = htonl(st.st_mtime);
	entry->mtime_nano = htonl(st.st_mtim.tv_nsec);
	entry->dev = htonl(st.st_dev);
	entry->ino = htonl(st.st_ino);
	entry->mode = htonl(st.st_mode);
	entry->sixteen_bits_and_padding = htonl(0x000081A4);
	entry->uid = htonl(st.st_uid);
	entry->gid = htonl(st.st_gid);
	entry->file_size = htonl(st.st_size);
	entry->flag = htons(strlen(filepath) & 0xFFF);
	
	return entry;
}

struct entry *get_index_entry(FILE **fptr){

	struct entry *ent = malloc(sizeof(struct entry));
	fread(&(ent->det.ctime), 4, 1, *fptr);
	fread(&(ent->det.ctime_nano), 4, 1, *fptr);
	fread(&(ent->det.mtime), 4, 1, *fptr);
	fread(&(ent->det.mtime_nano), 4, 1, *fptr);
	fread(&(ent->det.dev), 4, 1, *fptr);
	fread(&(ent->det.ino), 4, 1, *fptr);
	fread(&(ent->det.mode), 4, 1, *fptr);
	fread(&(ent->det.sixteen_bits_and_padding), 4, 1, *fptr);
	fread(&(ent->det.uid), 4, 1, *fptr);
	fread(&(ent->det.gid), 4, 1, *fptr);
	fread(&(ent->det.file_size), 4, 1, *fptr);
	fread(ent->sha_val, 1, 20, *fptr);
	fread(&(ent->det.flag), 2, 1, *fptr);
	
	int ch;
	int num_chars = 0;
	
	do{
		ch = fgetc(*fptr);
		ent->filename[num_chars++] = (char)ch;
	}
	while(ch != 0);
	size_t entry_size = 66 + strlen(ent->filename) + 1;
	size_t padding = (8 - (entry_size % 8)) % 8;
	fseek(*fptr, padding, SEEK_CUR);
	
	return ent;
}

void write_to_index_file(struct entry *ent, FILE **fptr){
	
	fwrite(&(ent->det.ctime), 1, 4, *fptr);
	fwrite(&(ent->det.ctime_nano), 1, 4, *fptr);
	fwrite(&(ent->det.mtime), 1, 4, *fptr);
	fwrite(&(ent->det.mtime_nano), 1, 4, *fptr);
	fwrite(&(ent->det.dev), 1, 4, *fptr);
	fwrite(&(ent->det.ino), 1, 4, *fptr);
	fwrite(&(ent->det.mode), 1, 4, *fptr);
	fwrite(&(ent->det.sixteen_bits_and_padding), 1, 4, *fptr);
	fwrite(&(ent->det.uid), 1, 4, *fptr);
	fwrite(&(ent->det.gid), 1, 4, *fptr);
	fwrite(&(ent->det.file_size), 1, 4, *fptr);
	fwrite(ent->sha_val, 1, 20, *fptr);
	fwrite(&(ent->det.flag), 1, 2, *fptr);
	fwrite(ent->filename, 1, strlen(ent->filename), *fptr);
	fputc(0, *fptr);
	
	//padding
	for(int i=0;i<(8 - ((67 + strlen(ent->filename)) % 8)) % 8;i++){
		fputc(0, *fptr);
	}
	
}

void add(const char *filepath){

	FILE *fptr;
	
	if(0 != access(INDEX_PATH, F_OK)){
		fptr = fopen(INDEX_PATH, "wb+");
		if(fptr == NULL){
			fprintf(stderr, "%s couldn't be opened son\n", INDEX_PATH);
			exit(EXIT_FAILURE);
		}
		
		uint32_t version = htonl(2);
		uint32_t num_entries = htonl(0);
		
		// index header
		fwrite("DIRC", 4, 1, fptr);
		fwrite(&version, 4, 1, fptr);
		fwrite(&num_entries, 4, 1, fptr);
	}
	else{
		fptr = fopen(INDEX_PATH, "rb+");
		if(fptr == NULL){
			fprintf(stderr, "%s couldn't be opened\n", INDEX_PATH);
			exit(EXIT_FAILURE);
		}
	}
	
	char filename[MAX_FILE_NAME_SIZE];
	snprintf(filename, MAX_FILE_NAME_SIZE, "%s", filepath);	
	unsigned char *sha_val_file_ptr = hash_object(filepath);
	unsigned char sha_val_file[20];
	memcpy(sha_val_file, sha_val_file_ptr, 20);
	struct index_entry_det *entry_to_push = in_big_endian(filepath);
	
	struct entry combined;
	combined.det = *entry_to_push;
	memcpy(combined.sha_val, sha_val_file, 20);
	strncpy(combined.filename, filename, strlen(filename) + 1);

	if(-1 == fseek(fptr, 8, SEEK_SET)){ 
		//points to the 9th byte, i.e, it points to the first byte of the entry field in header
		fprintf(stderr, "seek operation failed\n");
		exit(EXIT_FAILURE);	
	}
	
	uint32_t curr_num_entries;
	fread(&curr_num_entries, 4, 1, fptr);
	curr_num_entries = ntohl(curr_num_entries);
	struct entry *idx_entries[curr_num_entries + 1];
	
	//now to read every entry in the index-file
	if(-1 == fseek(fptr, 12, SEEK_SET)){ //pointing to end of index-file header
		fprintf(stderr, "seek operation failed\n");
		exit(EXIT_FAILURE);	
	}
	
	uint32_t completed = 0;
	bool in_idx = false;
	
	if(curr_num_entries == 0){
		idx_entries[completed++] = &combined;
		in_idx = true;
	}
	else{
		for(size_t i=0; i < curr_num_entries; i++){
			idx_entries[completed++] = get_index_entry(&fptr);
			if(strcmp(idx_entries[completed-1]->filename, filename) == 0){
				in_idx = true;
				idx_entries[completed-1] = &combined;
			}
		}
		
		if(!in_idx){
			for(size_t i=0; i < curr_num_entries+1; i++){
				if(i == curr_num_entries){
					idx_entries[i] = &combined;
					completed = curr_num_entries + 1;
					break;
				}
				if(strcmp(idx_entries[i]->filename, filename) > 0){
					for(size_t j = curr_num_entries; j > i; j--){
    				idx_entries[j] = idx_entries[j - 1];
					}
					
					idx_entries[i] = &combined;
					completed = curr_num_entries + 1;
					break;
				}
			}
		}
	}	
	
	completed = htonl(completed);
	fseek(fptr, 8, SEEK_SET);
	fwrite(&completed, 4, 1, fptr); //updating the number of entries
	
	completed = ntohl(completed);
	
	//now to put all the index entries into the file
	for(int i = 0; i<completed; i++){
		write_to_index_file(idx_entries[i], &fptr);
	}
	
	//now to generate a hash
	int final_pos = ftell(fptr);
	if(-1 == fseek(fptr, 0, SEEK_SET)){
		fprintf(stderr, "seek operation failed\n");
		exit(EXIT_FAILURE);
	}
	
	unsigned char buffer[final_pos];
	fread(buffer, 1, final_pos, fptr);
	unsigned char sha_val[21];
	sha_val[20] = '\0';
	SHA1(buffer, final_pos, sha_val);
	fseek(fptr, final_pos, SEEK_SET);
	fwrite(sha_val, 1, 20, fptr);
	
	long end = ftell(fptr);
	fflush(fptr);
	if(ftruncate(fileno(fptr), end) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
	}
	
	if(fclose(fptr)){
		fprintf(stderr, "fclose failed\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i=0;i<completed;i++){
		if(idx_entries[i] != &combined){
			free(idx_entries[i]);
			idx_entries[i] = NULL;
		}
	}
	free(entry_to_push);
	free(sha_val_file_ptr);
	sha_val_file_ptr = NULL;
	entry_to_push = NULL;
	
}
