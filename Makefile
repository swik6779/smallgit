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
       commands/commit.c \
       core/object.c \
       core/utils.c 

rec: $(SRCS)
	$(CC) $(CFLAGS) -o rec $(SRCS) $(LIBS)

clean:
	rm -f rec

.PHONY: clean
