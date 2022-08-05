// Copyright (c) 2022 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; the full
// terms of the license can be found in the LICENSE.txt file.

#include "sim.h"

#include "insn.h"

#include <stdio.h>
#include <stdlib.h>

void sim_init(Simulator* sim, uint16_t* code, size_t len)
{
	sim->code = code;
	sim->max_pc = len;
	sim->pc = 0;

	sim->point = 0;
	sim->buf = calloc(len, 2);
}

void sim_reset(Simulator* sim)
{
	sim->point = 0;
	sim->buf = NULL;
	free(sim->buf);
}

void sim_perform_insert(Simulator* sim, uint8_t count, uint8_t value)
{
	for (uint8_t i = 0; i < count; ++i)
		sim->buf[sim->point++] = value;
}

void sim_perform_delete(Simulator* sim, uint8_t count)
{
	for (uint8_t i = 0; i < count; ++i)
		sim->buf[--sim->point] = ' ';
}

void sim_perform_swap(Simulator* sim)
{
	char current = sim->buf[sim->point];
	char next = sim->buf[sim->point + 1];

	sim->buf[sim->point] = next;
	sim->buf[++sim->point] = current;
}

void sim_step(Simulator* sim)
{
	Insn insn = insn_decode(sim->code[sim->pc]);

	switch (insn.op) {
	case OpInsert:
		sim_perform_insert(sim, insn.count, insn.value);
		break;
	case OpDelete:
		sim_perform_delete(sim, insn.count);
		break;
	case OpSwap:
		sim_perform_swap(sim);
		break;
	case OpAdvance:
		sim->point += insn.count;
		break;
	case OpBackup:
		sim->point -= insn.count;
		break;
	}

	++sim->pc;
}

#define SIM_DEBUG_INDENT "    "

void sim_debug_state(Simulator* sim)
{
	printf(SIM_DEBUG_INDENT "%s\n", sim->buf);
	printf(SIM_DEBUG_INDENT);

	// Print spaces up to point position.
	for (size_t i = 0; i < sim->point; ++i)
		putchar(' ');

	puts("^\n");
}

void sim_trace_all(Simulator* sim)
{
	while (sim->pc < sim->max_pc) {
		printf("--> 0x%04lx:  ", sim->pc * 2);

		Insn insn = insn_decode(sim->code[sim->pc]);
		insn_debug(&insn);

		sim_step(sim);
		sim_debug_state(sim);
	}
}
