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
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "cfg.h"

#define _INVALID_ "(invalid)"

// -----------------------------------------------------------------------
bool crk5_cfg_sys4_decode(uint16_t d, struct crk5_cfg_sys *sys)
{
	sys->sem = d & (1<<15);
	sys->dw3 = d & (1<<14);
	sys->lod_unl = d & (1<<13);
	sys->tick1x = d & (1<<12);
	sys->no_swap = d & (1<<11);
	sys->buf_4k = d & (1<<10);
	sys->all_resident = d & (1<<9);
	sys->disk_wait = d & (1<<8);
	sys->io_buf_count = d & 0xff;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_sys6_decode(uint16_t d, struct crk5_cfg_sys *sys)
{
	sys->auto_ram_files = d & (1<<15);
	sys->dir_guard = d & (1<<14);
	sys->dir_write_check = d & (1<<13);
	sys->unused_bits_3_7 = (d >> 8) & 0b11111;
	sys->dir_vec_count = d & 0xff;

	return true;
}
// -----------------------------------------------------------------------
bool crk5_cfg_sys7_decode(uint16_t d, struct crk5_cfg_sys *sys)
{
	sys->os_blocks = d >> 12;
	sys->unused_bits_4_7 = (d >> 8) & 0b1111;
	sys->line_buf_len = d & 0xff;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_sys4_encode(struct crk5_cfg_sys *sys, uint16_t *d)
{
	if (!sys || ! d) return false;
	if (sys->io_buf_count > 255) return false;

	*d = sys->sem << 15
		| sys->dw3 << 14
		| sys->lod_unl << 13
		| sys->tick1x << 12
		| sys->no_swap << 11
		| sys->buf_4k << 10
		| sys->all_resident << 9
		| sys->disk_wait << 8
		| sys->io_buf_count;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_sys6_encode(struct crk5_cfg_sys *sys, uint16_t *d)
{
	if (!sys || ! d) return false;
	if (sys->dir_vec_count > 255) return false;

	*d = sys->auto_ram_files << 15
		| sys->dir_guard << 14
		| sys->dir_write_check << 13
		| (sys->unused_bits_3_7 & 0b11111) << 8
		| sys->dir_vec_count;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_sys7_encode(struct crk5_cfg_sys *sys, uint16_t *d)
{
	if (!sys || ! d) return false;
	if ((sys->os_blocks > 15) || (sys->line_buf_len > 255)) return false;

	*d = sys->os_blocks << 12
		| (sys->unused_bits_4_7 & 0b1111) << 8
		| sys->line_buf_len;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_mx_decode(uint16_t d, struct crk5_cfg_mx_lines *mx)
{
	// 0x05 - default multix line params
	mx->auto_multix = d & (1<<15);
	mx->no_parity = d & (1<<14);
	mx->odd_parity = d & (1<<13);
	mx->bits8 = d & (1<<12);
	mx->xon_xoff = d & (1<<11);
	mx->bs_can = d & (1<<10);
	mx->uppercase = d & (1<<9);
	mx->unused_bit_8 = d & (1<<8);
	mx->stream_vectors = d & 0xff;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_mx_encode(struct crk5_cfg_mx_lines *mx, uint16_t *d)
{
	if (!mx || !d) return false;
	if (mx->stream_vectors > 255) return false;

	*d = mx->auto_multix << 15
		| mx->no_parity << 14
		| mx->odd_parity << 13
		| mx->bits8 << 12
		| mx->xon_xoff << 11
		| mx->bs_can << 10
		| mx->uppercase << 9
		| mx->unused_bit_8 << 8
		| mx->stream_vectors;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_mem_decode(uint16_t d, struct crk5_cfg_mem *mem)
{
	if (d == 0xffff) {
		mem->configured = false;
	} else {
		mem->configured = true;
		mem->init_required = d & (1<<15);
		mem->mega = d & (1<<14);
		mem->block_count = ((d >> 8) & 0b111111) + 1;
		mem->block_start = (d >> 4) & 0xf;
		mem->module_start = d & 0xf;
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_mem_encode(struct crk5_cfg_mem *mem, uint16_t *d)
{
	if (!mem || ! d) return false;

	if (mem->configured) {
		if ((mem->block_count > 64) || (mem->block_start > 15) || (mem->module_start > 15)) return false;
		*d = mem->init_required << 15
			| mem->mega << 14
			| (mem->block_count - 1) << 8
			| mem->block_start << 4
			| mem->module_start;
	} else {
		*d = 0xffff;
	}

	return true;
}

// -----------------------------------------------------------------------
const char * crk5_cfg_disk_type_name(unsigned type)
{
	static const char *names[] = {
		"MERA 9425",
		"Winchester",
		"Floppy 8\"",
		"Floppy 5.25\"",
		"PLIX disk",
		_INVALID_
	};

	if (type > CRK5_CFG_DISK_PLIX) return names[5];
	return names[type];
}

// -----------------------------------------------------------------------
const char * crk5_cfg_disk_floppy_density_name(unsigned type)
{
	static const char *names[] = { "SD", "DD", "HD", _INVALID_ };

	if (type > CRK5_CFG_FLOP5_HD) return names[3];
	return names[type];
}

// -----------------------------------------------------------------------
const char * crk5_cfg_disk_plix_type_name(unsigned type)
{
	static const char *names[] = {
		"EC 5061",
		"MERA 9425 (removable)",
		"MERA 9425 (fixed)",
		"MERA 9425 (whole)",
		_INVALID_
	};

	if (type > CRK5_CFG_PLIX_DEV_9425_WHOLE) return names[4];
	return names[type];
}

// -----------------------------------------------------------------------
bool crk5_cfg_disk_decode(uint16_t d, struct crk5_cfg_disk *disk)
{
	// 0x10..0x1f - disks
	if (d == 0xffff) {
		disk->configured = false;
	} else {
		disk->configured = true;
		disk->foreign = d & (1<<15);
		unsigned tmp_type = (d >> 13) & 0b11;

		switch (tmp_type) {
			case 0: // 9425, mem channel:
				disk->type = CRK5_CFG_DISK_9425;
				disk->d.m9425.chan = (d >> 2) & 0b1111;
				disk->d.m9425.dev = (d >> 6) & 0b111;
				disk->d.m9425.unused_bits_3_6 = (d >> 9) & 0b1111;
				disk->d.m9425.fixed = d & 1;
				break;
			case 1: // winchester
				disk->type = CRK5_CFG_DISK_WINCH;
				disk->d.winch.start_quant = (d >> 6) & 0b111111;
				disk->d.winch.type = (d >>3) & 0b111;
				disk->d.winch.num = d & 0b111;
				disk->d.winch.unused_bit_3 = (d >> 12) & 1;
				break;
			case 2: // floppy
				switch ((d >> 12) & 1) {
					case 0: // floppy 8"
						disk->type = CRK5_CFG_DISK_FLOP8;
						disk->d.flop8.dev = (d >> 3) & 0b11111;
						disk->d.flop8.door = d & 0b111; // 0, 1, 4, 5
						if ((disk->d.flop8.door == 2) || (disk->d.flop8.door == 3) || (disk->d.flop8.door > 5)) return false;
						if (disk->d.flop8.door > 1) disk->d.flop8.door -= 2;
						disk->d.flop8.unused_bits_4_7 = (d >> 8) & 0b1111;
						break;
					case 1: // floppy 5.25 "
						disk->type = CRK5_CFG_DISK_FLOP5;
						disk->d.flop5.density = (d >> 3) & 0b111;
						if (disk->d.flop5.density > CRK5_CFG_FLOP5_HD) return false;
						disk->d.flop5.dev = d & 0b111;
						disk->d.flop5.unused_bits_4_9 = (d >> 6) & 0b111111;
						break;
				}
				break;
			case 3: // 9425 or EC 5061 in PLIX
				disk->type = CRK5_CFG_DISK_PLIX;
				disk->d.plix.type = (d >> 8) & 3;
				if (disk->d.plix.type > 3) return false;
				disk->d.plix.controller = (d >> 3) & 0b11111;
				disk->d.plix.dev = d & 0b111;
				disk->d.plix.unused_bits_3_5 = (d >> 10) & 0b111;
				break;
			default:
				return false;
		}
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_disk_encode(struct crk5_cfg_disk *disk, uint16_t *d)
{
	if (!disk || !d) return false;

	if (!disk->configured) {
		*d = 0xffff;
	} else {
		*d = disk->foreign << 15;
		switch (disk->type) {
			case CRK5_CFG_DISK_9425:
				if ((disk->d.m9425.dev > 0b111) || (disk->d.m9425.chan > 0b1111)) return false;
				*d |= 0 << 13
					| disk->d.m9425.unused_bits_3_6 << 9
					| disk->d.m9425.dev << 6
					| disk->d.m9425.chan << 2
					| disk->d.m9425.fixed;
				break;
			case CRK5_CFG_DISK_WINCH:
				if ((disk->d.winch.start_quant > 0b111111) || (disk->d.winch.type > 4) || (disk->d.winch.num > 0b111)) return false;
				*d |= 1 << 13
					| disk->d.winch.unused_bit_3 << 12
					| disk->d.winch.start_quant << 6
					| disk->d.winch.type << 3
					| disk->d.winch.num;
				break;
			case CRK5_CFG_DISK_FLOP8:
				if ((disk->d.flop8.dev > 0b11111) || (disk->d.flop8.door > 3)) return false;
				*d |= 2 << 13
					| 0 << 12
					| disk->d.flop8.unused_bits_4_7 << 8
					| disk->d.flop8.dev << 3
					| (disk->d.flop8.door > 1 ? disk->d.flop8.door + 2 : disk->d.flop8.door);
				break;
			case CRK5_CFG_DISK_FLOP5:
				if ((disk->d.flop5.density > CRK5_CFG_FLOP5_HD) || (disk->d.flop5.dev > 0b111)) return false;
				*d |= 2 << 13
					| 1 << 12
					| disk->d.flop5.unused_bits_4_9 << 6
					| disk->d.flop5.density << 3
					| disk->d.flop5.dev;
				break;
			case CRK5_CFG_DISK_PLIX:
				if ((disk->d.plix.type > CRK5_CFG_PLIX_DEV_9425_WHOLE) || (disk->d.plix.controller > 0b11111) || (disk->d.plix.dev > 0b111)) return false;
				*d |= 3 << 13
					| disk->d.plix.unused_bits_3_5 << 10
					| disk->d.plix.type << 8
					| disk->d.plix.controller << 3
					| disk->d.plix.dev;
				break;
			default:
				return false;
		}
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_chandev_decode(uint16_t d, struct crk5_cfg_chandev *chandev)
{
	if (d == 0xffff) {
		chandev->configured = false;
	} else {
		chandev->configured = true;
		chandev->chan = (d >> 1) & 0b1111;
		chandev->dev = (d >> 5) & 0b111;
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_chandev_encode(struct crk5_cfg_chandev *chandev, uint16_t *d)
{
	if (!chandev || !d) return false;

	if (!chandev->configured) {
		*d = 0xffff;
	} else {
		if ((chandev->chan > 0b1111) || (chandev->dev > 0b111)) return false;
		*d = (chandev->chan << 1)
			| chandev->dev << 5;
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_tape_decode(uint16_t d, struct crk5_cfg_tape *tape)
{
	// TODO: not really sure how tape drive configuration works
	tape->raw = d;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_tape_encode(struct crk5_cfg_tape *tape, uint16_t *d)
{
	if (!tape || !d) return false;

	// TODO: not really sure how tape drive configuration works
	*d = tape->raw;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_int_decode(uint16_t d, unsigned *i, int shift, int mask)
{
	*i = (d >> shift) & mask;
	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_int_encode(unsigned i, uint16_t *d, int shift, int mask)
{
	if (!d) return false;
	if (i > mask) return false;
	*d |= (i & mask) << shift;
	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_mongroup_decode(uint16_t d, struct crk5_cfg_mongroup *mongroup)
{
	if (d == 0xffff) {
		mongroup->configured = false;
	} else {
		mongroup->configured = true;
		mongroup->dev = d;
	}
	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_mongroup_encode(struct crk5_cfg_mongroup *mongroup, uint16_t *d)
{
	if (!mongroup->configured) {
		*d = 0xffff;
	} else {
		// TODO: check max?
		*d = mongroup->dev;
	}
	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_oprq_decode(uint16_t d, struct crk5_cfg_oprq *oprq)
{
	if (d == 0xffff) {
		oprq->multix_reset = true;
	} else {
		oprq->multix_reset = false;
		oprq->ini_line = d;
	}
	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_oprq_encode(struct crk5_cfg_oprq *oprq, uint16_t *d)
{
	if (oprq->multix_reset) {
		*d = 0xffff;
	} else {
		// TODO: check max?
		*d = oprq->ini_line;
	}
	return true;
}

// -----------------------------------------------------------------------
const char * crk5_cfg_line_dir_name(unsigned dir)
{
	static const char *names[] = {
		_INVALID_,
		_INVALID_,
		"input",
		_INVALID_,
		"output",
		_INVALID_,
		"half duplex",
		"full duplex"
	};

	if (dir > 7) return names[0];
	return names[dir];
}

// -----------------------------------------------------------------------
const char * crk5_cfg_line_type_name(unsigned type)
{
	static const char *names[] = {
		"serial (USART)",
		"parallel (8255)",
		"serial (SYNC)",
		_INVALID_
	};

	if (type > 2) return names[3];
	return names[type];
}

// -----------------------------------------------------------------------
const char * crk5_cfg_line_protocol_name(unsigned proto)
{
	static const char *names[] = {
		"punched tape reader",
		"printer, tape puncher",
		"terminal",
		_INVALID_
	};

	if (proto > 2) return names[3];
	return names[proto];
}

// -----------------------------------------------------------------------
bool crk5_cfg_lines_decode(uint16_t d, struct crk5_cfg_lines *lines)
{
	if (d == 0xffff) {
		lines->configured = false;
	} else {
		lines->multix = d >> 8;
		lines->configured = true;
		if (lines->multix) {
			lines->d.mx.dir = d >> 13;
			if ((lines->d.mx.dir < 2) || (lines->d.mx.dir == 3) || (lines->d.mx.dir == 5) || (lines->d.mx.dir > 7)) return false;
			lines->d.mx.used = d & (1<<12);
			lines->d.mx.type = (d >> 8) & 0b1111;
			if (lines->d.mx.type > CRK5_CFG_LINE_TYPE_SYNC) return false;
			lines->d.mx.protocol = (d >> 5) & 0b111;
			if (lines->d.mx.protocol > CRK5_CFG_LINE_PROTO_TERMINAL) return false;
			lines->d.mx.count = (d & 0b11111) + 1;
		} else {
			lines->d.ch.dev = d & 0b1111;
			lines->d.ch.unused_bits_0_11 = d >> 4;
		}
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_lines_encode(struct crk5_cfg_lines *lines, uint16_t *d)
{
	if (!lines || !d) return false;

	if (!lines->configured) {
		*d = 0xffff;
	} else if (lines->multix) {
		if ((lines->d.mx.dir != CRK5_CFG_LINE_DIR_INPUT) && (lines->d.mx.dir != CRK5_CFG_LINE_DIR_OUTPUT) && (lines->d.mx.dir != CRK5_CFG_LINE_DIR_HDUPLEX) && (lines->d.mx.dir != CRK5_CFG_LINE_DIR_FDUPLEX)) return false;
		if (lines->d.mx.type > CRK5_CFG_LINE_TYPE_SYNC) return false;
		if ((lines->d.mx.protocol > CRK5_CFG_LINE_PROTO_TERMINAL) || (lines->d.mx.count > 32)) return false;
		*d = lines->d.mx.dir << 13
			| lines->d.mx.used << 12
			| lines->d.mx.type << 8
			| lines->d.mx.protocol << 5
			| (lines->d.mx.count - 1);
	} else {
		if (lines->d.ch.dev > 15) return false;
		*d = lines->d.ch.unused_bits_0_11 << 4
			| lines->d.ch.dev;
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_winch_quant_decode(uint16_t d, unsigned *winch_quant)
{
	*winch_quant = d;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_winch_quant_encode(unsigned winch_quant, uint16_t *d)
{
	if (winch_quant > 0xffff) return false;
	*d = winch_quant;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_winch_type_decode(uint16_t d, struct crk5_cfg_winch_type *wtype)
{
	wtype->unused_bits_0_3 = (d >> 12) & 0b1111;
	wtype->big = d >> 11;
	wtype->heads = 1 + ((d >> 8) & 0b111);
	wtype->unused_bits_8_15 = d & 0xff;

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_winch_type_encode(struct crk5_cfg_winch_type *wtype, uint16_t *d)
{
	if (wtype->heads > 8) return false;
	*d = wtype->unused_bits_0_3 << 12
		| wtype->big << 11
		| ((wtype->heads - 1) << 8)
		| (wtype->unused_bits_8_15 & 0xff);

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_winch_park_decode(uint16_t d, struct crk5_cfg_winch_type *wtype)
{
	if (d == 0xffff) {
		wtype->autopark = true;
	} else {
		wtype->autopark = false;
		wtype->park_cyl = d;
	}

	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_winch_park_encode(struct crk5_cfg_winch_type *wtype, uint16_t *d)
{
	if (wtype->autopark) {
		*d = 0xffff;
	} else {
		if (wtype->park_cyl > 0xffff) return false;
		*d = wtype->park_cyl;
	}
	return true;
}

// -----------------------------------------------------------------------
bool crk5_cfg_decode(uint16_t *d, struct crk5_cfg *cfg)
{
	int i;
	bool res = true;

	res &= crk5_cfg_sys4_decode(d[0x04], &cfg->sys);
	res &= crk5_cfg_mx_decode(d[0x05], &cfg->mx);
	res &= crk5_cfg_sys6_decode(d[0x06], &cfg->sys);
	res &= crk5_cfg_sys7_decode(d[0x07], &cfg->sys);
	for (i=0 ; i<CRK5_CFG_MEM_SLOTS ; i++) res &= crk5_cfg_mem_decode(d[0x08+i], cfg->mem+i);
	for (i=0 ; i<CRK5_CFG_DISK_SLOTS ; i++) res &= crk5_cfg_disk_decode(d[0x10+i], cfg->disk+i);
	for (i=0 ; i<CRK5_CFG_TAPE_SLOTS ; i++) res &= crk5_cfg_tape_decode(d[0x20+i], cfg->tape+i);
	for (i=0 ; i<CRK5_CFG_CAMAC_SLOTS ; i++) res &= crk5_cfg_chandev_decode(d[0x24+i], cfg->ch_camac+i);
	res &= crk5_cfg_chandev_decode(d[0x28], &cfg->ch_plix);
	res &= crk5_cfg_chandev_decode(d[0x29], &cfg->ch_multix);
	for (i=0 ; i<CRK5_CFG_CHAR_SLOTS ; i++) res &= crk5_cfg_chandev_decode(d[0x2a+i], cfg->ch_char+i);
	res &= crk5_cfg_chandev_decode(d[0x2d], &cfg->net);
	res &= crk5_cfg_chandev_decode(d[0x2e], &cfg->rtc);
	res &= crk5_cfg_int_decode(d[0x28], &cfg->winch_line, 8, 0b111111);
	res &= crk5_cfg_int_decode(d[0x29], &cfg->floppy_line, 8, 0b111111);
	cfg->unused_word_2c = d[0x2c];;
	res &= crk5_cfg_mongroup_decode(d[0x2f], &cfg->mongroup);
	res &= crk5_cfg_oprq_decode(d[0x30], &cfg->oprq);
	for (i=0 ; i<CRK5_CFG_LINE_SLOTS ; i++) res &= crk5_cfg_lines_decode(d[0x31+i], cfg->lines+i);

	res &= crk5_cfg_winch_quant_decode(d[0x63], &cfg->winch_quant);
	for (i=0 ; i<CRK5_CFG_WINCH_TYPE_SLOTS ; i++) {
		res &= crk5_cfg_winch_type_decode(d[0x64+i], cfg->winch_type+i);
		res &= crk5_cfg_winch_park_decode(d[0x68+i], cfg->winch_type+i);
	}

	return res;
}

// -----------------------------------------------------------------------
bool crk5_cfg_encode(struct crk5_cfg *cfg, uint16_t *d)
{
	int i;
	bool res = true;

	res &= crk5_cfg_sys4_encode(&cfg->sys, d+0x04);
	res &= crk5_cfg_mx_encode(&cfg->mx, d+0x05);
	res &= crk5_cfg_sys6_encode(&cfg->sys, d+0x06);
	res &= crk5_cfg_sys7_encode(&cfg->sys, d+0x07);
	for (i=0 ; i<CRK5_CFG_MEM_SLOTS ; i++) res &= crk5_cfg_mem_encode(cfg->mem+i, d+0x08+i);
	for (i=0 ; i<CRK5_CFG_DISK_SLOTS ; i++) res &= crk5_cfg_disk_encode(cfg->disk+i, d+0x10+i);
	for (i=0 ; i<CRK5_CFG_TAPE_SLOTS ; i++) res &= crk5_cfg_tape_encode(cfg->tape+i, d+0x20+i);
	for (i=0 ; i<CRK5_CFG_CAMAC_SLOTS ; i++) res &= crk5_cfg_chandev_encode(cfg->ch_camac+i, d+0x24+i);
	res &= crk5_cfg_chandev_encode(&cfg->ch_plix, d+0x28);
	res &= crk5_cfg_chandev_encode(&cfg->ch_multix, d+0x29);
	for (i=0 ; i<CRK5_CFG_CHAR_SLOTS ; i++) res &= crk5_cfg_chandev_encode(cfg->ch_char+i, d+0x2a+i);
	res &= crk5_cfg_chandev_encode(&cfg->net, d+0x2d);
	res &= crk5_cfg_chandev_encode(&cfg->rtc, d+0x2e);
	res &= crk5_cfg_int_encode(cfg->winch_line, d+0x28, 8, 0b111111);
	res &= crk5_cfg_int_encode(cfg->floppy_line, d+0x29, 8, 0b111111);
	d[0x2c] = cfg->unused_word_2c;
	res &= crk5_cfg_mongroup_encode(&cfg->mongroup, d+0x2f);
	res &= crk5_cfg_oprq_encode(&cfg->oprq, d+0x30);
	for (i=0 ; i<CRK5_CFG_LINE_SLOTS ; i++) res &= crk5_cfg_lines_encode(cfg->lines+i, d+0x31+i);

	res &= crk5_cfg_winch_quant_encode(cfg->winch_quant, d+0x63);
	for (i=0 ; i<CRK5_CFG_WINCH_TYPE_SLOTS ; i++) {
		res &= crk5_cfg_winch_type_encode(cfg->winch_type+i, d+0x64+i);
		res &= crk5_cfg_winch_park_encode(cfg->winch_type+i, d+0x68+i);
	}

	return res;
}

// -----------------------------------------------------------------------
bool crk5_cfg_load(FILE *f, off_t kernel_offset, struct crk5_cfg *cfg)
{
	uint16_t buf[CRK5_CFG_SIZE_WORDS];

	if (fseek(f, kernel_offset*2, SEEK_SET)) {
		return false;
	}
	if (fread(buf, sizeof(uint16_t), CRK5_CFG_SIZE_WORDS, f) != CRK5_CFG_SIZE_WORDS) {
		return false;
	}

	for (int i=0 ; i<CRK5_CFG_SIZE_WORDS ; i++) {
		buf[i] = ntohs(buf[i]);
	}

	return crk5_cfg_decode(buf, cfg);
}

// -----------------------------------------------------------------------
bool crk5_cfg_save(struct crk5_cfg *cfg, FILE *f, off_t kernel_offset)
{
	uint16_t buf[CRK5_CFG_SIZE_WORDS];

	if (!crk5_cfg_encode(cfg, buf)) {
		return false;
	}

	for (int i=0 ; i<CRK5_CFG_SIZE_WORDS ; i++) {
		buf[i] = htons(buf[i]);
	}

	int ranges[2][2] = {
		{ CRK5_CFG_CFG_START, CRK5_CFG_CFG_WORDS },
		{ CRK5_CFG_WTYPES_START, CRK5_CFG_WTYPES_WORDS },
	};
	for (int j=0 ; j<2 ; j++) {
		if (fseek(f, (kernel_offset + ranges[j][0]) * 2, SEEK_SET)) {
			return false;
		}
		if (fwrite(buf + ranges[j][0], sizeof(uint16_t), ranges[j][1], f) != ranges[j][1]) {
			return false;
		}
	}
	return true;
}

// -----------------------------------------------------------------------
void crk5_cfg_init_empty(struct crk5_cfg *cfg)
{
	memset(cfg, 0, sizeof(struct crk5_cfg));
}

// vim: tabstop=4 shiftwidth=4 autoindent
