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

struct crk5_kern_result {
    off_t offset;               // kernel image start (words into buffer)
    int raw;                    // is the kernel raw or installed?
	int mod;					// is the kernel for modified (MX-16) cpu?
    uint16_t entry_point;	    // current entry point (initial jump destination address to either START or COPSY0)
    uint16_t start_addr;        // kernel START address (valid only if kernel is raw)
    uint16_t cksum_addr;        // kernel checksum address
    uint16_t cksum_stored;      // stored checksum value
    uint16_t cksum_computed;    // computed checksum value
    struct crk5_kern_result *next;
};

struct crk5_kern_result * crk5_kern_find(uint16_t *buf, off_t len);
struct crk5_kern_result * crk5_kern_findall(uint16_t *buf, off_t len);
void crk5_kern_res_drop(struct crk5_kern_result *kern);

// vim: tabstop=4 shiftwidth=4 autoindent
