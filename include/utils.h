#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

char dec_to_hex(uint8_t val);
char *hexadecimal_equivalent(unsigned char ch, char hex[]);
char *byte_to_hex(unsigned char *sha_val);

#endif
