package cache

import (
	"cachesim/internal/memory"
	"math"
)

// CacheResult describes the result of a cache access.
type CacheResult int

const (
	// CacheResultMiss represents a cache miss.
	CacheResultMiss CacheResult = 0

	// CacheResultHit represents a cache hit.
	CacheResultHit CacheResult = 1
)

func (r CacheResult) String() string {
	if r == CacheResultMiss {
		return "miss"
	}

	return "hit"
}

// ReplacePolicy describes how cache lines should be replaced/overwritten.
type ReplacePolicy int

const (
	// ReplacePolicyFifo represents FIFO replacement.
	ReplacePolicyFifo ReplacePolicy = 0

	// ReplacePolicyLru represents LRU replacement.
	ReplacePolicyLru ReplacePolicy = 1
)

func (p ReplacePolicy) String() string {
	if p == ReplacePolicyFifo {
		return "FIFO"
	}

	return "LRU"
}

// Params defines common cache parameters.
type Params struct {
	LineCount uint
	LineSize  uint
	Policy    ReplacePolicy
}

// TotalSize returns the total size of the cache.
func (p Params) TotalSize() uint {
	return p.LineCount * p.LineSize
}

// Common is a struct containing common cache elements.
type Common struct {
	params Params
	stats  Stats
}

// NewParams creates a new Params instance.
func NewParams(lines uint, lineSize uint, policy ReplacePolicy) Params {
	return Params{lines, lineSize, policy}
}

// Cache defines behavior common to all cache types.
type Cache interface {
	Simulate(op memory.Operation) CacheResult
	Stats() Stats
}

// floorLog2 performs Log2(i) and rounds down, returning an unsigned integer.
func floorLog2(i uint) uint {
	return uint(math.Floor(math.Log2(float64(i))))
}

// widthMask returns a bitmask with the first N bits set to 1.
func widthMask(n uint) uint {
	return 0b1111111111111111111111111111111 >> (31 - n)
}
