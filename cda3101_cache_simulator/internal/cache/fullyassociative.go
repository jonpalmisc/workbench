package cache

import "cachesim/internal/memory"

// FullyAssociative is a fully-associative cache.
type FullyAssociative struct {
	Common
	data     []int
	replacer Replacer
}

// NewFullyAssociative creates a new fully-associative cache.
func NewFullyAssociative(p Params) *FullyAssociative {
	return &FullyAssociative{
		Common{p, Stats{}},
		make([]int, p.LineCount),
		NewAutoReplacer(p.Policy, p.LineCount),
	}
}

// tag extracts the tag from an address.
func (c *FullyAssociative) tag(address uint) int {
	offsetWidth := floorLog2(c.params.LineSize)

	return int(address >> offsetWidth)
}

func (c *FullyAssociative) Simulate(op memory.Operation) CacheResult {
	tag := c.tag(op.Address)

	for i, v := range c.data {
		if v == tag {
			c.replacer.Touch(i)

			return c.stats.Record(CacheResultHit)
		}
	}

	ri := c.replacer.NextRI()
	c.replacer.Touch(ri)

	c.data[ri] = tag

	return c.stats.Record(CacheResultMiss)
}

func (c *FullyAssociative) Stats() Stats {
	return c.stats
}
