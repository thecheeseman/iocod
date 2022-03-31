#ifndef VM_H
#define VM_H

#include "iocod.h"
#include "shared.h"
#include <stdint.h>

struct vm {
    intptr_t(*system_call)(intptr_t *parms);

    char name[MAX_QPATH];
    char full_qpath[MAX_QPATH + 1];

    char data[3];

    void *dll_handle;
    intptr_t(*entry_point)(intptr_t callnum, ...);
};

void vm_init(void);
struct vm *vm_create(const char *module, intptr_t(*systemcalls)(intptr_t *));
void vm_free(struct vm *vm);
void vm_clear(void);
intptr_t DECL vm_call(struct vm *vm, intptr_t callnum, ...);

#endif /* VM_H */
