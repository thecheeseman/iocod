#include "con_local.h"

struct field hist_field[MAX_HISTORY];
int hist_current = -1;
int hist_count = 0;

void history_add(struct field *field)
{
    if (field->cursor == 0)
        return;

    IC_ASSERT(hist_count <= MAX_HISTORY);
    IC_ASSERT(hist_count >= 0);
    IC_ASSERT(hist_current >= -1);
    IC_ASSERT(hist_current <= hist_count);

    for (int i = MAX_HISTORY - 1; i > 0; i--)
        hist_field[i] = hist_field[i - 1];

    hist_field[0] = *field; // insert

    if (hist_count < MAX_HISTORY)
        hist_count++;

    hist_current = -1; // re-init
}

struct field *history_previous(void)
{
    IC_ASSERT(hist_count <= MAX_HISTORY);
    IC_ASSERT(hist_count >= 0);
    IC_ASSERT(hist_current >= -1);
    IC_ASSERT(hist_current <= hist_count);

    int hist_prev = hist_current + 1;
    if (hist_prev >= hist_count)
        return NULL;

    hist_current++;
    return &hist_field[hist_current];
}

struct field *history_next(void)
{
    IC_ASSERT(hist_count <= MAX_HISTORY);
    IC_ASSERT(hist_count >= 0);
    IC_ASSERT(hist_current >= -1);
    IC_ASSERT(hist_current <= hist_count);

    if (hist_current >= 0)
        hist_current--;

    if (hist_current == -1)
        return NULL;

    return &hist_field[hist_current];
}
