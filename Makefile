CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = -lcrypto -lz

SRCS = main.c \
       commands/init.c \
       commands/hash_object.c \
       commands/cat_file.c \
       commands/write_tree.c \
       commands/commit_tree.c \
       commands/add.c \
       commands/add_all.c \
       core/object.c \
       core/utils.c 

sgit: $(SRCS)
	$(CC) $(CFLAGS) -o sgit $(SRCS) $(LIBS)

clean:
	rm -f sgit

.PHONY: clean
