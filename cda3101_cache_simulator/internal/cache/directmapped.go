package cache

import (
	"cachesim/internal/memory"
)

// DirectMapped is a direct-mapped cache.
type DirectMapped struct {
	Common
	data []int
}

// NewDirectMapped creates a new direct-mapped cache.
func NewDirectMapped(p Params) *DirectMapped {
	data := make([]int, p.LineCount)

	// Initialize all entries to an invalid value.
	for i := 0; i < int(p.LineCount); i++ {
		data[i] = -1
	}

	return &DirectMapped{Common{p, Stats{}}, data}
}

// decompose provides the line number for a given address.
func (c *DirectMapped) decompose(address uint) (int, int) {
	offsetWidth := floorLog2(c.params.LineSize)
	lineWidth := floorLog2(c.params.LineCount)

	lineNumber := (address >> offsetWidth) & widthMask(lineWidth)
	tag := address>>offsetWidth + lineWidth

	return int(lineNumber), int(tag)
}

func (c *DirectMapped) Simulate(op memory.Operation) CacheResult {
	i, tag := c.decompose(op.Address)

	if c.data[i] == tag {
		return c.stats.Record(CacheResultHit)
	}

	c.data[i] = tag
	return c.stats.Record(CacheResultMiss)
}

func (c *DirectMapped) Stats() Stats {
	return c.stats
}
