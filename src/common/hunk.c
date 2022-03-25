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

#include "shared.h"
#include "common.h"

#include "common/error.h"
#include "common/hunk.h"
#include "common/memory.h"
#include "common/print.h"

#define HUNK_MAGIC  0x89537892
#define HUNK_FREE_MAGIC 0x89537893

struct hunkheader {
	int magic;
	int size;
};

struct hunkused {
	int mark;
	int temphighwater;
	int permanent;
	int temp;
};

static struct hunkused hunk_low, hunk_high;

#define MIN_DEDICATED_COM_HUNKMEGS 1
#define MIN_COM_HUNKMEGS 80

static byte *s_hunkdata = NULL;
static int s_hunktotal;

static int s_hunkalloced; // not sure

#define BYTESTOMB(val) ((float)((float)val / 1048576))

// does nothing
void com_init_zone_memory(void)
{

}

/**
 * @brief meminfo mostly based on cod2
 * @param  
*/
void com_meminfo_f(void)
{
	com_printf("%8.4f MB total hunk\n", BYTESTOMB(s_hunktotal));
	// removed zone

	com_printf("%8.4f MB low permanent\n", BYTESTOMB(hunk_low.permanent));

	if (hunk_low.temp != hunk_low.permanent)
		com_printf("%8.4f MB low temp\n", BYTESTOMB(hunk_low.temp));

	com_printf("%8.4f MB high permanent\n", BYTESTOMB(hunk_high.permanent));

	if (hunk_high.temp != hunk_high.permanent)
		com_printf("%8.4f MB high temp\n", BYTESTOMB(hunk_high.temp));

	com_printf("%8.4f MB total hunk in use\n", 
			   BYTESTOMB(hunk_low.permanent + hunk_high.permanent));
}

void INCOMPLETE fun_080c27ec(void *a, void *b)
{
	(void) a; (void) b; //-Werror=unusued-parameter
}

void INCOMPLETE fun_08061888(void *a, void *b)
{
	(void) a; (void) b; //-Werror=unusued-parameter
}

void INCOMPLETE fun_08061980(void)
{

}

/**
 * @brief 
 * @param  
*/
void hunk_clear(void)
{
	void *a, *b;

	hunk_low.mark = 0;
	hunk_low.temphighwater = 0;
	hunk_low.permanent = 0;
	hunk_low.temp = 0;
	hunk_high.mark = 0;
	hunk_high.temphighwater = 0;
	hunk_high.permanent = 0;
	hunk_high.temp = 0;

	s_hunkalloced = 0;

	com_printf("hunk_clear: reset the hunk OK\n");

	a = s_hunkdata + hunk_low.permanent;
	b = (s_hunkdata + s_hunktotal) - hunk_high.permanent;
	fun_080c27ec(a, b);
	fun_08061888(a, b);
	fun_08061980();
}

// there are two versions of this function
// one with sv_shutdown_game_progs() / vm_clear()
// and one without
void hunk_clear2(void)
{
	sv_shutdown_game_progs();

	hunk_clear();

	vm_clear();
}

void hunk_clear_to_mark_low(void)
{
	void *a, *b;

	hunk_low.temp = hunk_low.mark;
	hunk_low.permanent = hunk_low.mark;
	a = s_hunkdata + hunk_low.permanent;
	b = (s_hunkdata + s_hunktotal) - hunk_high.permanent;
	fun_080c27ec(a, b);
	fun_08061888(a, b);
}

