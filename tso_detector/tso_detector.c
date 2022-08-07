/*
 * Adapted from github.com/saagarjha/TSOEnabler/blob/master/testtso/main.c.
 */

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define BUFFER_SIZE 1024 * 8        /* Size of work buffer in elements */
#define ITER_LIMIT  BUFFER_SIZE * 4 /* Iteration limit for success */

atomic_uint g_total_iterations;
atomic_uint g_buffer[BUFFER_SIZE];

/**
 * Assert that consecutive values in the buffer are equal or increasing.
 *
 * Using a four-element window into a larger buffer (pictured below), this
 * function ensures that the element at point A is always greater than or equal
 * to the element at point B.
 *
 *     A B
 *   7 7 8 7
 *
 * Per the routine defined in `write`, this assertion is expected to always be
 * true on processors that implement total store ordering, e.g. x86.
 */
void assert_order(unsigned iter_count, unsigned v1, unsigned v2)
{
	if (v1 >= v2)
		return;

	fprintf(stderr, "Memory reordering detected after %d iterations.\n", iter_count);
	exit(1);
}

/**
 * Continuously increment the value of each index in the global buffer.
 *
 * A single pass through a 4-element buffer might look like:
 *
 *   0 0 0 0
 *   1 0 0 0
 *   1 1 0 0
 *   1 1 1 0
 *   1 1 1 1
 */
void* write(void* arg)
{
	(void)arg;
loop:
	for (size_t i = 0; i < BUFFER_SIZE; ++i)
		atomic_fetch_add_explicit(g_buffer + i, 1, memory_order_relaxed);

	atomic_fetch_add_explicit(&g_total_iterations, 1, memory_order_release);

	goto loop;
	return NULL;
}

/**
 * Continuously read pairs of indices from the global buffer, checking for store
 * reordering with `assert_order`.
 */
void* read(void* arg)
{
	(void)arg;

	unsigned int count = 0;
loop:
	for (size_t i = 0; i < BUFFER_SIZE - 1; ++i) {
		unsigned int v2 = atomic_load_explicit(g_buffer + i + 1, memory_order_relaxed);
		unsigned int v1 = atomic_load_explicit(g_buffer + i, memory_order_relaxed);

		assert_order(count, v1, v2);
	}

	while (count == atomic_load_explicit(&g_total_iterations, memory_order_acquire)) { }
	++count;

	/* Stop if `ITER_LIMIT` iterations have been performed without any
	 * detected memory reordering. The program should terminate this way
	 * every time on x86. */
	if (count >= ITER_LIMIT) {
		printf("%d iterations completed; memory reordering not detected.\n", count);
		exit(0);
	}

	goto loop;
	return NULL;
}

int main()
{
	pthread_t write_thread;
	pthread_create(&write_thread, NULL, write, NULL);

	pthread_t read_thread;
	pthread_create(&read_thread, NULL, read, NULL);

	pthread_join(read_thread, NULL);
	pthread_join(write_thread, NULL);
}
