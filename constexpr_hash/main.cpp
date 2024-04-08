#include <cstdio>
#include <string>

constexpr std::size_t ce_strlen(const char *s)
{
    return std::string::traits_type::length(s);
}

constexpr std::size_t ce_wcslen(const wchar_t *s)
{
    return std::wstring::traits_type::length(s);
}

constexpr unsigned int ce_hash(const char *str)
{
    auto length = ce_strlen(str);
    auto bits = (unsigned)(sizeof(unsigned) * 8);
    auto three_fourths = (unsigned)((bits * 3) / 4);
    auto one_eighth = (unsigned)(bits / 8);
    auto high_bits = (unsigned)(0xFFFFFFFF) << (bits - one_eighth);
    unsigned hash = 69;
    unsigned test = 0;

    for (unsigned i = 0; i < length; ++str, ++i) {
        hash = (hash << one_eighth) + (*str);

        if ((test = hash & high_bits) != 0)
            hash = ((hash ^ (test >> three_fourths)) & (~high_bits));
    }

    return hash;
}

constexpr unsigned int magic(char const *symbol)
{
    return ce_hash(symbol);
}

[[clang::noinline]] extern int dyn_resolve_stub(int a, int b)
{
    printf("%x\n", a);
    printf("%x\n", b);
    return a + b;
}

#define dyn_import(DLL, SYMBOL) dyn_resolve_stub(magic(DLL), magic(SYMBOL))

int main(int argc, char const **argv)
{
    (void)argc;
    (void)argv;

    printf("%x\n", dyn_import("kernel32.dll", "VirtualAlloc"));

    return 0;
}
