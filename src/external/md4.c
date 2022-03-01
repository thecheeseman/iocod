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
 * @file md4.c
 * @date 2022-02-04
*/

#include <string.h>
#include <stdint.h>

#ifdef HAVE_OPENSSL
#include <openssl/md4.h>
#elif !defined(_MD4_H)
#define _MD4_H

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD4_u32plus;

typedef struct {
	MD4_u32plus lo, hi;
	MD4_u32plus a, b, c, d;
	unsigned char buffer[64];
	MD4_u32plus block[16];
} MD4_CTX;

/*
 * The basic MD4 functions.
 *
 * F and G are optimized compared to their RFC 1320 definitions, with the
 * optimization for F borrowed from Colin Plumb's MD5 implementation.
 */
#define F(x, y, z)			((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)			(((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z)			((x) ^ (y) ^ (z))

 /*
  * The MD4 transformation for all three rounds.
  */
#define STEP(f, a, b, c, d, x, s) \
	(a) += f((b), (c), (d)) + (x); \
	(a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));

  /*
   * SET reads 4 input bytes in little-endian byte order and stores them in a
   * properly aligned word in host byte order.
   *
   * The check for little-endian architectures that tolerate unaligned memory
   * accesses is just an optimization.  Nothing will break if it fails to detect
   * a suitable architecture.
   *
   * Unfortunately, this optimization may be a C strict aliasing rules violation
   * if the caller's data buffer has effective type that cannot be aliased by
   * MD4_u32plus.  In practice, this problem may occur if these MD4 routines are
   * inlined into a calling function, or with future and dangerously advanced
   * link-time optimizations.  For the time being, keeping these MD4 routines in
   * their own translation unit avoids the problem.
   */
#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#define SET(n) \
	(*(MD4_u32plus *)&ptr[(n) * 4])
#define GET(n) \
	SET(n)
#else
#define SET(n) \
	(ctx->block[(n)] = \
	(MD4_u32plus)ptr[(n) * 4] | \
	((MD4_u32plus)ptr[(n) * 4 + 1] << 8) | \
	((MD4_u32plus)ptr[(n) * 4 + 2] << 16) | \
	((MD4_u32plus)ptr[(n) * 4 + 3] << 24))
#define GET(n) \
	(ctx->block[(n)])
#endif

   /*
	* This processes one or more 64-byte data blocks, but does NOT update the bit
	* counters.  There are no alignment requirements.
	*/
static const void *body(MD4_CTX *ctx, const void *data, unsigned long size)
{
	const unsigned char *ptr;
	MD4_u32plus a, b, c, d;
	MD4_u32plus saved_a, saved_b, saved_c, saved_d;
	const MD4_u32plus ac1 = 0x5a827999, ac2 = 0x6ed9eba1;

	ptr = (const unsigned char *) data;

	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;

	do {
		saved_a = a;
		saved_b = b;
		saved_c = c;
		saved_d = d;

		/* Round 1 */
		STEP(F, a, b, c, d, SET(0), 3)
			STEP(F, d, a, b, c, SET(1), 7)
			STEP(F, c, d, a, b, SET(2), 11)
			STEP(F, b, c, d, a, SET(3), 19)
			STEP(F, a, b, c, d, SET(4), 3)
			STEP(F, d, a, b, c, SET(5), 7)
			STEP(F, c, d, a, b, SET(6), 11)
			STEP(F, b, c, d, a, SET(7), 19)
			STEP(F, a, b, c, d, SET(8), 3)
			STEP(F, d, a, b, c, SET(9), 7)
			STEP(F, c, d, a, b, SET(10), 11)
			STEP(F, b, c, d, a, SET(11), 19)
			STEP(F, a, b, c, d, SET(12), 3)
			STEP(F, d, a, b, c, SET(13), 7)
			STEP(F, c, d, a, b, SET(14), 11)
			STEP(F, b, c, d, a, SET(15), 19)

			/* Round 2 */
			STEP(G, a, b, c, d, GET(0) + ac1, 3)
			STEP(G, d, a, b, c, GET(4) + ac1, 5)
			STEP(G, c, d, a, b, GET(8) + ac1, 9)
			STEP(G, b, c, d, a, GET(12) + ac1, 13)
			STEP(G, a, b, c, d, GET(1) + ac1, 3)
			STEP(G, d, a, b, c, GET(5) + ac1, 5)
			STEP(G, c, d, a, b, GET(9) + ac1, 9)
			STEP(G, b, c, d, a, GET(13) + ac1, 13)
			STEP(G, a, b, c, d, GET(2) + ac1, 3)
			STEP(G, d, a, b, c, GET(6) + ac1, 5)
			STEP(G, c, d, a, b, GET(10) + ac1, 9)
			STEP(G, b, c, d, a, GET(14) + ac1, 13)
			STEP(G, a, b, c, d, GET(3) + ac1, 3)
			STEP(G, d, a, b, c, GET(7) + ac1, 5)
			STEP(G, c, d, a, b, GET(11) + ac1, 9)
			STEP(G, b, c, d, a, GET(15) + ac1, 13)

			/* Round 3 */
			STEP(H, a, b, c, d, GET(0) + ac2, 3)
			STEP(H, d, a, b, c, GET(8) + ac2, 9)
			STEP(H, c, d, a, b, GET(4) + ac2, 11)
			STEP(H, b, c, d, a, GET(12) + ac2, 15)
			STEP(H, a, b, c, d, GET(2) + ac2, 3)
			STEP(H, d, a, b, c, GET(10) + ac2, 9)
			STEP(H, c, d, a, b, GET(6) + ac2, 11)
			STEP(H, b, c, d, a, GET(14) + ac2, 15)
			STEP(H, a, b, c, d, GET(1) + ac2, 3)
			STEP(H, d, a, b, c, GET(9) + ac2, 9)
			STEP(H, c, d, a, b, GET(5) + ac2, 11)
			STEP(H, b, c, d, a, GET(13) + ac2, 15)
			STEP(H, a, b, c, d, GET(3) + ac2, 3)
			STEP(H, d, a, b, c, GET(11) + ac2, 9)
			STEP(H, c, d, a, b, GET(7) + ac2, 11)
			STEP(H, b, c, d, a, GET(15) + ac2, 15)

			a += saved_a;
		b += saved_b;
		c += saved_c;
		d += saved_d;

		ptr += 64;
	} while (size -= 64);

	ctx->a = a;
	ctx->b = b;
	ctx->c = c;
	ctx->d = d;

	return ptr;
}

