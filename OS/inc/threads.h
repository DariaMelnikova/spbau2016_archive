#ifndef THREADS
#define THREADS
#include <stdatomic.h>

typedef struct spinlock 
{
	atomic_int locked;
} spinlock;

void lock (struct spinlock * lock);
void unlock (struct spinlock * lock);
#endif
