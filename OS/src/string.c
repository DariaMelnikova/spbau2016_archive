#include <string.h>

size_t strlen(const char *str)
{
	const char *begin = str;

	while (*str++);
	return str - begin - 1;
}

void *memcpy(void *dst, const void *src, size_t size)
{
	char *to = dst;
	const char *from = src;

	while (size--)
		*to++ = *from++;
	return dst;
}

void *memset(void *dst, int fill, size_t size)
{
	char *to = dst;

	while (size--)
		*to++ = fill;
	return dst;
}

int strncmp(const char *str1, const char *str2, uint32_t length)
{
	for (int i = 0; i < (int) length; i++)
		if (str1[i] != str2[i])
			return str1[i] - str2[i];
	return 0;
}

char* strncpy(char *destination, const char *source, uint32_t length)
{
	for (int i = 0; i < (int) length; i++)
		destination[i] = source[i];
	return destination;
}
