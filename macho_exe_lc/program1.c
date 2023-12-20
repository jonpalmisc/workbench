#include "stdio.h"

extern int get_magic(void);

int main(int argc, char *argv[])
{
    puts("This is program 1.");
    printf("The magic is %d.\n", get_magic());
}
