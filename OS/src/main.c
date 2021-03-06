#include <serial.h>
#include <memory.h>
#include <balloc.h>
#include <paging.h>
#include <debug.h>
#include <alloc.h>
#include <print.h>
#include <ints.h>
#include <time.h>
#include "fs.h"
#include "initramfs.h"

static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}
/*
static void test_kmap(void)
{
	const size_t count = 1024;
	struct page **pages = mem_alloc(sizeof(*pages) * count);
	size_t i;

	BUG_ON(!pages);
	for (i = 0; i != count; ++i) {
		pages[i] = __page_alloc(0);
		if (!pages[i])
			break;
	}

	char *ptr = kmap(pages, i);

	BUG_ON(!ptr);
	BUG_ON((uintptr_t)ptr < HIGHER_BASE);

	for (size_t j = 0; j != i * PAGE_SIZE; ++j)
		ptr[i] = 13;

	for (size_t j = 0; j != i * PAGE_SIZE; ++j)
		BUG_ON(ptr[i] != 13);

	kunmap(ptr);
	mem_free(pages);
}

static void test_alloc(void)
{
	struct list_head head;
	unsigned long count = 0;

	list_init(&head);
	while (1) {
		struct list_head *node = mem_alloc(sizeof(*node));

		if (!node)
			break;
		BUG_ON((uintptr_t)node < HIGHER_BASE);
		++count;
		list_add(node, &head);
	}

	printf("Allocated %lu bytes\n", count * sizeof(head));

	while (!list_empty(&head)) {
		struct list_head *node = head.next;

		BUG_ON((uintptr_t)node < HIGHER_BASE);
		list_del(node);
		mem_free(node);
	}

	mem_alloc_shrink();
}

static void test_slab(void)
{
	struct list_head head;
	struct mem_cache cache;
	unsigned long count = 0;

	list_init(&head);
	mem_cache_setup(&cache, sizeof(head), sizeof(head));
	while (1) {
		struct list_head *node = mem_cache_alloc(&cache);

		if (!node)
			break;
		BUG_ON((uintptr_t)node < HIGHER_BASE);
		++count;
		list_add(node, &head);
	}

	printf("Allocated %lu bytes\n", count * sizeof(head));

	while (!list_empty(&head)) {
		struct list_head *node = head.next;

		BUG_ON((uintptr_t)node < HIGHER_BASE);
		list_del(node);
		mem_cache_free(&cache, node);
	}

	mem_cache_release(&cache);
}

static void test_buddy(void)
{
	struct list_head head;
	unsigned long count = 0;

	list_init(&head);
	while (1) {
		struct page *page = __page_alloc(0);

		if (!page)
			break;
		++count;
		list_add(&page->ll, &head);
	}

	printf("Allocated %lu pages\n", count);

	while (!list_empty(&head)) {
		struct list_head *node = head.next;
		struct page *page = CONTAINER_OF(node, struct page, ll);

		list_del(&page->ll);
		__page_free(page, 0);
	}
}
*/
static void test_fs(void) {
	mkdir("/data");
	mkdir("/test");
	struct f_descriptor * file = open("/data/in");
	struct f_descriptor * names = opendir("/data");
	struct file_names * name_list = readdir(names);
	printf("in /data/in %d files:\n",names->file->files_count);
	while (name_list)
	{
		printf("%s\n", name_list->name);
		name_list = name_list->next;
	}
	closedir(names, name_list);

	struct f_descriptor * file2 = open("/test/testfile1");
	close(file2);
	struct f_descriptor * file3 = open("/test/testfile2");
	close(file3);
	struct f_descriptor * names2 = opendir("/test");
	struct file_names * name_list2 = readdir(names2);
	printf("in /test %d files:\n",names2->file->files_count);
	while (name_list2)
	{
		printf("%s\n", name_list2->name);
		name_list2 = name_list2->next;
	}
	closedir(names2, name_list2);

	struct f_descriptor * root = opendir("");
	struct file_names * names3 = readdir(root);
	printf("in root/ %d fies:\n",root->file->files_count);
	while (names3)
	{
		printf("%s\n", names3->name);
		names3 = names3->next;
	}
	closedir(root, names3);
	printf("\nRead&write check:\n");

	char* str = "abcd";
	char* str2 = "efg";
	write(file, str, 4);
	write(file, str2, 3);
	char strgot[7];
	strgot[6] = '\0';
	size_t bytes_count = read(file, strgot, 6);
	printf(strgot);
	printf("\nbytes read: %d\n", bytes_count);
	close(file);
	printf("fs checked \n\n");
}



static void test_ram(void) {
	printf("RAM fs check:\n");
	struct f_descriptor * main_d = opendir("/initramfs");
	struct file_names * names = readdir(main_d);
	while (names)
	{
		printf("%s\n", names->name);
		names = names->next;
	}
	closedir(main_d, names);
	printf("\n");
	main_d = opendir("/initramfs/check_dir");
	names = readdir(main_d);
	while (names)
	{
		printf("%s\n", names->name);
		names = names->next;
	}
	closedir(main_d, names);
	printf("\n");
	main_d = opendir("/initramfs/empty_dir");
	names = readdir(main_d);
	while (names)
	{
		printf("%s\n", names->name);
		names = names->next;
	}
	closedir(main_d, names);

	char read_f[16];
	read_f[15] = '\0';
	struct f_descriptor * file = open("/initramfs/check_dir/check_file");
	uint64_t bytes_count = read(file, read_f, 15);
	close(file);
	printf("initramfs check file contains :\n\n %s \n\n", read_f);
	printf("\nbytes read: %d\n", bytes_count);
}


void main(void *bootstrap_info)
{
	qemu_gdb_hang();
	serial_setup();
	ints_setup();
	time_setup();
	balloc_setup(bootstrap_info);
	paging_setup();
	page_alloc_setup();
	mem_alloc_setup();
	kmap_setup();
	enable_ints();
	init_fs();
	load_ram_fs();


	printf("Tests Begin\n");
/*	test_buddy();
	test_slab();
	test_alloc();
	test_kmap();
*/	test_fs();
	test_ram();
	printf("Tests Finished\n");

	while (1);
}
