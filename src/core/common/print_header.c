#include "com_local.h"

IC_PUBLIC
void ic_print_header(const char *text, size_t size, char sep)
{
    size_t len = strlen(text);
    size_t i;
    size_t j;
    
    if (size < len) {
        log_warn(_("Tried to print header with size smaller than text"));
        return;
    }

    char *header = ic_malloc(size + 1);
    for (i = 0 ; i < size; i++)
        header[i] = sep;
    header[size] = '\0';

    size_t lpad = (size / 2) - (len / 2);
    size_t rpad = (size / 2) + (len / 2);

    if (rpad > size)
        rpad = size;

    if (len > 0) {
        header[lpad - 1] = ' ';

        for (j = 0, i = lpad; i < (len + lpad); i++, j++)
            header[i] = text[j];

        header[i] = ' ';
    }

    ic_printf("%s\n", header);
    ic_free(header);
}
