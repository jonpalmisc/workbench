#include "library.h"

#include <mach-o/dyld.h>

#include <stdio.h>
#include <string.h>

#define OFFSET__S_DID_INIT 0x4000
#define OFFSET__G_MAGIC 0x4004

static uintptr_t get_image_base(char const *target_name)
{
    for (uint32_t i = 0; i < _dyld_image_count(); ++i) {
        char const *current_name = _dyld_get_image_name(i);
        if (NULL == strstr(current_name, target_name))
            continue;

        return (uintptr_t)_dyld_get_image_header(i);
    }

    return -1;
}

static void patch_magic(void)
{
    uintptr_t image_base = get_image_base("libptl.dylib");
    if (-1 == image_base) {
        puts("Failed to find image base!");
        return;
    }

    printf("Library base address: %#lx\n", image_base);

    int *s_did_init = (int *)(image_base + OFFSET__S_DID_INIT);
    int *g_magic = (int *)(image_base + OFFSET__G_MAGIC);

    puts("Current state:");
    printf("  s_did_init=%#x\n", *s_did_init);
    printf("  g_magic=%#x\n", *g_magic);

    puts("Updating magic...");
    *g_magic = 0x77;
}

#define INITIAL_MAGIC 0x41

int main(int argc, char const **argv)
{
    (void)argc;
    (void)argv;

    puts("Initializing magic...");
    ptl_set_magic_once(INITIAL_MAGIC);

    printf("Initial magic value: %#x\n", ptl_get_magic());

    puts("Attempting to re-set magic...");
    ptl_set_magic_once(0x55);

    if (ptl_get_magic() != INITIAL_MAGIC) {
        puts("Magic unexpectedly changed!");
        return 1;
    }

    printf("Magic did not change (expected).\n");

    puts("Patching magic...");
    patch_magic();

    if (ptl_get_magic() == INITIAL_MAGIC) {
        puts("Failed to patch magic!");
        return 1;
    }

    printf("New magic value: %#x\n", ptl_get_magic());

    return 0;
}
