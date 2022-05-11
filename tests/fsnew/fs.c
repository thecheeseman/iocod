#include "fs.h"

int main(int argc, char* argv[])
{
    struct pak *pak = fs_load_zip_file("js023730042013.pk3", "main");
    if (pak != NULL)
        ic_printf("%s\n", pak->filename);

    return 0;
}
