#include "common/error.h"
#include "common/hunk.h"
#include "server/server_local.h"
#include "server/syscalls/hunk.h"
#include "va.h"

#define HUNKERRMSG "can only be called in g_init_game and the first few " \
                   "frames of g_run_frame"
#define hunk_error() com_error(ERR_DROP, va("\x15%s %s", __func__, HUNKERRMSG));

void *hunk_alloc_internal(int size)
{
    if (sv.state != SS_LOADING)
        hunk_error();

    return hunk_alloc(size);
}

void *hunk_alloc_low_internal(int size)
{
    if (sv.state != SS_LOADING)
        hunk_error();

    return hunk_alloc_low(size);
}

void *hunk_alloc_align_internal(int size, int align)
{
    if (sv.state != SS_LOADING)
        hunk_error();

    return hunk_alloc_align(size, align);
}

void *hunk_alloc_low_align_internal(int size, int align)
{
    if (sv.state != SS_LOADING)
        hunk_error();

    return hunk_alloc_low_align(size, align);
}
