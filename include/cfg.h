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

#ifndef CRK5_CFG_H
#define CRK5_CFG_H

#include <inttypes.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CRK5_CFG_SIZE_WORDS 0x6b
#define CRK5_CFG_SIZE_BYTES (CRK5_CFG_SIZE_WORDS*2)

#define CRK5_CFG_MEM_SLOTS 8
#define CRK5_CFG_DISK_SLOTS 16
#define CRK5_CFG_TAPE_SLOTS 4
#define CRK5_CFG_LINE_SLOTS 15
#define CRK5_CFG_CAMAC_SLOTS 4
#define CRK5_CFG_CHAR_SLOTS 2
#define CRK5_CFG_WINCH_TYPE_SLOTS 4

#define CRK5_CFG_ABSENT -1

// -----------------------------------------------------------------------
struct crk5_cfg_sys {
	bool sem;
	bool dw3;
	bool lod_unl;
	bool tick1x;
	bool no_swap;
	bool buf_4k;
	bool all_resident;
	bool disk_wait;
	unsigned io_buf_count;

	bool auto_ram_files;
	bool dir_guard;
	bool dir_write_check;
	int unused_bits_3_7;
	unsigned dir_vec_count;

	unsigned os_blocks;
	int unused_bits_4_7;
	unsigned line_buf_len;
};

// -----------------------------------------------------------------------
struct crk5_cfg_mx_lines {
	bool auto_multix;
	bool no_parity;
	bool odd_parity;
	bool bits8;
	bool xon_xoff;
	bool bs_can;
	bool uppercase;
	bool unused_bit_8;
	int stream_vectors;
};

// -----------------------------------------------------------------------
struct crk5_cfg_mem {
	bool configured;
	bool init_required;
	bool mega;
	unsigned block_count;
	unsigned block_start;
	unsigned module_start;
};

// -----------------------------------------------------------------------
enum crk5_cfg_disk_types {
	CRK5_CFG_DISK_9425	= 0,
	CRK5_CFG_DISK_WINCH	= 1,
	CRK5_CFG_DISK_FLOP8	= 2,
	CRK5_CFG_DISK_FLOP5	= 3,
	CRK5_CFG_DISK_PLIX	= 4,
};

enum crk5_cfg_flop5_densities {
	CRK5_CFG_FLOP5_SD = 0,
	CRK5_CFG_FLOP5_DD = 1,
	CRK5_CFG_FLOP5_HD = 2,
};

enum crk5_cfg_plix_dev_types {
	CRK5_CFG_PLIX_DEV_EC5061 = 0,
	CRK5_CFG_PLIX_DEV_9425_REMOVABLE = 1,
	CRK5_CFG_PLIX_DEV_9425_FIXED = 2,
	CRK5_CFG_PLIX_DEV_9425_WHOLE = 3,
};

// -----------------------------------------------------------------------
struct crk5_cfg_disk {
	// 10..1f
	bool configured;
	bool foreign;
	int type;

	union {
		struct m9425 {
			int chan;
			int dev;
			bool fixed;
			int unused_bits_3_6;
		} m9425;
		struct winch {
			int start_quant;
			int type;
			int num;
			int unused_bit_3;
		} winch;
		struct flop8 {
			int dev;
			int door;
			int unused_bits_4_7;
		} flop8;
		struct plix {
			int type;
			int controller;
			int dev;
			int unused_bits_3_5;
		} plix;
		struct flop5 {
			int density;
			int dev;
			int unused_bits_4_9;
		} flop5;
	} d;
};

enum crk5_cfg_line_dirs {
	CRK5_CFG_LINE_DIR_INPUT = 2,
	CRK5_CFG_LINE_DIR_OUTPUT = 4,
	CRK5_CFG_LINE_DIR_HDUPLEX = 6,
	CRK5_CFG_LINE_DIR_FDUPLEX = 7,
};

enum crk5_cfg_line_types {
	CRK5_CFG_LINE_TYPE_USART = 0,
	CRK5_CFG_LINE_TYPE_8255 = 1,
	CRK5_CFG_LINE_TYPE_SYNC = 2,
};

enum crk5_cfg_line_protocols {
	CRK5_CFG_LINE_PROTO_PREADER = 0,
	CRK5_CFG_LINE_PROTO_PUNCHER = 1,
	CRK5_CFG_LINE_PROTO_PRINTER = 1, // yes, it's the same
	CRK5_CFG_LINE_PROTO_TERMINAL = 2,
};

// -----------------------------------------------------------------------
struct crk5_cfg_lines {
	bool configured;
	bool multix;
	union {
		struct mx {
			int dir;
			bool used;
			int type;
			int protocol;
			int count;
		} mx;
		struct ch {
			int dev;
			int unused_bits_0_11;
		} ch;
	} d;
};

// -----------------------------------------------------------------------
struct crk5_cfg_tape {
	uint16_t raw;
};

// -----------------------------------------------------------------------
struct crk5_cfg_chandev {
	bool configured;
	int chan;
	int dev;
};

// -----------------------------------------------------------------------
struct crk5_cfg_mongroup {
	bool configured;
	int dev;
};

// -----------------------------------------------------------------------
struct crk5_cfg_oprq {
	bool multix_reset;
	int ini_line;
};

// -----------------------------------------------------------------------
struct crk5_cfg_winch_type {
	int unused_bits_0_3;
	bool big;
	int heads;
	int unused_bits_8_15;
	int park_cyl;
	bool autopark;
};

// -----------------------------------------------------------------------
struct crk5_cfg {
	uint16_t magic[4];
	struct crk5_cfg_sys sys;
	struct crk5_cfg_mx_lines mx;
	struct crk5_cfg_mem mem[CRK5_CFG_MEM_SLOTS];
	struct crk5_cfg_disk disk[CRK5_CFG_DISK_SLOTS];
	struct crk5_cfg_tape tape[CRK5_CFG_TAPE_SLOTS];
	struct crk5_cfg_chandev ch_camac[CRK5_CFG_CAMAC_SLOTS];
	struct crk5_cfg_chandev ch_plix;
	struct crk5_cfg_chandev ch_multix;
	struct crk5_cfg_chandev ch_char[CRK5_CFG_CHAR_SLOTS];
	struct crk5_cfg_chandev rtc;
	struct crk5_cfg_chandev net;
	int winch_line;
	int floppy_line;
	uint16_t unused_word_2c;
	struct crk5_cfg_mongroup mongroup;
	struct crk5_cfg_oprq oprq;
	struct crk5_cfg_lines lines[CRK5_CFG_LINE_SLOTS];
	int winch_quant;
	struct crk5_cfg_winch_type winch_type[CRK5_CFG_WINCH_TYPE_SLOTS];
};

void crk5_cfg_init_empty(struct crk5_cfg *cfg);
bool crk5_cfg_decode(uint16_t *d, struct crk5_cfg *cfg);
bool crk5_cfg_encode(struct crk5_cfg *cfg, uint16_t *d);

const char * crk5_cfg_disk_type_name(int type);
const char * crk5_cfg_disk_floppy_density_name(int type);
const char * crk5_cfg_disk_plix_type_name(int type);
const char * crk5_cfg_line_dir_name(int dir);
const char * crk5_cfg_line_type_name(int type);
const char * crk5_cfg_line_protocol_name(int proto);

#ifdef __cplusplus
}
#endif

#endif
// vim: tabstop=4 shiftwidth=4 autoindent
