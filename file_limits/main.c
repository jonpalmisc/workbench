#undef NDEBUG
#include <assert.h>

#include <stdio.h>
#include <sys/resource.h>
#include <sys/sysctl.h>

int main(int argc, char const **argv)
{
    struct rlimit nofile;
    assert(getrlimit(RLIMIT_NOFILE, &nofile) == 0);

    unsigned kernel_max_files = 0;
    size_t out_size = sizeof(kernel_max_files);
    assert(sysctlbyname("kern.maxfilesperproc", &kernel_max_files, &out_size, NULL, 0) == 0);

    printf("nofile.rlim_cur=%llu\n", nofile.rlim_cur);
    printf("nofile.rlim_max=%llu\n", nofile.rlim_max);
    printf("kern.maxfilesperproc=%u\n", kernel_max_files);

    return 0;
}
