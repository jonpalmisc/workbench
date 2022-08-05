// Copyright (c) 2022 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; the full
// terms of the license can be found in the LICENSE.txt file.

#include "insn.h"

#include <stdio.h>

#define ENUM_NAME(var)                                                         \
	case var:                                                              \
		return #var;

const char* op_name(Op op)
{
	switch (op) {
		ENUM_NAME(OpInsert);
		ENUM_NAME(OpDelete);
		ENUM_NAME(OpSwap);
		ENUM_NAME(OpAdvance);
		ENUM_NAME(OpBackup);
	}
}

void insn_init(Insn* insn, Op op, uint8_t count, uint8_t value)
{
	insn->op = op;
	insn->count = count;
	insn->value = value;
}

uint16_t insn_encode(Insn* insn)
{
	uint16_t result = 0;
	result |= (insn->op << 13);
	result |= (insn->count << 7);
	result |= insn->value;

	return result;
}

Insn insn_decode(uint16_t raw)
{
	Insn insn;
	insn.value = raw & 0x7f;
	insn.count = (raw >> 7) & 0x3f;
	insn.op = (raw >> 13) & 0x7;

	return insn;
}

void insn_debug(Insn* i)
{
	const char* op = op_name(i->op);
	uint16_t encoded = insn_encode(i);

	printf("%04x\t<%s, %d * '%c'>\n\n", encoded, op, i->count, i->value);
}
