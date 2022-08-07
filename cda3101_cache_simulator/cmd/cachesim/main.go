package main

import (
	"cachesim/internal/cache"
	"cachesim/internal/trace"
	"time"

	"flag"
	"fmt"
	"os"

	"golang.org/x/text/language"
	"golang.org/x/text/message"
)

// exitShowUsage shows the program usage and exits.
func exitShowUsage() {
	fmt.Println("usage: cachesim [-options] <trace>")
	flag.PrintDefaults()

	os.Exit(0)
}

func main() {
	var lineCount, lineSize, setWidth uint
	var kindString, policyString string
	var verbose, csvOut bool

	// Configure and parse flags.
	flag.UintVar(&lineCount, "lines", 4, "number of cache lines")
	flag.UintVar(&lineSize, "width", 8, "width of each cache line")
	flag.UintVar(&setWidth, "span", 1, "number of lines in a set")
	flag.StringVar(&kindString, "kind", "dm", "kind of cache")
	flag.StringVar(&policyString, "policy", "fifo", "line replacement policy")
	flag.BoolVar(&verbose, "log", false, "use verbose logging")
	flag.BoolVar(&csvOut, "csv", false, "output results as CSV")
	flag.Usage = exitShowUsage
	flag.Parse()

	// Retrieve the remaining arguments.
	args := flag.Args()
	if len(args) != 1 {
		exitShowUsage()
	}

	// Verify the line count is valid.
	if lineCount == 0 || lineCount&(lineCount-1) != 0 {
		fmt.Println("error: line count must be a power of two")
		os.Exit(1)
	}

	// Verify the set width is valid.
	if lineCount%setWidth != 0 {
		fmt.Printf("error: line count not a multiple of set size (%v %% %v != 0)\n",
			lineCount, setWidth)
		os.Exit(1)
	}

	// Parse the trace file, benchmarking the process.
	parseStart := time.Now()
	ops, err := trace.Read(args[0])
	if err != nil {
		fmt.Printf("error: failed to read trace file (%v)\n", err)
		os.Exit(1)
	}

	// Finish recording parsing performance.
	parseDuration := time.Since(parseStart)

	// Choose the replacement policy from the user's input.
	repPolicy := cache.ReplacePolicyFifo
	if policyString == "lru" {
		repPolicy = cache.ReplacePolicyLru
	}

	// Create the parameters for the cache from the user's input.
	userParams := cache.NewParams(lineCount, lineSize, repPolicy)

	// Create the cache that will be used for simulation.
	var userCache cache.Cache
	var kindCode, kindDisplayName string
	switch kindString {
	case "sa":
		userCache = cache.NewSetAssociative(setWidth, userParams)
		kindCode = "SA"
		kindDisplayName = fmt.Sprintf("Set Associative (%v sets)",
			userParams.LineCount/setWidth)
	case "fa":
		userCache = cache.NewFullyAssociative(userParams)
		kindCode = "FA"
		kindDisplayName = "Fully Associative"
	default:
		userCache = cache.NewDirectMapped(userParams)
		kindCode = "DM"
		kindDisplayName = "Direct Mapped"
	}

	// Perform and benchmark the simulation.
	simStart := time.Now()
	for _, op := range ops {
		r := userCache.Simulate(op)

		if verbose {
			fmt.Printf("0x%08x \t%v\n", op.Address, r)
		}
	}

	// Stop profiling the simulation.
	simDuration := time.Since(simStart)

	stats := userCache.Stats()

	if csvOut {
		// Kind, Total Size, Line Count, Line Width, Set Width, Set Count, Policy, Hit Rate
		fmt.Printf("%s,%v,%v,%v,%v,%v,%v,%f\n",
			kindCode,
			userParams.TotalSize(),
			userParams.LineCount,
			userParams.LineSize,
			setWidth,
			userParams.LineCount/setWidth,
			userParams.Policy,
			stats.HitRatio())
		os.Exit(0)
	}

	p := message.NewPrinter(language.English)

	p.Println("Cache configuration:")
	p.Printf("  Model                 %v\n", kindDisplayName)
	p.Printf("  Total size            %v bytes\n", userParams.TotalSize())
	p.Printf("  Line count            %v\n", userParams.LineCount)
	p.Printf("  Line width            %v bytes\n", userParams.LineSize)
	p.Printf("  Replacement policy    %v\n\n", userParams.Policy)

	p.Println("Simulation results:")
	p.Printf("  Total trials          %v\n", stats.HitCount+stats.MissCount)
	p.Printf("  Cache hits            %v\n", stats.HitCount)
	p.Printf("  Cache misses          %v\n", stats.MissCount)
	p.Printf("  Hit ratio             %.2f%%\n\n", stats.HitRatio()*100.0)

	p.Println("Performance summary:")
	p.Printf("  Parse trace           %v\n", parseDuration)
	p.Printf("  Run simulation        %v\n", simDuration)
	p.Printf("  Total runtime         %v\n", parseDuration+simDuration)
}
