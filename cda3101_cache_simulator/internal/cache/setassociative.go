package cache

import (
	"cachesim/internal/memory"
)

// SetAssociative is a set-associative cache.
type SetAssociative struct {
	Common
	data      []int
	setSize   uint
	replacers []Replacer
}

// NewSetAssociative creates a new set-associative cache.
func NewSetAssociative(setSize uint, p Params) *SetAssociative {
	setCount := p.LineCount / setSize

	replacers := make([]Replacer, setCount)
	for i := 0; i < int(setCount); i++ {
		replacers[i] = NewAutoReplacer(p.Policy, setSize)
	}

	return &SetAssociative{
		Common{p, Stats{}},
		make([]int, p.LineCount),
		setSize,
		replacers,
	}
}

// decompose extracts the set and tag from an address.
func (c *SetAssociative) decompose(address uint) (int, int) {
	offsetWidth := floorLog2(c.params.LineSize)

	setCount := c.params.LineCount / c.setSize
	setWidth := floorLog2(setCount)

	set := (address >> offsetWidth) & widthMask(setWidth)
	tag := address >> (offsetWidth + setWidth)

	return int(set), int(tag)
}

func (c *SetAssociative) Simulate(op memory.Operation) CacheResult {
	set, tag := c.decompose(op.Address)

	offset := set * int(c.setSize)
	end := offset + int(c.setSize)
	for i := offset; i < end; i++ {
		if c.data[i] == tag {
			c.replacers[set].Touch(i - offset)

			return c.stats.Record(CacheResultHit)
		}
	}

	ri := c.replacers[set].NextRI()
	c.replacers[set].Touch(ri)

	c.data[offset+ri] = tag

	return c.stats.Record(CacheResultMiss)
}

func (c *SetAssociative) Stats() Stats {
	return c.stats
}
