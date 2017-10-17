#include "threads.h"
#include <stdatomic.h>


void lock (struct spinlock *lock)
{
	do {
		while (atomic_load_explicit(&lock->locked, memory_order_relaxed) == 1);
	} while (atomic_exchange_explicit(&lock->locked, 1, memory_order_acquire) == 1);
}

void unlock (struct spinlock * lock)
{
	atomic_store_explicit(&lock->locked, 0, memory_order_release);
}
