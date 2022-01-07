//  Copyright (c) 2014, 2022 Jakub Filipowicz <jakubf@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

#include "kfind.h"

#define PAT_ANY -2
#define PAT_END -1

// ENDSYS @ 0x0003: .word AEOV0
// AEOV0 = 014000-OV0\7-0400 = 014000-1\7-0400 = 014000-256-0400 = 0x1600
#define ENDSYS 3
#define AEOV0 0x1600

// kernel "header" @ 0x0000: UJ ??? .word 0x0000, AEOV0
static const int kern_pat[] = { 0xf000, PAT_ANY, 0x0000, AEOV0, PAT_END };
static const int kern_pat_len = 4;

// COPSY0: LW r4, ???
static const uint16_t copsy0_pat = 0x4100;
static const uint16_t copsy0_addr_lim_low = 0x2000;
// jump address update code in COPSY0 procedure: LW r1, START  RW r1, 1
static const int copsy0_upd_pat[] = { 0x4040, PAT_ANY, 0x5040, 0x0001, PAT_END };

// START: LWT r4, 0
static const uint16_t start_pat = 0xd500;
static const uint16_t start_addr_lim = 0x00a0;

// CHCS code: LWT r2, 0  LW r3, ???  LW r1, [ENDSYS]  SW r3, r1
static const int chcs_pat[] = { 0xd480, 0x40c0, PAT_ANY, 0x4240, ENDSYS, 0x88c1, PAT_END };
static const uint16_t chcs_addr_lim = 0x0070;

// COPSYS code: LA ???, RA r1, LW r1, [???]
static const int copsys_pat[] = { 0xf480, PAT_ANY, 0xf881, 0x4240, PAT_END };

#define S(x) (swap_bytes ? ((x & 0xff) << 8 | (x >> 8)) : x)

// -----------------------------------------------------------------------
static int pat_search(uint16_t *buf, off_t len, const int *pattern, bool swap_bytes)
{
	uint16_t *pos = buf;
	uint16_t *spos = NULL;

	const int *cpat = pattern;
	while (pos < buf+len) {
		// match
		if ((S(*pos) == *cpat) || (*cpat == PAT_ANY)) {
			// first match
			if (!spos) {
				spos = pos;
			}
			cpat++;
			if (*cpat == PAT_END) {
				return spos-buf;
			} else {
				pos++;
			}
		// no match
		} else {
			if (spos) {
				pos = spos+1;
				spos = NULL;
				cpat = pattern;
			} else {
				pos++;
			}
		}
	}

	return -1;
}

// -----------------------------------------------------------------------
// TODO: this should work on char*, as we can't assume that input is word-aligned
static struct crk5_kern_result * _crk5_kern_find(uint16_t *buf, off_t len, bool swap_bytes)
{
	struct crk5_kern_result *kern = malloc(sizeof (struct crk5_kern_result));
	kern->next = NULL;

	// find kernel image start offset (words into the buffer)
	kern->offset = pat_search(buf, len, kern_pat, swap_bytes);
	if (kern->offset < 0) {
		goto cleanup;
	}

	uint16_t *kstart = buf + kern->offset;
	off_t klen = len - kern->offset;

	// get kernel version
	off_t copsys_pos = pat_search(kstart, klen, copsys_pat, swap_bytes);
	if (copsys_pos >= 0) {
		off_t sysnum_offset = S(kstart[copsys_pos-1]) & 0b111111;
		uint16_t kversion = S(kstart[copsys_pos-sysnum_offset]);
		kern->vmaj = (kversion & 0b0000000001111111);
		kern->vmin = (kversion & 0b1111111110000000) >> 7;
	} else {
		kern->vmaj = 0;
		kern->vmin = 0;
	}

	// check if this kernel is for modified cpu
	uint16_t int5_vec = S(kstart[0x40+5]);
	uint16_t int11_vec = S(kstart[0x40+11]);
	if (int5_vec == int11_vec) {
		kern->mod = 0;
	} else {
		kern->mod = 1;
	}

	// current entry point
	kern->entry_point = S(kstart[1]);

	// instruction that jump leads to
	uint16_t jmp_content = S(kstart[kern->entry_point]);

	// is initial jump to either START or COPSY0?
	if ((kern->entry_point < start_addr_lim) && (jmp_content == start_pat)) {
		kern->raw = 0;
	} else if ((kern->entry_point > copsy0_addr_lim_low) && (jmp_content == copsy0_pat)) {
		kern->raw = 1;
	} else {
		goto cleanup;
	}

	// find CHCS checksum function offset
	off_t chcs_pos = pat_search(kstart, klen, chcs_pat, swap_bytes);
	if ((chcs_pos < 0) || (chcs_pos > chcs_addr_lim)) {
		goto cleanup;
	}

	// checksum address
	kern->cksum_addr = chcs_pos - 1;

	// stored checksum
	kern->cksum_stored = S(kstart[kern->cksum_addr]);

	// calculate checksum
	kern->cksum_computed = 0;
	for (int i=kern->offset+chcs_pos ; i<kern->offset + AEOV0 ; i++) {
		kern->cksum_computed -= S(buf[i]);
	}

	// if kernel is raw and buffer is long enough - find real entry address
	if (kern->raw && (klen > copsy0_addr_lim_low)) {
		// find START address update code in COPSY0 code
		off_t copsy0_upd_pos = pat_search(kstart+copsy0_addr_lim_low, klen-copsy0_addr_lim_low, copsy0_upd_pat, swap_bytes);
		if (copsy0_upd_pos < 0) {
			goto cleanup;
		}

		// START address as found in COPSY0
		kern->start_addr = S(kstart[copsy0_upd_pos+1]);
	} else {
		kern->start_addr = 0;
	}

	return kern;

cleanup:
	free(kern);
	return NULL;
}

// -----------------------------------------------------------------------
struct crk5_kern_result * crk5_kern_find(uint16_t *buf, off_t len)
{
	return _crk5_kern_find(buf, len, false);
}

// -----------------------------------------------------------------------
static struct crk5_kern_result * _crk5_kern_findall(uint16_t *buf, off_t len, bool swap_bytes)
{
	off_t offset = 0;
	struct crk5_kern_result *tkern;
	struct crk5_kern_result *kern = NULL;
	struct crk5_kern_result *lkern;

	do {
		tkern = _crk5_kern_find(buf+offset, len-offset, swap_bytes);
		if (tkern) {
			offset += tkern->offset + kern_pat_len;
			if (!kern) {
				kern = tkern;
			} else {
				lkern->next = tkern;
			}
			lkern = tkern;
		}
	} while (tkern && (offset < len));

	return kern;
}

// -----------------------------------------------------------------------
struct crk5_kern_result * crk5_kern_findall(uint16_t *buf, off_t len)
{
	return _crk5_kern_findall(buf, len, false);
}

// -----------------------------------------------------------------------
struct crk5_kern_result * crk5_kern_findall_file(FILE *f, bool swap_bytes)
{
	if (!f) return NULL;

	struct stat sb;
	int fd = fileno(f);
	if (fstat(fd, &sb) == -1) return NULL;
	uint16_t *buf = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) return NULL;

	struct crk5_kern_result *ret = _crk5_kern_findall(buf, sb.st_size/2, swap_bytes);

	munmap(buf, sb.st_size);
	return ret;
}

// -----------------------------------------------------------------------
void crk5_kern_res_drop(struct crk5_kern_result *kern)
{
	struct crk5_kern_result *next;
	while (kern) {
		next = kern->next;
		free(kern);
		kern = next;
	}
}

// vim: tabstop=4 shiftwidth=4 autoindent
