#include <stdint.h>
#include "fs.h"
#include "memory.h"
#include "alloc.h"
#include "string.h"
#include "print.h"
#include "limits.h"



void insert_file(struct file * dir, struct file * file)
{
	if (dir->files)
		list_add_tail(&file->head, dir->files);
	else
	{
		dir->files = &file->head;
	}
	dir->files_count++;
}

void init_file(struct file * file, const char * name, f_type type)
{
	strncpy(file->name, name, strlen(name));
	file->size = 0;
	file->type = type;
	list_init(&file->head);
	file->file_lock.locked = 0;
	file->data = NULL;
	file->files = NULL;
	file->order = 0;
	file->files_count = 0;
}

struct file root;

void init_fs()
{
	init_file(&root, "", DIR);
}

void init_descr(struct f_descriptor * descr, struct file * file, uint64_t offset)
{
	descr->file = file;
	descr->cur_file = file->files;
	descr->offset = offset;

}

void make_file(struct file * file, const char * name)
{
	init_file(file, name, REG);
}

struct file * find_file(f_type type, struct file * dir, const char * name, size_t name_end)
{
	if(dir->files == NULL)
		return NULL;
	struct list_head * cur = dir->files;
	struct list_head * fst = cur;
	for (;;)
	{
		struct file * cur_file = LIST_ENTRY(cur, struct file, head);
		if (strncmp(cur_file->name, name, name_end) == 0 && cur_file->type == type)
		{
			unlock(&dir->file_lock);	
			return cur_file;
		}
		cur = cur->next;
		if (cur == fst)
			break;
	}
	return NULL;
}

size_t cur_dir_name(const char * name, size_t pos)
{
	if (name[pos]=='/')
		pos++;
	while ( pos<strlen(name))
	{
		if (name[pos]=='/')
			return pos;
		pos++;
	}
	return 0;
}


struct file * find_dir(const char * name)
{
	size_t pos = 0;
	struct file * cur = &root;
	for(;;)
	{
		size_t name_end = cur_dir_name(name, pos);

		if (name_end == 0)
			return cur;
		pos = name_end;

		cur = find_file(DIR, cur, name, name_end);

		if (cur == NULL)
			return NULL;
	}
}



void mkdir(const char * name)
{
	struct file * cur = find_dir(name);
	lock(&cur->file_lock);
	if (!find_file(DIR, cur, name, strlen(name)))
	{
		struct file * dir = (struct file *)mem_alloc(0);
		init_file(dir, name, DIR);
		insert_file(cur, dir);
	}
	unlock(&cur->file_lock);
}

struct f_descriptor * open (const char * name)
{

	struct file * dir = find_dir(name);
	if(!dir)
		return NULL;
	lock(&dir->file_lock);
	struct file * file = find_file(REG, dir, name, strlen(name));
	if (file)
		lock(&file->file_lock);
	else
	{
		file = mem_alloc(sizeof(struct file));
		init_file(file, name, REG);
		lock(&(file->file_lock));
		insert_file(dir, file);
	}
	struct f_descriptor * descr = (struct f_descriptor *)mem_alloc(sizeof(struct f_descriptor));
	init_descr(descr, file, 0);
	unlock(&file->file_lock);
	unlock(&dir->file_lock);
	return descr;
} 

void close(struct f_descriptor * descr)
{

	lock(&descr->file->file_lock);
	mem_free(descr);
	unlock(&descr->file->file_lock);
}

size_t read(struct f_descriptor * descr, void * data, size_t size)
{
	lock(&descr->file->file_lock);
	size_t read = 0;
	char * r_data = data;
	while (descr->offset < descr->file->size && read < size)
	{
		r_data[read] = ((char *)descr->file->data)[descr->offset];
		descr->offset++;
		read++;
	}
	unlock(&descr->file->file_lock);
	return read;
}


void write(struct f_descriptor * descr , void * data, size_t size)
{
	lock(&descr->file->file_lock);
	if (!data)
		return;
	int old_size = descr->file->size;
	size_t new_size = size + old_size;

	char * new = mem_alloc(new_size);
	if (!new)
		return;

	char * iter = new;
	char * file_iter = descr->file->data;
	char * data_iter = data;
	for (int i = 0; i < old_size - 1; ++i)
		*iter++ = *file_iter++;
	if (old_size > 0)
		*iter++ = *file_iter;
	for (int i = 0; i < (int)size - 1; ++i)
		*iter++ = *data_iter++;
	if ((int)size > 0)
		*iter = *data_iter;

	mem_free(descr->file->data);

	descr->file->data = new;
	descr->file->size = new_size;
	unlock(&descr->file->file_lock);

}

struct f_descriptor * opendir(const char * name)
{
	if (strlen(name) == 0)
	{
		struct f_descriptor * descr = (struct f_descriptor *)mem_alloc(sizeof(struct f_descriptor));
		init_descr(descr, &root, 0);
		return descr;
	}

	struct file * dir = find_dir(name);
	if(!dir)
		return NULL;
	struct file * file = find_file(DIR, dir, name, strlen(name));
	if (file)
		lock(&file->file_lock);
	else
	{
		unlock(&file->file_lock);
		return 0;
	}
	struct f_descriptor * descr = (struct f_descriptor *)mem_alloc(sizeof(struct f_descriptor));
	init_descr(descr, file, 0);
	unlock(&file->file_lock);
	return descr;
}


void closedir(struct f_descriptor * descr, struct file_names * names)
{
	lock(&descr->file->file_lock);
	if (names)
	{
		size_t f_c = descr->file->files_count;
		struct file_names * cur = names;
		for (size_t i = 0; i < f_c; ++i)
		{
			struct file_names * next  = cur->next;
			mem_free(cur);
			cur = next;
		}
	}
	mem_free(descr);	
	unlock(&descr->file->file_lock);
}

struct file_names * readdir(struct f_descriptor * descr)
{
	if (!descr->file->files_count)
	{
		return 0;
	}
	lock(&descr->file->file_lock);
	struct file_names * result = (struct file_names *)mem_alloc(sizeof(struct file_names));
	struct file * cur_file = LIST_ENTRY(descr->file->files, struct file, head);
	result->name = cur_file->name;
	result->next = NULL;
	struct file_names * cur = result;
	size_t f_c = descr->file->files_count;
	for (size_t i = 1; i < f_c; ++i)
	{
		cur_file = LIST_ENTRY(cur_file->head.next, struct file, head);
		struct file_names * next = (struct file_names *)mem_alloc(sizeof(struct file_names));
		next->name = cur_file->name;
		next->next = NULL;
		cur->next  = next;
		cur = next;
	}
	unlock(&descr->file->file_lock);
	return result;
}

