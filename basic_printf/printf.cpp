#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>

static char const g_hex_tbl_lower[] = "0123456789abcdef";
static char const g_hex_tbl_upper[] = "0123456789ABCDEF";

typedef void (*jp_printf_sink_t)(char c);

int jp_printf_do(jp_printf_sink_t sink_fn, char const *fmt, va_list args)
{
    size_t out_sz = 0;

    auto emit = [&sink_fn, &out_sz](char c) {
        sink_fn(c);
        ++out_sz;
    };

    auto emit_str = [&emit](char const *str) {
        if (!str)
            str = "<null>";
        while (*str)
            emit(*str++);
    };

    auto emit_dec = [&emit_str](uint64_t num, bool is_signed) {
        char buf[32] = { 0 };
        size_t pos = sizeof(buf) - 1;

        bool is_neg = false;
        if (is_signed && (int64_t)num < 0) {
            is_neg = true;
            num = -num;
        }

        while (num >= 10) {
            unsigned digit = num % 10;
            num /= 10;

            buf[--pos] = digit + '0';
        }
        buf[--pos] = num + '0';

        if (is_neg)
            buf[--pos] = '-';

        emit_str(&buf[pos]);
    };

    auto emit_hex = [&emit_str](uint64_t num, bool use_upper) {
        char buf[32] = { 0 };
        size_t pos = sizeof(buf) - 1;

        auto table = use_upper ? g_hex_tbl_upper : g_hex_tbl_lower;

        do {
            unsigned digit = num % 16;
            num /= 16;

            buf[--pos] = table[digit];
        } while (num);

        emit_str(&buf[pos]);
    };

    char c = 0;
    for (;;) {
        // Output normal characters until a percent sign is encountered.
        while ((c = *fmt++)) {
            if (c == '%')
                break;

            emit(c);
        }

        if (!c)
            break;

        bool use_upper = false;
        bool wide_num = false;

    next_fmt:
        c = *fmt++;
        if (!c)
            break;

        switch (c) {
        case '%':
            emit('%');
            break;

        case 'l':
            wide_num = true;
            goto next_fmt;

        case 'i':
        case 'd': {
            uint64_t num = wide_num ? va_arg(args, int64_t) : va_arg(args, int32_t);
            emit_dec(num, true);
            break;
        }
        case 'u': {
            uint64_t num = wide_num ? va_arg(args, uint64_t) : va_arg(args, uint32_t);
            emit_dec(num, false);
            break;
        }

        case 'X':
            use_upper = true;
        case 'x': {
            uint64_t num = wide_num ? va_arg(args, uint64_t) : va_arg(args, uint32_t);
            emit_hex(num, use_upper);
            break;
        }

        case 's': {
            char const *str = va_arg(args, char const *);
            emit_str(str);
            break;
        }

        default:
            break;
        }
    }

    return out_sz;
}

void jp_printf_stdout_sink(char c)
{
    if (c)
        putchar(c);
}

int jp_vprintf(char const *fmt, va_list args)
{
    return jp_printf_do(jp_printf_stdout_sink, fmt, args);
}

__attribute__((__format__(__printf__, 1, 2))) int jp_printf(char const *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int result = jp_vprintf(fmt, args);

    va_end(args);
    return result;
}

int main(int argc, char const **argv)
{
    (void)argc;
    (void)argv;

    auto sz = jp_printf("Hello, %s! %d in decimal is %x in hex. %ld is a big number.\n",
        "world", -123, -123, 1L << 34);
    jp_printf("Printed %d characters.\n", sz);

    return 0;
}
