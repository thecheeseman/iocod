#include "../icheaders/iocod.h"
#include <stdio.h>
#include <time.h>

#ifdef IC_PLATFORM_WINDOWS
#include "dirent.h"
#else
#include <dirent.h>
#endif

int main(void)
{
    srand(time(NULL));

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name);
        }

        closedir(dir);
    }

    return 0;
}