void MD4_Init(MD4_CTX *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;

	ctx->lo = 0;
	ctx->hi = 0;
}

void MD4_Update(MD4_CTX *ctx, const void *data, unsigned long size)
{
	MD4_u32plus saved_lo;
	unsigned long used, available;

	saved_lo = ctx->lo;
	if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
		ctx->hi++;
	ctx->hi += size >> 29;

	used = saved_lo & 0x3f;

	if (used) {
		available = 64 - used;

		if (size < available) {
			memcpy(&ctx->buffer[used], data, size);
			return;
		}

		memcpy(&ctx->buffer[used], data, available);
		data = (const unsigned char *) data + available;
		size -= available;
		body(ctx, ctx->buffer, 64);
	}

	if (size >= 64) {
		data = body(ctx, data, size & ~(unsigned long) 0x3f);
		size &= 0x3f;
	}

	memcpy(ctx->buffer, data, size);
}

#define OUT(dst, src) \
	(dst)[0] = (unsigned char)(src); \
	(dst)[1] = (unsigned char)((src) >> 8); \
	(dst)[2] = (unsigned char)((src) >> 16); \
	(dst)[3] = (unsigned char)((src) >> 24);

void MD4_Final(unsigned char *result, MD4_CTX *ctx)
{
	unsigned long used, available;

	used = ctx->lo & 0x3f;

	ctx->buffer[used++] = 0x80;

	available = 64 - used;

	if (available < 8) {
		memset(&ctx->buffer[used], 0, available);
		body(ctx, ctx->buffer, 64);
		used = 0;
		available = 64;
	}

	memset(&ctx->buffer[used], 0, available - 8);

	ctx->lo <<= 3;
	OUT(&ctx->buffer[56], ctx->lo)
		OUT(&ctx->buffer[60], ctx->hi)

		body(ctx, ctx->buffer, 64);

	OUT(&result[0], ctx->a)
		OUT(&result[4], ctx->b)
		OUT(&result[8], ctx->c)
		OUT(&result[12], ctx->d)

		memset(ctx, 0, sizeof(*ctx));
}

#endif

//===================================================================

unsigned com_block_checksum(const void *buffer, int length)
{
	int digest[4];
	unsigned val;
	MD4_CTX ctx;

	MD4_Init(&ctx);
	MD4_Update(&ctx, (unsigned char *) buffer, length);
	MD4_Final((unsigned char *) digest, &ctx);

	val = digest[0] ^ digest[1] ^ digest[2] ^ digest[3];
	return val;
}

unsigned com_block_checksum_key(void *buffer, int length, int key)
{
	int digest[4];
	unsigned val;
	MD4_CTX ctx;

	MD4_Init(&ctx);
	MD4_Update(&ctx, (unsigned char *) &key, 4);
	MD4_Update(&ctx, (unsigned char *) buffer, length);
	MD4_Final((unsigned char *) digest, &ctx);

	val = digest[0] ^ digest[1] ^ digest[2] ^ digest[3];

	return val;
}