/**
 * @brief Initialize hunk-based memory
 * @param
*/
void com_init_hunk_memory(void)
{
	struct cvar *cv;
	int min;
	char *msg = NULL;

	if (s_hunkdata != NULL) {
		com_printf("Hunk memory already initialized, not reallocating\n");
		return;
	}

	if (fs_loadstack() != 0)
		com_error(ERR_FATAL, "file system loadstack not zero");

	cv = cvar_get("com_hunkmegs", "128", CVAR_LATCH | CVAR_ARCHIVE);

	if (com_dedicated != NULL && com_dedicated->integer > 0) {
		min = MIN_DEDICATED_COM_HUNKMEGS;
		msg = "Minimum com_hunkmegs for a dedicated server is %i, allocating %i.\n";
	} else {
		min = MIN_COM_HUNKMEGS;
		msg = "Minimum com_hunkmegs is %i, allocating %i.\n";
	}

	if (cv->integer < min) {
		s_hunktotal = 1024 * 1024 * min;
		com_printf(msg, min, s_hunktotal / (1024 * 1024));
	} else {
		s_hunktotal = cv->integer * 1024 * 1024;
	}

	// cod2/ioquake3/rtcw use calloc(1, s_hunktotal);
	// cod1/q3 use malloc(s_hunktotal + 31);
	// probably safer to just use calloc?
	s_hunkdata = z_malloc(s_hunktotal);
	if (s_hunkdata == NULL) {
		com_error(ERR_FATAL, "Hunk data failed to allocate %.4f MB",
				  BYTESTOMB(s_hunktotal));
	}

	com_printf("hunk allocated %.4f MB\n", BYTESTOMB(s_hunktotal));

	// not sure what this is used for yet?
	// hunk_ptr = s_hunkdata;

	// some magic that i think only works on 32 bit systems (not present in cod2+)
	// this causes segfaults when trying to allocate new temp memory
	#if id386
	s_hunkdata = (byte *)(((int) s_hunkdata + 31) & ~31);
	#endif

	hunk_clear();

	cmd_add_command("meminfo", com_meminfo_f);
}

void com_hunk_shutdown(void)
{
	z_free(s_hunkdata);

	cmd_remove_command("meminfo");
}

/**
 * @brief Allocate temp memory on the low hunk
 * @param size
 * @return
*/
void *hunk_allocate_temp_memory(int size)
{
	void *buf;
	struct hunkheader *hdr;
	int low, z;

	low = hunk_low.temp;

	if (s_hunkdata == NULL) {
		buf = z_malloc(size);
		if (buf == NULL)
			com_error(ERR_FATAL, "couldn't allocate %i bytes of memory");

		com_memset(buf, 0, size);
		return buf;
	}

	// this is what cod1 is doing internally
	z = (low + 15) & ~15;
	hdr = (struct hunkheader *) (s_hunkdata + z);
	hunk_low.temp = z + size + 0x10;
	// all low hunk is aligned by 16 bytes

	if (s_hunktotal < hunk_low.temp + hunk_high.temp) {
		com_meminfo_f();

		// cod2 style error message
		com_error(ERR_DROP,
				  "failed on %i bytes " \
				  "(total % .4f MB, low % .4f MB, high % .4f MB), " \
				  "needs % i more hunk bytes", 
				  size + 0x10, 
				  BYTESTOMB(s_hunktotal), 
				  BYTESTOMB(hunk_low.temp), 
				  BYTESTOMB(hunk_high.temp), 
				  (hunk_high.temp + hunk_low.temp) - s_hunktotal);
	}

	buf = hdr + 4;
	hdr->magic = HUNK_MAGIC;
	hdr->size = hunk_low.temp - low;

	return buf;
}

/**
 * @brief Allocate temp memory on the high hunk
 * There's no equivalent to free_temp_memory_high() in the game so this is 
 * never actually cleared?
 * @param size
 * @return
*/
void *hunk_allocate_temp_memory_high(int size)
{
	hunk_high.temp += (size + 15) & ~15;

	if (s_hunktotal < hunk_high.temp + hunk_low.temp) {
		com_meminfo_f();

		// cod2 style error message
		com_error(ERR_DROP,
				  "failed on % i bytes " \
				  "(total % .4f MB, low % .4f MB, high % .4f MB), " \
				  "needs % i more hunk bytes", 
				  size + 0x10, 
				  BYTESTOMB(s_hunktotal), 
				  BYTESTOMB(hunk_low.temp), 
				  BYTESTOMB(hunk_high.temp), 
				  (hunk_high.temp + hunk_low.temp) - s_hunktotal);
	}

	return (s_hunktotal + s_hunkdata) - hunk_high.temp;
}

