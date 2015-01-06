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

// -----------------------------------------------------------------------
struct crk5_cfg_mem {
	int silicon;
	int mega;
	unsigned blocks;
	unsigned block_start;
	unsigned module_start;
};

struct crk5_cfg_mem * crk5_cfg_mem_unpack(uint16_t *d);
int crk5_cfg_mem_pack(struct crk5_cfg_mem *mem, uint16_t *d);
void crk5_cfg_mem_delete(struct crk5_cfg_mem *mem);

// -----------------------------------------------------------------------
struct crk5_cfg_disk {
	int own;
	unsigned type;		// different disk types use fields differently:
						// 9425		flop8	flop5	winch	plix
						// ----------------------------------------
	unsigned addr_maj;	// chan		unit	--		--		line
	unsigned addr_min;	// unit		door	num		num		num
	int fixed;			// fixed?	--		--		--		--
	unsigned media;		// --		--		type	type	type
	unsigned quant;		// --		--		--		quant	--
};

// -----------------------------------------------------------------------
struct crk5_cfg_chdev {
	unsigned dir;
	int used;
	unsigned type;
	unsigned protocol;
	unsigned count;
};

// -----------------------------------------------------------------------
struct crk5_cfg_tape {
	unsigned unit;
	unsigned chan;
};

#define CRK5_CFG_SIZE 60

// -----------------------------------------------------------------------
struct crk5_cfg {
	// 4
	int sem;
	int dw3;
	int lod_unl;
	int fast_clock;
	int no_swap;
	int buf_4k;
	int all_resident;
	int disk_wait;
	unsigned io_buf_num;

	// 5
	int multix_auto;
	int multix_no_parity;
	int multix_parity_odd;
	int multix_8bit;
	int multix_xon_xoff;
	int multix_bs_can;
	int multix_caps;
	int multix_streams;

	// 6
	int auto_ramdisk;
	int nofs_block;
	int dir_write_check;
	int user_vectors;

	// 7
	int os_segments;
	int user_buf_len;

	// 8..f
	struct crk5_cfg_mem *mem[8];

	// 10..1f
	struct crk5_cfg_disk_disk *disk[16];

	// 20
	unsigned last_tape_unit;

	// 20-23
	struct crk5_cfg_tape *tape[4];

	// 24-27
	unsigned chan_camac1;
	unsigned chan_camac2;
	unsigned chan_pi;
	unsigned chan_iec;

	// 28-2b
	unsigned chan_plix;
	unsigned line_winchester;
	unsigned line_flop;
	unsigned chan_multix;
	unsigned chan_char1;
	unsigned chan_char2;

	// 2d
	unsigned chan_net;
	unsigned dev_net;

	// 2e
	unsigned chan_rtc;
	unsigned dev_rtc;

	// 2f
	unsigned mon_group;

	// 30
	int oprq_line;

	// 31-3f
	struct crk5_cfg_chdev *chdev[15];
};

// vim: tabstop=4 shiftwidth=4 autoindent
