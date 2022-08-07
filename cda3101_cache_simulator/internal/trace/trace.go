package trace

import (
	"cachesim/internal/memory"

	"bufio"
	"fmt"
	"os"
)

// Read reads and parses a trace file.
func Read(path string) ([]memory.Operation, error) {
	ops := []memory.Operation{}

	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}

	reader := bufio.NewReader(file)
	for {
		op := memory.Operation{}

		// TODO: Optimize this because scanning is slow!
		n, err := fmt.Fscanf(reader, "%c 0x%x %d\n",
			&op.Kind, &op.Address, &op.Size)

		// Check error, explicitly ignoring EOF errors.
		if err != nil && err.Error() != "EOF" {
			return nil, err
		}

		// Stop if we are at the end of the file.
		if n == 0 {
			break
		}

		ops = append(ops, op)
	}

	return ops, nil
}
