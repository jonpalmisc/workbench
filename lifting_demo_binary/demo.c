#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int arithmetic_square(int num)
{
    return num * num;
}

int arithmetic_add_7(int num)
{
    return num + 7;
}

int arithmetic_sum(int *numbers, size_t count)
{
    int sum = 0;

    for (size_t i = 0; i < count; ++i)
        sum += numbers[i];

    return sum;
}

struct String {
    char const *data;
    size_t length;
};

char const *g_null_string = "<null>";

char const *string_data_safe(struct String const *string)
{
    if (!string || !string->data)
        return g_null_string;

    return string->data;
}

size_t string_length(struct String const *string)
{
    if (!string)
        return 0;

    return string->length;
}

int example_ssa_small()
{
    int x = rand();
    x += 3;

    if (x % 2 == 0)
        x *= 4;
    else
        x -= 1;

    return x;
}

int example_ssa_medium(int p1, int p2)
{
    int x = p1 + 2;

    if (p1) {
        x += 7;

        if (p2 < 8)
            x /= 2;
    } else {
        x *= 3;

        if (p2 > 12)
            x -= 4;
    }

    return x;
}
