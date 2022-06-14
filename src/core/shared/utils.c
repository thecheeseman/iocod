#include "iocod.h"

#define MAX_PATH 256

IC_PUBLIC
bool ic_bit_check(const int arr[], int num)
{
    int i = 0;
    while (num > 31) {
        i++;
        num -= 32;
    }

    return ((arr[i] & (1 << num)) != 0);
}

IC_PUBLIC
void ic_bit_clear(int arr[], int num)
{
    int i = 0;
    while (num > 31) {
        i++;
        num -= 32;
    }

    arr[i] &= ~(1 << num);
}

IC_PUBLIC
void ic_bit_set(int arr[], int num)
{
    int i = 0;
    while (num > 31) {
        i++;
        num -= 32;
    }

    arr[i] |= (1 << num);
}

IC_PUBLIC
IC_NON_NULL(1)
char *ic_skip_path(char *path)
{
    char *last = path;
    while (*path != '\0') {
        if (*path == '/')
            last = path + 1;

        path++;
    }

    return last;
}

IC_PUBLIC
IC_NON_NULL(1, 2)
void ic_strip_extension(const char *in, char *out, size_t size)
{
    size_t len = 0;

    while ((len < size - 1) && *in != '\0' && *in != '.') {
        *out++ = *in++;
        len++;
    }

    *out = '\0';
}

IC_PUBLIC
IC_NON_NULL(1, 2)
void ic_strip_filename(char *in, char *out)
{
    strncpyz(out, in, strlen(in));
    char *end = ic_skip_path(out);
    *end = '\0';
}
