#include "build.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <build>\n", argv[0]);
        return EXIT_FAILURE;
    }

    build_t build = build_from_str(argv[1]);
    if (BUILD_INVALID == build) {
        fprintf(stderr, "Error: Unknown build: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("%s -> %d\n", argv[1], build);
    return EXIT_SUCCESS;
}
