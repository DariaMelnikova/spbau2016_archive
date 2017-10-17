#include "initramfs.h"
#include "balloc.h"
#include <stdint.h>
#include "balloc.h"
#include "fs.h"
#include "string.h"
#include "memory.h"
#include "print.h"
#include "alloc.h"

uint64_t ram_fs_begin;
uint64_t ram_fs_end;



uint64_t read_hex(const char * text, size_t size)
{
	size_t i = 0;
	uint32_t result = 0;
	while(i < size)
	{
		if (text[i] <= '9' && text[i] >= '0')
			result = text[i] - '0' + result * 16;
		else
			result = text[i] - 'A' + 10 + result * 16;
		i++;
	}
	return result;
}

struct module 
{
	uint32_t begin;
	uint32_t end;
	uint32_t string;
	uint32_t reserved;
} __attribute__((packed));

uintptr_t mod4(uintptr_t a)
{
	if (a % 4 == 0)
		return a;
	else
		return ((a / 4) + 1) * 4;
}


uint64_t load_item(uint64_t start)
{
	start = mod4(start);
	if (start >=ram_fs_end )
		return start;
	struct cpio_header * cpio = (struct cpio_header *)va(start);

	uint32_t mode = read_hex(cpio->mode, 8);
	uint64_t name_len = read_hex(cpio->namesize, 8);
	uint64_t file_len = read_hex(cpio->filesize, 8);
	start += sizeof(struct cpio_header);
	if (start >=ram_fs_end )
		return start;

	char * name = mem_alloc(name_len + 2);
	strncpy(name+1,va(start), name_len);

	name[0]='/';
	name[name_len]='\0';
	if (strncmp(name+1, "TRAILER!!!", 10) == 0)
		return ram_fs_end;
	start += name_len;
	if (start >=ram_fs_end )
		return start;
	start = mod4(start);
	if(S_ISREG(mode))
	{
		struct f_descriptor * file = open(name);
		write(file, va(start), file_len);
		close(file);
	}
	else
		mkdir(name);
	printf("loaded %s\n", name);
	mem_free(name);
	start += file_len;
	return start;


}


void load_ram_fs()
{
	uint64_t start = ram_fs_begin;
	uint64_t end = ram_fs_end;
	while (start < end)
		start = load_item(start);
}



void init_ram(const struct mboot_info * mboot)
{

	if (!((1 << 3) & mboot->flags))
		return;
	uint32_t mods_count = mboot->mods_count;

	struct module * mods_addr = (struct module *)(uint64_t)mboot->mods_addr;
	for (uint32_t i = 0; i < mods_count; ++i)
	{

		uint64_t st = (uint64_t)mods_addr[i].begin;
		uint64_t fin = (uint64_t)mods_addr[i].end;
		if (fin-st < 7)
			continue;
		char * string = (char *)(uint64_t)mods_addr[i].begin;
		if (strncmp(string, MAGIC, 6)==0)
		{
			ram_fs_begin = st;
			ram_fs_end = fin;
			printf("ramfs initialized\n");
			break;
		}
	}
}
