#pragma once
#include <stdint.h>
#include "list.h"
#include "threads.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

typedef enum
{
	DIR,
	REG
} f_type;

struct file
{
	char name[256];
	uint64_t size;
	uint64_t order;
	void * data;
	f_type type;
	struct list_head head;
	struct spinlock file_lock;
	struct list_head * files;
	size_t files_count;
} __attribute((packed))__;

struct f_descriptor
{
	struct file * file;
	uint64_t offset;
	struct list_head * cur_file;
} ;

struct file_names
{
	struct file_names * next;
	const char * name;
};

void mkdir(const char * name);
struct f_descriptor * open(const char * name);
void close(struct f_descriptor * descr);
size_t read(struct f_descriptor * descr, void * data, size_t size);
void write(struct f_descriptor * descr, void * data, size_t size);
struct file_names * readdir(struct f_descriptor * descr);
void init_fs();
struct f_descriptor * opendir(const char * name);
void closedir(struct f_descriptor * descr, struct file_names * names);
