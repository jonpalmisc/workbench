/*
 * Copyright (c) 2022 Jon Palmisciano. All rights reserved.
 *
 * Use of this source code is governed by the BSD 3-Clause license; the full
 * terms of the license can be found in the LICENSE.txt file.
 */

#include <stdio.h>

#include <mach/clock.h>
#include <mach/mach.h>

/* ===--------------------------------------------------------------------=== */

volatile uint32_t g_timer = 0x66666666; /* Fake nanosecond timer */
clock_serv_t g_clock;                   /* Mach clock for time queries */
static int g_clock_initialized = 0;     /* Clock service initialization state */

/**
 * Simulate a clock tick, updating the timer value.
 */
void tick(void)
{
	mach_timespec_t time;

	/* Initialize clock service if needed. */
	if (!g_clock_initialized)
		host_get_clock_service(mach_host_self(), REALTIME_CLOCK, &g_clock);

	clock_get_time(g_clock, &time);
	g_timer = time.tv_nsec;
}

/* ===--------------------------------------------------------------------=== */

typedef unsigned int entropy_t; /* Entropy result size/type */

/**
 * Get a random value. Should roughly emulate the following assembly:
 *
 *       mov     x1, x0
 *       mov     w0, #0
 *       mov     w2, #32
 *   1:
 *       mov     w3, w2
 *   2:
 *       subs    w3, w3, #1
 *       b.ne    2b
 *       ldr     w3, [x1]
 *       and     w3, w3, #1
 *       orr     w0, w3, w0, lsl #1
 *       subs    w2, w2, #1
 *       b.ne    1b
 *       ret
 *
 * The procedure above begins with X1 holding the address to a MMIO timer.
 */
entropy_t entropy(void)
{
	uint32_t result = 0;

	for (int i = 0; i < sizeof(entropy_t) * 8; ++i) {
		for (int j = 0; j < 64; ++j)
			asm volatile("nop");

		tick();
		result = (result << 1) | (g_timer & 1);
	}

	return result;
}

/* ===--------------------------------------------------------------------=== */

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	for (int i = 0; i < 32; ++i)
		printf("%08x\n", entropy());

	mach_port_deallocate(mach_task_self(), g_clock);
}
