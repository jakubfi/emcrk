//  Copyright (c) 2014 Jakub Filipowicz <jakubf@gmail.com>
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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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


// -----------------------------------------------------------------------
static int pat_search(uint16_t *buf, off_t len, const int *pattern)
{
	uint16_t *pos = buf;
	uint16_t *spos = NULL;

	int *cpat = (int *) pattern;

	while (pos < buf+len) {
		// match
		if ((*pos == *cpat) || (*cpat == PAT_ANY)) {
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
				cpat = (int *) pattern;
			} else {
				pos++;
			}
		}
	}

	return -1;
}

// -----------------------------------------------------------------------
// need char* because we cannot assume that input is word-aligned
struct crk5_kern_result * crk5_kern_find(uint16_t *buf, off_t len)
{
	struct crk5_kern_result *kern = malloc(sizeof (struct crk5_kern_result));
	kern->next = NULL;

	// find kernel image start offset (words into the buffer)
	kern->offset = pat_search(buf, len, kern_pat);
	if (kern->offset < 0) {
		goto cleanup;
	}

	uint16_t *kstart = buf + kern->offset;
	off_t klen = len - kern->offset;

	// get kernel version
	off_t copsys_pos = pat_search(kstart, klen, copsys_pat);
	if (copsys_pos >= 0) {
		off_t sysnum_offset = kstart[copsys_pos-1] & 0b111111;
		uint16_t kversion = kstart[copsys_pos-sysnum_offset];
		kern->vmaj = (kversion & 0b0000000001111111);
		kern->vmin = (kversion & 0b1111111110000000) >> 7;
	} else {
		kern->vmaj = 0;
		kern->vmin = 0;
	}

	// check if this kernel is for modified cpu
	uint16_t int5_vec = kstart[0x40+5];
	uint16_t int11_vec = kstart[0x40+11];
	if (int5_vec == int11_vec) {
		kern->mod = 0;
	} else {
		kern->mod = 1;
	}

	// current entry point
	kern->entry_point = kstart[1];

	// instruction that jump leads to
	uint16_t jmp_content = kstart[kern->entry_point];

	// is initial jump to either START or COPSY0?
	if ((kern->entry_point < start_addr_lim) && (jmp_content == start_pat)) {
		kern->raw = 0;
	} else if ((kern->entry_point > copsy0_addr_lim_low) && (jmp_content == copsy0_pat)) {
		kern->raw = 1;
	} else {
		goto cleanup;
	}

	// find CHCS checksum function offset
	off_t chcs_pos = pat_search(kstart, klen, chcs_pat);
	if ((chcs_pos < 0) || (chcs_pos > chcs_addr_lim)) {
		goto cleanup;
	}

	// checksum address
	kern->cksum_addr = chcs_pos - 1;

	// stored checksum
	kern->cksum_stored = kstart[kern->cksum_addr];

	// calculate checksum
	kern->cksum_computed = 0;
	for (int i=kern->offset+chcs_pos ; i<kern->offset + AEOV0 ; i++) {
		kern->cksum_computed -= buf[i];
	}

	// if kernel is raw and buffer is long enough - find real entry address
	if (kern->raw && (klen > copsy0_addr_lim_low)) {
		// find START address update code in COPSY0 code
		off_t copsy0_upd_pos = pat_search(kstart+copsy0_addr_lim_low, klen-copsy0_addr_lim_low, copsy0_upd_pat);
		if (copsy0_upd_pos < 0) {
			goto cleanup;
		}

		// START address as found in COPSY0
		kern->start_addr = kstart[copsy0_upd_pos+1];
	} else {
		kern->start_addr = 0;
	}

	return kern;

cleanup:
	free(kern);
	return NULL;
}

// -----------------------------------------------------------------------
struct crk5_kern_result * crk5_kern_findall(uint16_t *buf, off_t len)
{
	off_t offset = 0;
	struct crk5_kern_result *tkern;
	struct crk5_kern_result *kern = NULL;
	struct crk5_kern_result *lkern;

	do {
		tkern = crk5_kern_find(buf+offset, len-offset);
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
