#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#define LIB_PATH   "/Applications/Xcode-beta.app/Contents/Developer/Platforms/iPhoneOS.platform/usr/lib/dsc_extractor.bundle"
#define EXTRACT_FN "dyld_shared_cache_extract_dylibs_progress"

typedef void (^dsc_progress_block_t)(unsigned current, unsigned total);
typedef int (*dsc_extract_fn_t)(const char *dsc_path, const char *out_path, dsc_progress_block_t callback);

int main(int argc, char const **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <dsc>\n", argv[0]);
        return EXIT_FAILURE;
    }

    void *lib = dlopen(LIB_PATH, RTLD_NOW);
    if (!lib) {
        fprintf(stderr, "Error: Failed to open 'dsc_extractor' library!\n");
        return EXIT_FAILURE;
    }

    dsc_extract_fn_t extract = dlsym(lib, EXTRACT_FN);
    if (!extract) {
        fprintf(stderr, "Error: Failed to get extractor function!\n");
        return EXIT_FAILURE;
    }

    extract(argv[1], ".", ^(unsigned current, unsigned progress) {
        fprintf(stderr, "Extracting... (%u/%u)\n", current, progress);
    });
}
