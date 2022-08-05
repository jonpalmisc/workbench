// Copyright (c) 2022 Jon Palmisciano. All rights reserved.
//
// Use of this source code is governed by the BSD 3-Clause license; the full
// terms of the license can be found in the LICENSE.txt file.

#ifndef TVM_SIM_H
#define TVM_SIM_H

#include <stddef.h>
#include <stdint.h>

/// Program simulator.
typedef struct Simulator Simulator;
struct Simulator {
	uint16_t* code;
	size_t max_pc;
	size_t pc;

	char* buf;
	size_t point;
};

/// Initialize a simulator.
void sim_init(Simulator*, uint16_t* code, size_t len);

/// Reset the simulator.
///
/// De-allocates the work buffer and resets internal state. The simulator must
/// be re-initialized using `sim_init` again before use.
void sim_reset(Simulator*);

/// Perform an insert operation.
///
/// Insert the specified `value` exactly `count` times.
void sim_perform_insert(Simulator*, uint8_t count, uint8_t value);

/// Perform a delete operation.
///
/// Delete the character to the left of the point exactly `count` times.
void sim_perform_delete(Simulator*, uint8_t count);

/// Perform a swap operation.
///
/// Swap the character under the point with the next character, then advance the
/// point's position by exactly one character.
void sim_perform_swap(Simulator*);

/// Execute the instruction under the PC.
void sim_step(Simulator*);

/// Print the current state of the emulator.
///
/// Display the current working buffer and the current point position.
void sim_debug_state(Simulator*);

/// Execute the entire program, printing the simulator state after each step.
void sim_trace_all(Simulator*);

#endif
