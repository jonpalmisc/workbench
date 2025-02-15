#ifdef CONFIG_PHF

#include "build.h"

static int index_from_str(char const *key)
{
#define NK $NK
#define NG $NG
#define NS $NS

    static int s1[] = {
        $S1
    };
    static int s2[] = {
        $S2
    };
    static int g[] = {
        $G
    };

    int i, f1 = 0, f2 = 0;

    for (i = 0; key[i] != '\0' && i < NS; i++) {
        f1 += s1[i] * key[i];
        f2 += s2[i] * key[i];
    }
    i = (g[f1 % NG] + g[f2 % NG]) % NG;
    return (i < NK) ? i : -1;

#undef NK
#undef NG
#undef NS
}

#include "build_table.h"

build_t build_from_str(char const *str)
{
    int index = index_from_str(str);
    if (index == -1)
        return -1;

    return g_builds[index];
}

#else

#include "build_cmp.h"

#endif
