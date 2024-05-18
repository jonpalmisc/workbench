#include <cstdint>
#include <cstdio>

#define NO_INLINE __attribute__((noinline))

struct base_t {
    char const *m_a;
    uint64_t m_gap[4];
    uint64_t m_b;
};

NO_INLINE void base_init(char const *a, uint64_t b, base_t *self)
{
    // Optimization barrier to prevent Clang from making a structure on the
    // caller's stack and passing it to this function instead of separate
    // arguments in registers.
    asm volatile("add   x1, x1, x1");

    self->m_a = a;
    self->m_b = b;
}

void base_dump(base_t *self)
{
    printf("base_t<%s, %llu>\n", self->m_a, self->m_b);
}

struct sub_t : public base_t {
    uint64_t m_c;
};
NO_INLINE void sub_init(char const *a, uint64_t b, uint64_t c, sub_t *self)
{
    // See optimization barrier comment above.
    asm volatile("add   x2, x2, x2");

    base_init(a, b, self);
    self->m_c = c;
}

void sub_dump(sub_t *self)
{
    printf("sub_t<%s, %llu, %llu>\n", self->m_a, self->m_b, self->m_c);
}

char const *g_a = "BaseClass";
uint64_t volatile g_b = 2;
uint64_t volatile g_c = 4;
char const *g_a2 = "__Subclass";
uint64_t volatile g_b2 = 2;
uint64_t volatile g_c2 = 4;

int main(int argc, char const **argv)
{
    (void)argc;
    (void)argv;

    base_t base;
    base_init(g_a, g_b, &base);

    sub_t sub;
    sub_init(g_a2, g_b2, g_c2, &sub);

    base_dump(&base);
    sub_dump(&sub);

    return 0;
}
