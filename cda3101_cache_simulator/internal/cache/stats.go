package cache

// Stats holds statistics for a cache.
type Stats struct {
	HitCount  uint
	MissCount uint
}

// HitRatio returns the ratio of hits to all lookups.
func (s Stats) HitRatio() float32 {
	return float32(s.HitCount) / float32(s.HitCount+s.MissCount)
}

// Record records the appropriate statistic and passes through the argument.
func (s *Stats) Record(r CacheResult) CacheResult {
	if r == CacheResultHit {
		s.HitCount++
	} else {
		s.MissCount++
	}

	return r
}