void *hunk_reallocate_temp_memory(int size)
{
	hunk_low.temp = size + hunk_low.permanent;

	if (s_hunktotal < hunk_high.temp + hunk_low.temp) {
		com_meminfo_f();

		// cod2 style error message
		com_error(ERR_DROP, 
				  "failed on %i bytes " \
				  "(total % .4f MB, low % .4f MB, high % .4f MB), " \
				  "needs % i more hunk bytes", 
				  size + 0x10, 
				  BYTESTOMB(s_hunktotal), 
				  BYTESTOMB(hunk_low.temp), 
				  BYTESTOMB(hunk_high.temp), 
				  (hunk_high.temp + hunk_low.temp) - s_hunktotal);
	}

	return hunk_low.permanent + s_hunkdata;
}

/**
 * @brief
 * @param buf
*/
void hunk_free_temp_memory(void *buf)
{
	struct hunkheader *hdr;

	if (s_hunkdata == NULL) {
		z_free(buf);
		return;
	}

	hdr = ((struct hunkheader *) buf) - 4;
	if (hdr->magic != (int) HUNK_MAGIC)
		com_error(ERR_FATAL, "bad magic");

	hdr->magic = HUNK_FREE_MAGIC;
	hunk_low.temp -= hdr->size;
}

/**
 * @brief 
 * @param size 
 * @param align 
 * @return 
*/
void *hunk_alloc_align(int size, int align)
{
	void *buf;

	if (s_hunkdata == NULL)
		com_error(ERR_FATAL, "hunk memory system not initialized");

	hunk_high.permanent = (size + hunk_high.permanent + (align - 1)) & ~(align - 1);
	buf = (void *) ((s_hunktotal + s_hunkdata) - hunk_high.permanent);
	hunk_high.temp = hunk_high.permanent;

	if (s_hunktotal < hunk_high.permanent + hunk_low.temp) {
		com_meminfo_f();
		com_error(ERR_DROP, "failed on %i", size);
	}

	s_hunkalloced = hunk_high.permanent + hunk_low.permanent;

    com_memset(buf, 0, size);
	return buf;
}

/**
 * @brief Always align to 32
 * @param size 
 * @return 
*/
void *hunk_alloc(int size)
{
	return hunk_alloc_align(size, 32);
}

/**
 * @brief 
 * @param size 
 * @param align 
 * @return 
*/
void *hunk_alloc_low_align(int size, int align)
{
	void *buf;

	if (s_hunkdata == NULL)
		com_error(ERR_FATAL, "hunk memory system not initialized");

	hunk_low.permanent = (size + (align - 1) + hunk_low.permanent) & ~(align - 1);
	buf = (void *) (hunk_low.permanent + s_hunkdata);
	hunk_low.temp = hunk_low.permanent;

	if (s_hunktotal < hunk_low.permanent + hunk_high.temp) {
		com_meminfo_f();
		com_error(ERR_DROP, "failed on %i", size);
	}

	s_hunkalloced = hunk_high.permanent + hunk_low.permanent;

	com_memset(buf, 0, size);
	return buf;
}

/**
 * @brief Always align to 32
 * @param size 
 * @return 
*/
void *hunk_alloc_low(int size)
{
	return hunk_alloc_low_align(size, 32);
}

void hunk_swap_temp(void)
{
	hunk_high.temp = hunk_high.permanent;
}

void hunk_swap_temp_low(void)
{
	if (s_hunkdata != NULL)
		hunk_low.temp = hunk_low.permanent;
}

void hunk_set_mark_low(void)
{
	hunk_low.mark = hunk_low.permanent;
}

/*
void z_memory_dump(const char *name, const void *addr, const int len)
{
	int i;
	unsigned char buf[17];
	char path[64];
	FILE *f;
	const unsigned char *pc = (const unsigned char *) addr;

	if (len == 0 || len < 0)
		return;

	snprintf(path, 63, "%s.dump", name);
	f = fopen(path, "wb");
	if (f) {
		for (i = 0; i < len; i++) {
			if ((i % 16) == 0) {
				if (i != 0) {
					fprintf(f, "  %s\n", buf);
				}

				fprintf(f, "%12x ", addr + i);
			}

			fprintf(f, " %02x", pc[i]);

			if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
				buf[i % 16] = '.';
			} else {
				buf[i % 16] = pc[i];
			}
			buf[(i % 16) + 1] = '\0';
		}

		while ((i % 16) != 0) {
			fprintf(f, "   ");
			i++;
		}

		fprintf(f, "  %s\n", buf);
		fclose(f);
		printf("wrote dump file to %s\n", path);
	}
}
*/
