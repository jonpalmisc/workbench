#include "stdio.h"

extern int get_magic(void)
{
    return 666;
}

int main(int argc, char *argv[])
{
    puts("This is program 2.");
    printf("The magic is %d.\n", get_magic());
}
