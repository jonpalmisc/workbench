// Copyright (c) 2022 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; the full
// terms of the license can be found in the LICENSE.txt file.

#ifndef TVM_INSN_H
#define TVM_INSN_H

#include <stdint.h>

/// Text-editing operation.
typedef enum Op Op;
enum Op {
	OpInsert = 1, // Insert a character.
	OpDelete,     // Delete a character.
	OpSwap,       // Swap the next two characters.
	OpAdvance,    // Move the point forward.
	OpBackup,     // Move the point backward.
};

/// Get the name of an operation.
const char* op_name(Op op);

/// Text-editing VM instruction.
typedef struct Insn Insn;
struct Insn {
	Op op;
	uint8_t count;
	uint8_t value;
};

/// Initialize an instruction.
void insn_init(Insn*, Op op, uint8_t count, uint8_t value);

/// Encode an instruction into binary.
///
/// A 16-bit integer is divided into three fields: 3 bits, operation; 5 bits,
/// count; 7 bits, value.
///
///   15     12        7         0
///   | <op> | <count> | <value> |
///
uint16_t insn_encode(Insn*);

/// Decode an instruction from binary.
///
/// See `insn_encode` for encoding information.
Insn insn_decode(uint16_t raw);

/// Print a debug representation of an instruction.
///
/// Print the instruction's opcode, followed by its decoded form.
void insn_debug(Insn*);

#endif
