#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
void *memcpy(void *dst, const void *src, size_t size);
void *memset(void *dst, int fill, size_t size);
char* strncpy(char *destination, const char *source, uint32_t length);
int strncmp(const char *str1, const char *str2, uint32_t length);

#endif /*__STRING_H__*/
