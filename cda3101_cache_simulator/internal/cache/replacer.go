package cache

// Replacer defines an interface for various replacement models.
type Replacer interface {
	// Touch records an element access.
	Touch(int)

	// NextRI returns the index of the element that should be replaced next.
	NextRI() int
}

// FifoReplacer is a first-in, first-out replacer.
type FifoReplacer struct {
	nextIndex int
	maxIndex  int
}

// NewFifoReplacer creates a new FIFO replacer.
func NewFifoReplacer(size uint) *FifoReplacer {
	return &FifoReplacer{0, int(size)}
}

func (r *FifoReplacer) Touch(index int) {
	r.nextIndex++

	if r.nextIndex >= r.maxIndex {
		r.nextIndex = 0
	}
}

func (r *FifoReplacer) NextRI() int {
	return r.nextIndex
}

// LruReplacer defines replacer that replaces the least-recently-used element.
type LruReplacer struct {
	history []int
}

// NewLruReplacer creates a new LRU replacer.
func NewLruReplacer(size uint) *LruReplacer {
	return &LruReplacer{make([]int, size)}
}

func (r *LruReplacer) Touch(index int) {
	r.history[index]++
}

func (r *LruReplacer) NextRI() int {
	minIndex := 0
	minTouches := r.history[minIndex]

	for i, t := range r.history {
		if t < minTouches {
			minTouches = t
			minIndex = i
		}
	}

	return minIndex
}

// NewAutoReplacer creates the appropriate replacer given a ReplacePolicy.
func NewAutoReplacer(policy ReplacePolicy, lineCount uint) Replacer {
	if policy == ReplacePolicyFifo {
		return NewFifoReplacer(lineCount)
	}

	return NewLruReplacer(lineCount)
}
