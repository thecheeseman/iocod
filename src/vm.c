/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

/**
 * @file vm.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

#include "common/error.h"

#define MAX_VM 3
struct vm vmtable[MAX_VM];

struct vm *currentvm = NULL;
struct vm *lastvm = NULL;

void vm_init(void)
{
	memset(vmtable, 0, sizeof(vmtable));
}

intptr_t QDECL vm_dll_syscall(intptr_t arg, ...)
{
	intptr_t args[16];
	size_t i;
	va_list ap;
	args[0] = arg;

	va_start(ap, arg);
	for (i = 1; i < sizeof(args) / sizeof(args[i]); i++)
		args[i] = va_arg(ap, intptr_t);
	va_end(ap);

	return currentvm->system_call(args);
	//return currentvm->system_call(&arg);
}

struct vm *vm_create(const char *module, intptr_t (*system_calls)(intptr_t *))
{
	struct vm *vm;
	int i;

	if (module == NULL || *module == '\0' || system_calls == NULL)
		com_error(ERR_FATAL, "VM_Create: bad parms");

	// this call is in the actual code but it does nothing
	//remaining = hunk_memory_remaining();

	// see if we already have the vm
	for (i = 0; i < MAX_VM; i++) {
		if (!q_stricmp(vmtable[i].name, module)) {
			vm = &vmtable[i];
			return vm;
		}
	}

	// find a free vm
	for (i = 0; i < MAX_VM; i++) {
		if (*vmtable[i].name == '\0')
			break;
	}

	if (i == MAX_VM)
		com_error(ERR_FATAL, "VM_Create: no free vms");

	vm = &vmtable[i];

	q_strncpyz(vm->name, module, sizeof(vm->name));
	vm->system_call = system_calls;
	vm->dll_handle = sys_load_dll(module, vm->full_qpath, &vm->entry_point, 
								  vm_dll_syscall);

	if (vm->dll_handle == NULL)
		com_error(ERR_FATAL, "Failed to load shared library");

	return vm;
}

void vm_free(struct vm *vm)
{
	sys_unload_dll(vm->dll_handle);
	memset(vm, 0, sizeof(*vm));
	currentvm = NULL;
}

void vm_clear(void)
{
	int i;
	
	for (i = 0; i < MAX_VM; i++) {
		if (vmtable[i].dll_handle)
			sys_unload_dll(vmtable[i].dll_handle);

		memset(&vmtable[i], 0, sizeof(struct vm));
	}

	currentvm = NULL;
	lastvm = NULL;
}

intptr_t QDECL vm_call(struct vm *vm, intptr_t callnum, ...)
{
	int r;
	struct vm *oldvm;

	oldvm = currentvm;

	currentvm = vm;
	r = vm->entry_point((&callnum)[0], 
		(&callnum)[1], (&callnum)[2],  (&callnum)[3],  (&callnum)[4], 
		(&callnum)[5], (&callnum)[6],  (&callnum)[7],  (&callnum)[8], 
		(&callnum)[9], (&callnum)[10], (&callnum)[11], (&callnum)[12]);

	currentvm = oldvm;
	return r;
}
