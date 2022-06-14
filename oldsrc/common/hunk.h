#ifndef COMMON_HUNK_H
#define COMMON_HUNK_H

void com_init_zone_memory(void);
void hunk_clear(void);
void hunk_clear2(void);
void hunk_clear_to_mark_low(void);
void com_init_hunk_memory(void);
void com_hunk_shutdown(void);
void *hunk_alloc_align(int size, int align);
void *hunk_alloc(int size);
void *hunk_alloc_low_align(int size, int align);
void *hunk_alloc_low(int size);
void *hunk_allocate_temp_memory(int size);
void *hunk_allocate_temp_memory_high(int size);
void hunk_free_temp_memory(void *buf);
void hunk_swap_temp(void);
void hunk_swap_temp_low(void);
void hunk_set_mark_low(void);

#endif /* COMMON_HUNK_H */
