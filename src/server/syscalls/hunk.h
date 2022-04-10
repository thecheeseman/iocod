#ifndef SERVER_SYSCALLS_HUNK_H
#define SERVER_SYSCALLS_HUNK_H

void *hunk_alloc_internal(int size);
void *hunk_alloc_low_internal(int size);
void *hunk_alloc_align_internal(int size, int align);
void *hunk_alloc_low_align_internal(int size, int align);

#endif /* SERVER_SYSCALLS_HUNK_H */
