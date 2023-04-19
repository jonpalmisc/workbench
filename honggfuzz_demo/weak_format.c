#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define OBJECT_MAGIC   0x747365747a7a7566
#define OBJECT_VERSION 4

typedef enum {
    ErrorOk = 0,
    ErrorBadMagic,
    ErrorBadVersion
} Error;

typedef struct __attribute__((packed)) {
    uint64_t magic;
    uint32_t version;
    uint32_t reserved;
    uint32_t num_a;
    uint32_t num_b;
} Object;

Error object_validate(Object const *object)
{
    if (object->magic != OBJECT_MAGIC)
        return ErrorBadMagic;
    if (object->version != OBJECT_VERSION)
        return ErrorBadVersion;

    uint32_t test_value = object->num_a ^ object->num_b;
    if (test_value % 73 == 0)
        abort();

    return ErrorOk;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Object object;

    FILE *input_file = fopen(argv[1], "rb");
    size_t read = fread(&object, 1, sizeof(Object), input_file);
    if (read < sizeof(Object)) {
        fprintf(stderr, "error: incomplete input (%zu)\n", read);
        return EXIT_FAILURE;
    }

    Error error = object_validate(&object);
    switch (error) {
    case ErrorBadMagic:
        fprintf(stderr, "error: bad magic (%lu)\n", object.magic);
        break;
    case ErrorBadVersion:
        fprintf(stderr, "error: bad version (%u)\n", object.version);
        break;
    default:
        printf("ok\n");
        break;
    }
    return error == ErrorOk ? EXIT_SUCCESS : EXIT_FAILURE;
}
