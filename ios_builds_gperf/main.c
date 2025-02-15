#ifdef CONVERT_NAIVE
#include "convert_naive.h"
#endif
#ifdef CONVERT_PHF
#include "convert_phf.h"
#endif

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <build>\n", argv[0]);
		return EXIT_FAILURE;
	}

#ifdef CONVERT_NAIVE
	build_t id = build_from_string(argv[1]);
#endif
#ifdef CONVERT_PHF
	struct result_t* result = build_from_string(argv[1]);
	if (!result){
		fprintf(stderr, "Error: Lookup failed for build '%s'.\n", argv[1]);
		return EXIT_FAILURE;
	}

	build_t id = result->build;
#endif

	printf("%s -> %d\n", argv[1], id);
	return EXIT_SUCCESS;
}
