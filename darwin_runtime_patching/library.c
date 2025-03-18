#include "library.h"

static int g_magic = 0;

void ptl_set_magic_once(int magic)
{
    static int s_did_init = 0;

    if (s_did_init)
        return;

    g_magic = magic;
    s_did_init = 1;
}

int ptl_get_magic(void)
{
    return g_magic;
}
