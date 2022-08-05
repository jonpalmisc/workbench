// Copyright (c) 2022 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; the full
// terms of the license can be found in the LICENSE.txt file.

#include "insn.h"
#include "sim.h"

#include <stdlib.h>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	size_t len = 0;
	Insn* insns = malloc(sizeof(Insn) * 16);

	// Build a basic program; should produce the string "Hello!".
	insn_init(&insns[len++], OpInsert, 1, 'H');
	insn_init(&insns[len++], OpInsert, 1, 'e');
	insn_init(&insns[len++], OpInsert, 3, 'l');
	insn_init(&insns[len++], OpDelete, 1, '*');
	insn_init(&insns[len++], OpInsert, 1, '!');
	insn_init(&insns[len++], OpInsert, 1, 'o');
	insn_init(&insns[len++], OpBackup, 4, '*');
	insn_init(&insns[len++], OpAdvance, 2, '*');
	insn_init(&insns[len++], OpSwap, 1, '*');

	// Encode the program to binary.
	uint16_t* program = malloc(sizeof(uint16_t) * len);
	for (size_t i = 0; i < len; ++i)
		program[i] = insn_encode(&insns[i]);

	Simulator sim;
	sim_init(&sim, program, len);
	sim_trace_all(&sim);
	sim_reset(&sim);

	free(insns);
	free(program);

	return 0;
}
