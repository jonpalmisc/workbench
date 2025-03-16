// clang -std=c11 -o tutorial main.c

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

/*
 * The type that you choose for your bitfield members (regardless of how many
 * bits you assign to each) influences the total size of the structure, and
 * does so in a potentially confusing way which varies depending on whether or
 * not you are packing the structure or not. This behavior is described in the
 * standard of course, but I don't wish to memorize it.
 *
 * In my opinion, the easiest way to do this is to do the following:
 *
 *   1. Determine how many bits you need ahead of time and pick the appropriate
 *      integer type, e.g. 'int32_t' if you need 30 bits.
 *   2. Make all struct members/fields that type, regardless of how many bits
 *      you assign to each.
 *
 * By doing this, with every subsequent member, you are just allocating
 * additional bits out of the total number of bits you have available.
 *
 * This assumes you're using bitfields as an abstraction over a peripheral
 * register, etc. or something similar. If you're using bitfields in the middle
 * of a larger structure to save space, then it gets more complicated.
 */
typedef uint64_t bits_int_t;

/*
 * Try seeing how packing affects the structure.
 */
#define PACK_STRUCT 0
#if PACK_STRUCT
#define STRUCT_ATTRIBUTES __attribute__((packed))
#else
#define STRUCT_ATTRIBUTES
#endif

typedef struct STRUCT_ATTRIBUTES {
    bits_int_t a : 20; /* Lowest 20 bits hold field A. */
    bits_int_t b : 12; /* Next 12 bits hold field B. */

    /*
     * So far, we've only declared 32 bits worth of fields. Because we defined
     * our member type to be a 64-bit integer, we still have 32 bits remaining
     * we can use.
     *
     * Try changing HAS_FIELD_C to 1 below to add another member, while
     * observing the structure size does not change.
     */

#define HAS_FIELD_C 0
#if HAS_FIELD_C
    uint64_t c : 12; /* Next 12 bits hold field C. */
#endif

    /*
     * Even with field C, we still have 20 bits remaining.
     *
     * Try changing HAS_FIELD_D to add a final member to the struct, while also
     * changing FIELD_D_BITS and observe the effect on the structure size.
     */

#define HAS_FIELD_D 0
#define FIELD_D_BITS 20
#if HAS_FIELD_D
    uint32_t d : FIELD_D_BITS; /* Finally, we have field D. */
#endif
} bits_t;

/*
 * This tutorial expects you will manipulate the structure to see how it
 * behaves. However, in production, especially if you are describing a
 * peripheral register or something with a known fixed size, it would be a good
 * idea to add a check like the one below.
 */

#if 0
static_assert(sizeof(bits_t) == 8, "did you add too many members?");
#endif

void hexdump(void const *buf, size_t len);

int main(int argc, char const **argv)
{
    (void)argc;
    (void)argv;

    bits_t bits = {
        .a = 0x41,
        .b = 0x567,
#if HAS_FIELD_C
        .c = 0xAA0,
#endif
#if HAS_FIELD_D
        .d = 1 << (FIELD_D_BITS - 1),
#endif
    };

    printf("bits.a = 0x%X\n", bits.a);
    printf("bits.b = 0x%X\n", bits.b);
#if HAS_FIELD_C
    printf("bits.c = 0x%X\n", bits.c);
#endif
#if HAS_FIELD_D
    printf("bits.d = 0x%X\n", bits.d);
#endif

    printf("\nsizeof(bits_t) = %zu\n", sizeof(bits_t));
    hexdump((uint8_t const *)&bits, sizeof(bits));
}

void hexdump(void const *buf, size_t len)
{
    uint8_t const *data = buf;

    for (size_t i = 0; i < len; i += 16) {
        printf("%08zx: ", i);

        for (size_t j = 0; j < 16; ++j) {
            if (i + j < len) {
                printf("%02x ", data[i + j]);
            } else {
                printf("   ");
            }
        }
        printf("  ");

        for (size_t j = 0; j < 16; ++j) {
            if (i + j < len) {
                int c = data[i + j];
                printf("%c", (isprint(c) && c != ' ') ? c : '.');
            }
        }
        printf("\n");
    }
}
